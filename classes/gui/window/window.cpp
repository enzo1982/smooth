 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/window/backends/windowbackend.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/statusbar.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/math.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/mdi/window.h>
#include <smooth/basic/input.h>
#include <smooth/resources.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/surface.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::Window::classID = S::Object::RequestClassID();
S::Int		 S::GUI::Window::nOfActiveWindows = 0;

S::GUI::Window::Window(const String &title, const Point &iPos, const Size &iSize, Void *iWindow) : Widget(iPos, iSize)
{
	backend = WindowBackend::CreateBackendInstance();
	backend->onEvent.SetParentObject(this);
	backend->onEvent.Connect(&Window::Process, this);

	stay		= False;
	maximized	= False;
	minimized	= False;

	type = classID;

	nOfActiveWindows++;

	if (title != NIL)	text = title;
	else			text = "smooth Application";

	frameWidth = 2;
	updateRect = Rect(Point(-1, -1), Size(0, 0));

	icon = NIL;

	created		= False;
	visible		= False;
	destroyed	= False;
	initshow	= False;

	trackMenu = NIL;

	mainLayer = new Layer();

	RegisterObject(mainLayer);

	doQuit.Connect(True);

	onCreate.SetParentObject(this);
	onPaint.SetParentObject(this);
	onResize.SetParentObject(this);
	onPeek.SetParentObject(this);
	onEvent.SetParentObject(this);
}

S::GUI::Window::~Window()
{
	if (created && !destroyed)	backend->Close();
	else if (!created)		nOfActiveWindows++;

	UnregisterObject(mainLayer);
	DeleteObject(mainLayer);

	if (trackMenu != NIL) ClosePopupMenu();

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop--;

	delete backend;
}

S::Int S::GUI::Window::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (created) backend->SetMetrics(nPos, nSize);

	Bool	 resized = (GetWidth() != nSize.cx || GetHeight() != nSize.cy);
	Bool	 prevVisible = visible;

	visible = False;

	Widget::SetMetrics(nPos, nSize);

	if (resized)
	{
		Surface	*surface = GetDrawSurface();

		surface->SetSize(GetSize());

		CalculateOffsets();

		onResize.Emit();
	}

	visible = prevVisible;

	return Success();
}

S::GUI::Bitmap &S::GUI::Window::GetIcon()
{
	return icon;
}

S::Int S::GUI::Window::SetIcon(const Bitmap &nIcon)
{
	Bitmap	 newIcon = nIcon;

#ifdef __WIN32__
	if (nIcon == NIL) newIcon = Bitmap((HBITMAP) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDB_ICON), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_SHARED));
#endif

	if (newIcon != NIL)
	{
		icon = newIcon;

		icon.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

		backend->SetIcon(icon);
	}

	return Success();
}

S::GUI::Layer *S::GUI::Window::GetMainLayer()
{
	return mainLayer;
}

S::Int S::GUI::Window::SetText(const String &nTitle)
{
	text = nTitle;

	if (created)
	{
		Process(SM_WINDOWTITLECHANGED, 0, 0);

		backend->SetTitle(text);
	}

	return Success();
}

S::Int S::GUI::Window::SetStatusText(const String &nStatus)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID)
		{
			widget->SetText(nStatus);

			return Success();
		}
	}

	return Error();
}

const S::String &S::GUI::Window::GetStatusText()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID) return widget->GetText();
	}

	return defaultStatus;
}

S::Int S::GUI::Window::SetDefaultStatusText(const String &nStatus)
{
	defaultStatus = nStatus;

	RestoreDefaultStatusText();

	return Success();
}

S::Int S::GUI::Window::RestoreDefaultStatusText()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget *widget = GetNthObject(i);

		if (widget == NIL) continue;

		if (widget->GetObjectType() == Statusbar::classID)
		{
			widget->SetText(defaultStatus);

			break;
		}
	}

	return Success();
}

S::Int S::GUI::Window::Show()
{
	if (!created) Create();

	if (maximized && !initshow)
	{
		backend->Hide();

		maximized = False;
		Maximize();
	}

	if (minimized && !initshow)
	{
		backend->Hide();

		minimized = False;
		Minimize();
	}

	backend->Show();

	initshow	= True;
	visible		= True;

	onShow.Emit();

	return Success();
}

S::Int S::GUI::Window::Hide()
{
	if (!created) Create();

	backend->Hide();

	if (maximized && !initshow)
	{
		maximized = False;
		Maximize();
	}

	if (minimized && !initshow)
	{
		minimized = False;
		Minimize();
	}

	initshow	= True;
	visible		= False;

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Window::Minimize()
{
	if (minimized) return Success();

	if (!created)
	{
		minimized = True;

		return Success();
	}

	backend->Minimize();

	minimized = True;

	return Success();
}

S::Int S::GUI::Window::Maximize()
{
	if (maximized) return Success();

	if (!created)
	{
		maximized = True;

		return Success();
	}

	backend->Maximize();

	maximized = True;

	return Success();
}

S::Int S::GUI::Window::Restore()
{
	if (!maximized && !minimized) return Success();

	if (!created)
	{
		maximized = False;
		minimized = False;

		return Success();
	}

	backend->Restore();

	maximized = False;
	minimized = False;

	return Success();
}

S::Bool S::GUI::Window::IsMaximized()
{
	return maximized;
}

S::Bool S::GUI::Window::IsMinimized()
{
	return minimized;
}

S::GUI::Point S::GUI::Window::GetRealPosition()
{
	return Point(0, 0);
}

S::GUI::Rect S::GUI::Window::GetWindowRect()
{
	return Rect(GetPosition(), GetSize());
}

S::GUI::Rect S::GUI::Window::GetClientRect()
{
	return Rect(Point(innerOffset.left, innerOffset.top), GetSize() - Size(innerOffset.left + innerOffset.right, innerOffset.top + innerOffset.bottom));
}

S::GUI::Rect S::GUI::Window::GetRestoredWindowRect()
{
	if (IsMaximized())	return backend->GetRestoredWindowRect();
	else			return GetWindowRect();
}

const S::GUI::Rect &S::GUI::Window::GetUpdateRect()
{
	return updateRect;
}

S::Int S::GUI::Window::SetUpdateRect(const Rect &newUpdateRect)
{
	updateRect = newUpdateRect;

	return Success();
}

S::Int S::GUI::Window::SetMinimumSize(const Size &newMinSize)
{
	backend->SetMinimumSize(newMinSize);

	return Success();
}

S::Int S::GUI::Window::SetMaximumSize(const Size &newMaxSize)
{
	backend->SetMaximumSize(newMaxSize);

	return Success();
}

S::Bool S::GUI::Window::Create()
{
	if (IsRegistered() && !created)
	{
		if (backend->Open(text, GetPosition(), GetSize(), flags) == Success())
		{
			created = True;
			visible = False;

			CalculateOffsets();

			onCreate.Emit();

			return True;
		}
		else
		{
			return False;
		}
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop++;

	return False;
}

S::Int S::GUI::Window::Stay()
{
	if (!IsRegistered()) return Error();

	SetFlags(flags | WF_MODAL);

	if (!created)	Create();
	if (!visible)	Show();

	stay	= True;

#ifdef __WIN32__
	if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_KILLFOCUS, 0, 0);
	else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_KILLFOCUS, 0, 0);

	if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_ACTIVATEAPP, 1, 0);
	else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_ACTIVATEAPP, 1, 0);
#endif

	System::EventProcessor	*event = new System::EventProcessor();

	while (!destroyed)
	{
		if (peekLoop > 0)	event->ProcessNextEvent(False);
		else			event->ProcessNextEvent(True);
	}

	delete event;

#ifdef __WIN32__
	if (nOfActiveWindows == 0 && !initializing) PostQuitMessage(0);
#endif

	return Success();
}

S::Int S::GUI::Window::Close()
{
#ifdef __WIN32__
	if (Setup::enableUnicode)	::PostMessageW((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);
	else				::PostMessageA((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);
#endif

	return Success();
}

S::Bool S::GUI::Window::IsInUse()
{
	return (created && !destroyed);
}

S::Int S::GUI::Window::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return Success();

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

	Int	 rVal = Success();

#ifdef __WIN32__
	if (trackMenu != NIL && (message == SM_LBUTTONDOWN || message == SM_RBUTTONDOWN || message == WM_KILLFOCUS))
	{
		Bool	 destroyPopup = True;

		if (message == WM_KILLFOCUS && Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->GetHandle() >= trackMenu->GetHandle()) destroyPopup = False;

		if (destroyPopup) ClosePopupMenu();
	}

	switch (message)
	{
		case WM_CLOSE:
			if (doQuit.Call()) backend->Close();

			rVal = Break;

			break;
		case WM_DESTROY:
			destroyed = True;

			if (nOfActiveWindows == 0 && loopActive)
			{
				if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
				else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
			}
			else
			{
				nOfActiveWindows--;
			}

			rVal = Break;

			break;
		case WM_QUIT:
			destroyed = True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return Break;
		case WM_SYSCOLORCHANGE:
			GetColors();

			break;
		case WM_PAINT:
			{
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = uRect;

				if (::GetUpdateRect((HWND) backend->GetSystemWindow(), &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right += 5;
					updateRect.bottom += 5;

					PAINTSTRUCT	 ps;

					BeginPaint((HWND) backend->GetSystemWindow(), &ps);

					if (Math::Abs((updateRect.right - updateRect.left) - GetWidth()) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - GetHeight()) < 20)	Paint(SP_PAINT);
					else																		Paint(SP_UPDATE);

					EndPaint((HWND) backend->GetSystemWindow(), &ps);
				}
			}

			rVal = Break;

			break;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndpos = (LPWINDOWPOS) lParam;

				SetMetrics(Point(wndpos->x, wndpos->y), Size(wndpos->cx, wndpos->cy));
			}

			break;
		case WM_SIZE:
			if (wParam == SIZE_RESTORED) minimized = False;

			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE && !(flags & WF_SYSTEMMODAL))
			{
				for (Int i = 0; i < Object::GetNOfObjects(); i++)
				{
					Object	*object = Object::GetNthObject(i);

					if (object == NIL) continue;

					if (object->GetObjectType() == classID && object->GetHandle() > GetHandle() && (object->GetFlags() & WF_MODAL))
					{
						SetActiveWindow((HWND) ((Window *) object)->GetSystemWindow());

						rVal = Break;

						break;
					}
				}
			}

			break;
		case WM_SETFOCUS:
			if (!focussed)
			{
				focussed = True;

				onGetFocus.Emit();
			}

			break;
		case WM_KILLFOCUS:
			if (Window::GetWindow((HWND) wParam) != NIL)
			{
				if (Window::GetWindow((HWND) wParam)->GetObjectType() == ToolWindow::classID && Window::GetWindow((HWND) wParam)->GetHandle() >= GetHandle()) break;
			}

			if (focussed)
			{
				focussed = False;

				onLoseFocus.Emit();
			}

		case WM_ACTIVATEAPP:
			if (flags & WF_MODAL)
			{
				Bool	 activate = False;
				HWND	 actWnd = GetActiveWindow();

				if (actWnd == (HWND) backend->GetSystemWindow()) break;

				if (GetWindow(actWnd) == NIL)					activate = False;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	break;
				else if (GetWindow(actWnd)->GetHandle() < GetHandle())		activate = True;
				else if (GetWindow(actWnd)->GetHandle() > GetHandle())		GetWindow(actWnd)->SetFlags(WF_MODAL);

				if (activate && message == WM_ACTIVATEAPP)
				{
					if (wParam)	activate = True;
					else		activate = False;
				}

				if (activate && message == WM_KILLFOCUS)
				{
					if (GetWindow(SetActiveWindow((HWND) backend->GetSystemWindow())) != NIL)	activate = True;
					else										activate = False;
				}

				if (activate)	SetWindowPos((HWND) backend->GetSystemWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				else		SetWindowPos((HWND) backend->GetSystemWindow(), message == WM_KILLFOCUS ? HWND_NOTOPMOST : GetForegroundWindow(), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			}

			if (flags & WF_APPTOPMOST)
			{
				Bool	 activate = False;
				HWND	 actWnd = GetActiveWindow();

				if (actWnd == (HWND) backend->GetSystemWindow()) break;

				if (GetWindow(actWnd) == NIL)					activate = False;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	break;
				else								activate = True;

				if (activate && message == WM_ACTIVATEAPP)
				{
					if (wParam)	activate = True;
					else		activate = False;
				}

				if (activate) SetWindowPos((HWND) backend->GetSystemWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				else
				{
					SetWindowPos((HWND) backend->GetSystemWindow(), GetForegroundWindow(), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
					SetWindowPos((HWND) backend->GetSystemWindow(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				}
			}

			if (flags & WF_SYSTEMMODAL && message == WM_KILLFOCUS)
			{
				Bool	 activate = False;
				HWND	 actWnd = GetForegroundWindow();

				if (actWnd == (HWND) backend->GetSystemWindow()) break;

				if (GetWindow(actWnd) == NIL)					activate = True;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	activate = False;
				else if (GetWindow(actWnd)->GetHandle() < GetHandle())		activate = True;
				else if (GetWindow(actWnd)->GetHandle() > GetHandle())		GetWindow(actWnd)->SetFlags(WF_SYSTEMMODAL);

				if (activate)
				{
					SetWindowPos((HWND) backend->GetSystemWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetForegroundWindow((HWND) backend->GetSystemWindow());
				}
			}

			break;
		case SM_EXECUTEPEEK:
			onPeek.Emit();

			rVal = Break;

			break;
		case SM_RBUTTONDOWN:
			OpenPopupMenu();

			if (trackMenu != NIL) rVal = Break;

			break;
	}
#endif

	if (rVal == Success())
	{
		for (Int i = GetNOfObjects() - 1; i >= 0; i--)
		{
			Widget	*object = GetNthObject(i);

			if (object == NIL) continue;

			if (object->Process(message, wParam, lParam) == Break)
			{
				rVal = Break;

				break;
			}
		}
	}

	LeaveProtectedRegion();

	return rVal;
}

S::Int S::GUI::Window::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!created)		return Success();
	if (!IsVisible())	return Success();

	if ((updateRect.right - updateRect.left == 0) || (updateRect.bottom - updateRect.top == 0)) return Success();

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (updateRect.left < frameWidth)			updateRect.left		= frameWidth;
	if (updateRect.top < frameWidth)			updateRect.top		= frameWidth;
	if (GetWidth() - updateRect.right < frameWidth)		updateRect.right	= GetWidth() - frameWidth;
	if (GetHeight() - updateRect.bottom < frameWidth)	updateRect.bottom	= GetHeight() - frameWidth;

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		Bool	 preRTL = Setup::rightToLeft;

		Setup::rightToLeft = False;

		surface->StartPaint(updateRect);

		Setup::rightToLeft = preRTL;

		surface->Box(updateRect, Setup::BackgroundColor, FILLED);

		if (type != ToolWindow::classID)
		{
			Widget	*lastWidget = NIL;
			Int	 bias = 0;
			Int	 topoffset = 3;
			Int	 rightobjcount = 0;
			Int	 leftobjcount = 0;
			Int	 btmobjcount = 0;
			Int	 topobjcount = 0;

			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				Widget	*object = GetNthObject(i);

				if (object->GetOrientation() == OR_TOP)		topobjcount++;
				else if (object->GetOrientation() == OR_BOTTOM)	btmobjcount++;
				else if (object->GetOrientation() == OR_LEFT)	leftobjcount++;
				else if (object->GetOrientation() == OR_RIGHT)	rightobjcount++;

				if (object->GetOrientation() == OR_TOP)
				{
					lastWidget = object;

					if (object->subtype == WO_SEPARATOR)
					{
						bias = -3;

						topoffset += object->GetHeight() + 3;

						Point	 p1 = Point(4, topoffset - 2);
						Point	 p2 = Point(GetWidth() - 4, p1.y);

						if (icon != NIL) p1.x += 17;

						surface->Bar(p1, p2, OR_HORZ);
					}
					else
					{
						bias = 0;

						topoffset += object->GetHeight();
					}
				}
			}

			if (topobjcount > 0)
			{
				Point	 p1 = Point(4, innerOffset.top - 2 + bias);
				Point	 p2 = Point(GetWidth() - 4, p1.y);

				if (lastWidget->subtype == WO_NOSEPARATOR) { p1.y -= 3; p2.y -= 3; }

				surface->Bar(p1, p2, OR_HORZ);
				surface->Bar(p1 + Point(0, 2), p2 + Point(0, 2), OR_HORZ);
			}

			if (btmobjcount > 0)
			{
				Point	 p1 = Point(4, GetHeight() - innerOffset.bottom);
				Point	 p2 = Point(GetWidth() - 4, p1.y);

				surface->Bar(p1, p2, OR_HORZ);
				surface->Bar(p1 + Point(0, 2), p2 + Point(0, 2), OR_HORZ);
			}

			if (leftobjcount > 0)
			{
				Point	 p1 = Point(innerOffset.left - 3, innerOffset.top);
				Point	 p2 = Point(p1.x, GetHeight() - innerOffset.bottom - 2);

				surface->Bar(p1, p2, OR_VERT);
			}

			if (rightobjcount > 0)
			{
				Point	 p1 = Point(GetWidth() - innerOffset.right + 1, innerOffset.top);
				Point	 p2 = Point(p1.x, GetHeight() - innerOffset.bottom - 2);

				surface->Bar(p1, p2, OR_VERT);
			}
		}

		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			Widget	*widget = GetNthObject(j);

			if (widget->IsAffected(updateRect) && widget->GetObjectType() != Layer::classID) widget->Paint(SP_PAINT);
		}

		for (Int k = 0; k < GetNOfObjects(); k++)
		{
			Widget	*widget = GetNthObject(k);

			if (widget->IsAffected(updateRect) && widget->GetObjectType() == Layer::classID) widget->Paint(SP_PAINT);
		}

		onPaint.Emit();

		surface->EndPaint();
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::Window::CalculateOffsets()
{
	Widget	*lastWidget	= NIL;
	Int	 rightobjcount	= 0;
	Int	 leftobjcount	= 0;
	Int	 btmobjcount	= 0;
	Int	 topobjcount	= 0;

	if (GetObjectType() == ToolWindow::classID)	innerOffset = Rect(Point(0, 0), Size(0, 0));
	else						innerOffset = Rect(Point(3, 3), Size(0, 0));

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget->GetOrientation() == OR_TOP)
		{
			topobjcount++;

			lastWidget = widget;

			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(GetWidth() - innerOffset.left - innerOffset.right, widget->GetHeight()));

			innerOffset.top += widget->GetHeight();

			if (widget->subtype == WO_SEPARATOR) innerOffset.top += 3;
		}
	}

	if (topobjcount > 0)
	{
		innerOffset.top += 3;

		if (lastWidget->subtype == WO_NOSEPARATOR) innerOffset.top += 3;
	}

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		Widget	*widget = GetNthObject(j);

		if (widget->GetOrientation() == OR_BOTTOM)
		{
			btmobjcount++;

			widget->SetMetrics(Point(innerOffset.left, GetHeight() - innerOffset.bottom - widget->GetHeight()), Size(GetWidth() - innerOffset.left - innerOffset.right, widget->GetHeight()));

			innerOffset.bottom += widget->GetHeight();
		}
	}

	if (btmobjcount > 0) innerOffset.bottom += 4;

	for (Int k = 0; k < GetNOfObjects(); k++)
	{
		Widget	*widget = GetNthObject(k);

		if (widget->GetOrientation() == OR_LEFT)
		{
			leftobjcount++;

			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(widget->GetWidth(), GetHeight() - innerOffset.top - innerOffset.bottom));

			innerOffset.left += widget->GetWidth();
		}
	}

	if (leftobjcount > 0) innerOffset.left += 3;

	for (Int l = 0; l < GetNOfObjects(); l++)
	{
		Widget	*widget = GetNthObject(l);

		if (widget->GetOrientation() == OR_RIGHT)
		{
			rightobjcount++;

			widget->SetMetrics(Point(GetWidth() - innerOffset.right - widget->GetWidth(), innerOffset.top), Size(widget->GetWidth(), GetHeight() - innerOffset.top - innerOffset.bottom));

			innerOffset.right += widget->GetWidth();
		}
	}

	if (rightobjcount > 0) innerOffset.right += 3;

	for (Int m = 0; m < GetNOfObjects(); m++)
	{
		Widget	*widget = GetNthObject(m);

		if (widget->GetOrientation() == OR_CENTER)
		{
			widget->SetMetrics(Point(innerOffset.left, innerOffset.top), Size(GetWidth() - innerOffset.left - innerOffset.right, GetHeight() - innerOffset.top - innerOffset.bottom));
			widget->SetOrientation(OR_CENTER);
		}
	}
}

S::Int S::GUI::Window::MouseX()
{
	if (Setup::rightToLeft)	return GetWidth() - (Input::MouseX() - GetX()) - 1;
	else			return Input::MouseX() - GetX();
}

S::Int S::GUI::Window::MouseY()
{
	return Input::MouseY() - GetY();
}

S::Bool S::GUI::Window::IsMouseOn(const Rect &rect)
{
	Surface	*surface = GetDrawSurface();

	if (surface->GetSystemSurface() == NIL) return False;

#ifdef __WIN32__
	if (!PtVisible((HDC) surface->GetSystemSurface(), Input::MouseX() - GetX(), Input::MouseY() - GetY())) return False;
#endif

	if ((MouseX() >= rect.left) && (MouseX() < rect.right) && (MouseY() >= rect.top) && (MouseY() < rect.bottom))	return True;
	else														return False;
}

S::GUI::Surface *S::GUI::Window::GetDrawSurface()
{
	return backend->GetDrawSurface();
}

S::Void *S::GUI::Window::GetSystemWindow()
{
	return backend->GetSystemWindow();
}

S::Void S::GUI::Window::OpenPopupMenu()
{
	trackMenu = getTrackMenu.Call(MouseX(), MouseY());

	if (trackMenu != NIL)
	{
		trackMenu->SetPosition(Point(MouseX(), MouseY()));
		trackMenu->internalRequestClose.Connect(&Window::ClosePopupMenu, this);

		RegisterObject(trackMenu);
	}
}

S::Void S::GUI::Window::ClosePopupMenu()
{
	if (trackMenu != NIL)
	{
		trackMenu->internalRequestClose.Disconnect(&Window::ClosePopupMenu, this);

		UnregisterObject(trackMenu);

		trackMenu = NIL;
	}
}

S::Int S::GUI::Window::RegisterObject(Widget *object)
{
	if (object->GetOrientation() == OR_UPPERLEFT || object->GetOrientation() == OR_UPPERRIGHT || object->GetOrientation() == OR_LOWERLEFT || object->GetOrientation() == OR_LOWERRIGHT) return mainLayer->RegisterObject(object);

	if (Widget::RegisterObject(object) == Success())
	{
		if (object->GetObjectType() == Titlebar::classID)
		{
			if (!Binary::IsFlagSet(object->GetFlags(), TB_MAXBUTTON)) flags = flags | WF_NORESIZE;
		}
		else if (object->GetObjectType() == Statusbar::classID)
		{
			SetDefaultStatusText(object->GetText());
		}

		if (object->GetOrientation() != OR_FREE) CalculateOffsets();

		return Success();
	}

	return Error();
}

S::Int S::GUI::Window::UnregisterObject(Widget *object)
{
	if (object->GetOrientation() == OR_UPPERLEFT || object->GetOrientation() == OR_UPPERRIGHT || object->GetOrientation() == OR_LOWERLEFT || object->GetOrientation() == OR_LOWERRIGHT) return mainLayer->UnregisterObject(object);

	if (Widget::UnregisterObject(object) == Success())
	{
		if (object->GetOrientation() != OR_FREE) CalculateOffsets();

		return Success();
	}

	return Error();
}

S::GUI::Window *S::GUI::Window::GetWindow(Void *sysWindow)
{
	if (sysWindow == NIL) return NIL;

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*window = Object::GetNthObject(i);

		if (window == NIL) continue;

		if (window->GetObjectType() == Window::classID || window->GetObjectType() == MDI::Window::classID || window->GetObjectType() == ToolWindow::classID)
		{
			if (((Window *) window)->GetSystemWindow() == sysWindow) return (Window *) window;
		}
	}

	return NIL;
}
