 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/window/windowbackend.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/basic/statusbar.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/misc/math.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/system/timer.h>
#include <smooth/color.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/mdi/window.h>
#include <smooth/basic/input.h>
#include <smooth/resources.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/surface.h>
#include <smooth/dllmain.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::Window::classID = S::Object::RequestClassID();
S::Int		 S::GUI::Window::nOfActiveWindows = 0;

S::GUI::Window::Window(String title, Void *iWindow)
{
	backend = WindowBackend::CreateBackendInstance();
	backend->onEvent.SetParentObject(this);
	backend->onEvent.Connect(&Window::Process, this);

	containerType = classID;

	possibleContainers.AddEntry(Application::classID);

	stay		= False;
	maximized	= False;
	minimized	= False;

	type = classID;

	nOfActiveWindows++;

	if (title != NIL)	text = title;
	else			text = "smooth Application";

	value = 0;

	innerOffset = Rect(Point(3, 3), Size(0, 0));

	updateRect = Rect(Point(-1, -1), Size(0, 0));
	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	icon = NIL;

	created		= False;
	visible		= False;
	destroyed	= False;
	initshow	= False;
	firstPaint	= True;

	size = Size(200, 200);

	trackMenu = NIL;
	paintTimer = NIL;

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

	if (trackMenu != NIL)
	{
		UnregisterObject(trackMenu);
		DeleteObject(trackMenu);
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop--;

	delete backend;
}

S::Int S::GUI::Window::SetMetrics(const Point &nPos, const Size &nSize)
{
	updateRect = Rect();

	if (created) backend->SetMetrics(nPos, nSize);

	pos	= nPos;
	size	= nSize;

	return Success;
}

S::GUI::Bitmap &S::GUI::Window::GetIcon()
{
	return icon;
}

S::Int S::GUI::Window::SetIcon(const Bitmap &nIcon)
{
	Bitmap	 newIcon = nIcon;

#ifdef __WIN32__
	if (&nIcon == &SI_DEFAULT) newIcon = Bitmap((HBITMAP) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDB_ICON), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_SHARED));
#endif

	if (newIcon != NIL)
	{
		icon = newIcon;

		icon.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

		backend->SetIcon(icon);

		return Success;
	}
	else
	{
		return Success;
	}
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

	return Success;
}

S::Int S::GUI::Window::SetStatusText(String nStatus)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			((Statusbar *) object)->SetText(nStatus);

			return Success;
		}
	}

	return Failure;
}

S::String S::GUI::Window::GetStatusText()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			return ((Statusbar *) object)->GetText();
		}
	}

	return NIL;
}

S::Int S::GUI::Window::SetDefaultStatusText(String nStatus)
{
	defaultStatus = nStatus;

	RestoreDefaultStatusText();

	return Success;
}

S::Int S::GUI::Window::RestoreDefaultStatusText()
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			((Statusbar *) object)->SetText(defaultStatus);

			break;
		}
	}

	return Success;
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

	return Success;
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

	return Success;
}

S::Int S::GUI::Window::Minimize()
{
	if (minimized) return Success;

	if (!created)
	{
		minimized = True;

		return Success;
	}

	backend->Minimize();

	minimized = True;

	return Success;
}

S::Int S::GUI::Window::Maximize()
{
	if (maximized) return Success;

	if (!created)
	{
		maximized = True;

		return Success;
	}

	backend->Maximize();

	maximized = True;

	return Success;
}

S::Int S::GUI::Window::Restore()
{
	if (!maximized && !minimized) return Success;

	if (!created)
	{
		maximized = False;
		minimized = False;

		return Success;
	}

	backend->Restore();

	maximized = False;
	minimized = False;

	return Success;
}

S::Bool S::GUI::Window::IsMaximized()
{
	return maximized;
}

S::Bool S::GUI::Window::IsMinimized()
{
	return minimized;
}

S::GUI::Rect S::GUI::Window::GetWindowRect()
{
	return Rect(pos, size);
}

S::GUI::Rect S::GUI::Window::GetClientRect()
{
	return innerOffset;
}

S::GUI::Rect S::GUI::Window::GetRestoredWindowRect()
{
	if (IsMaximized())	return backend->GetRestoredWindowRect();
	else			return GetWindowRect();
}

S::GUI::Rect S::GUI::Window::GetUpdateRect()
{
	if (timedUpdateRect.left == -1	&&
	    timedUpdateRect.top == -1	&&
	    timedUpdateRect.right == -1	&&
	    timedUpdateRect.bottom == -1)	return updateRect;
	else					return timedUpdateRect;
}

S::Int S::GUI::Window::SetUpdateRect(Rect newUpdateRect)
{
	updateRect = newUpdateRect;

	return Success;
}

S::Int S::GUI::Window::SetMinimumSize(Size newMinSize)
{
	backend->SetMinimumSize(newMinSize);

	return Success;
}

S::Int S::GUI::Window::SetMaximumSize(Size newMaxSize)
{
	backend->SetMaximumSize(newMaxSize);

	return Success;
}

S::Bool S::GUI::Window::Create()
{
	if (registered && !created)
	{
		if (backend->Open(text, pos, size, flags) == Success)
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
	if (!registered) return value;

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

	return value;
}

S::Int S::GUI::Window::Close()
{
	Process(SM_LOOSEFOCUS, 0, 0);

#ifdef __WIN32__
	if (Setup::enableUnicode)	::PostMessageW((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);
	else				::PostMessageA((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);
#endif

	return Success;
}

S::Bool S::GUI::Window::IsInUse()
{
	return (created && !destroyed);
}

S::Int S::GUI::Window::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return Success;

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

	Int	 rVal = -1;

#ifdef __WIN32__
	if (trackMenu != NIL && (message == SM_LBUTTONDOWN || message == SM_RBUTTONDOWN || message == WM_KILLFOCUS))
	{
		Bool	 destroyPopup = True;

		if (message == WM_KILLFOCUS && Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->GetHandle() >= trackMenu->GetHandle()) destroyPopup = False;

		if (destroyPopup)
		{
			UnregisterObject(trackMenu);
			DeleteObject(trackMenu);

			trackMenu = NIL;
		}
	}

	switch (message)
	{
		case WM_CLOSE:
			if (doQuit.Call()) backend->Close();

			rVal = 0;

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

			rVal = 0;

			break;
		case WM_QUIT:
			destroyed = True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return 0;
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

					if ((Math::Abs((updateRect.right - updateRect.left) - size.cx) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - size.cy) < 20) || firstPaint)	Paint(SP_DELAYED);
					else																			Paint(SP_UPDATE);

					EndPaint((HWND) backend->GetSystemWindow(), &ps);
				}

				firstPaint = False;
			}

			rVal = 0;

			break;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndpos = (LPWINDOWPOS) lParam;
				Bool		 resized = (size.cx != wndpos->cx || size.cy != wndpos->cy);

				pos	= Point(wndpos->x, wndpos->y);
				size	= Size(wndpos->cx, wndpos->cy);

				GetDrawSurface()->SetSize(size);

				if (resized)
				{
					updateRect = Rect(Point(0, 0), size);

					CalculateOffsets();

					onResize.Emit();
				}
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

						rVal = 0;

						break;
					}
				}
			}

			break;
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
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

			rVal = 0;

			break;
		case SM_RBUTTONDOWN:
			{
				Menu	*track = getTrackMenu.Call(MouseX(), MouseY());

				if (track != NIL)
				{
					trackMenu = new PopupMenu(track);

					trackMenu->pos.x = MouseX();
					trackMenu->pos.y = MouseY();

					trackMenu->onClick.Connect(&Window::PopupProc, this);

					RegisterObject(trackMenu);

					rVal = 0;
				}
			}

			break;
	}
#endif

	if (rVal == -1)
	{
		for (Int i = GetNOfObjects() - 1; i >= 0; i--)
		{
			Widget	*object = assocObjects.GetNthEntry(i);

			if (object == NIL) continue;

			if (object->Process(message, wParam, lParam) == Break)
			{
				rVal = 0;

				break;
			}
		}
	}

	LeaveProtectedRegion();

	return rVal;
}

S::Int S::GUI::Window::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!created)		return Success;
	if (!visible)		return Success;

	if ((updateRect.right - updateRect.left == 0) || (updateRect.bottom - updateRect.top == 0)) return Success;

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (firstPaint || (updateRect.left < 2))		updateRect.left		= 2;
	if (firstPaint || (updateRect.top < 2))			updateRect.top		= 2;
	if (firstPaint || (size.cx - updateRect.right < 2))	updateRect.right	= size.cx - 2;
	if (firstPaint || (size.cy - updateRect.bottom < 2))	updateRect.bottom	= size.cy - 2;

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		surface->StartPaint(updateRect);
		surface->Box(updateRect, Setup::BackgroundColor, FILLED);

		Widget	*lastWidget = NIL;
		Point	 doublebar1;
		Point	 doublebar2;
		Int	 bias = 0;
		Int	 topoffset = 3;
		Int	 rightobjcount = 0;
		Int	 leftobjcount = 0;
		Int	 btmobjcount = 0;
		Int	 topobjcount = 0;

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			Widget	*object = assocObjects.GetNthEntry(i);

			if (object->GetOrientation() == OR_TOP)
			{
				topobjcount++;

				lastWidget = object;

				if (object->subtype == WO_SEPARATOR)
				{
					bias = -3;

					topoffset += object->size.cy + 3;

					doublebar1.x = 4;
					doublebar1.y = topoffset - 2;
					doublebar2.x = size.cx - 4;
					doublebar2.y = doublebar1.y;

					if (icon != NIL) doublebar1.x += 17;
					if (Setup::rightToLeft) doublebar1.x++;

					surface->Bar(doublebar1, doublebar2, OR_HORZ);
				}
				else
				{
					bias = 0;

					topoffset += object->size.cy;
				}
			}
			else if (object->GetOrientation() == OR_BOTTOM)
			{
				btmobjcount++;
			}
			else if (object->GetOrientation() == OR_LEFT)
			{
				leftobjcount++;
			}
			else if (object->GetOrientation() == OR_RIGHT)
			{
				rightobjcount++;
			}
		}

		if (topobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = innerOffset.top - 2 + bias;
			doublebar2.x = size.cx - 4;

			if (topobjcount > 0) if (lastWidget->subtype == WO_NOSEPARATOR) doublebar1.y -= 3;

			doublebar2.y = doublebar1.y;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);
		}

		if (btmobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = size.cy - innerOffset.bottom;
			doublebar2.x = size.cx - 4;
			doublebar2.y = doublebar1.y;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);
		}

		if (leftobjcount > 0)
		{
			doublebar1.x = innerOffset.left - 3;
			doublebar1.y = innerOffset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		if (rightobjcount > 0)
		{
			doublebar1.x = size.cx - innerOffset.right + 1;
			doublebar1.y = innerOffset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			Widget	*object = assocObjects.GetNthEntry(j);

			if (object == NIL) continue;

			if (object->IsVisible() && object->IsAffected(updateRect) && object->GetObjectType() != Layer::classID) object->Paint(SP_PAINT);
		}

		if ((message == SP_DELAYED) && (flags & WF_DELAYPAINT))
		{
			if (paintTimer != NIL) DeleteObject(paintTimer);

			if (timedUpdateRect.left == -1 && timedUpdateRect.top == -1 && timedUpdateRect.right == -1 && timedUpdateRect.bottom == -1)
			{
				timedUpdateRect = updateRect;
			}
			else
			{
				timedUpdateRect.left	= (Int) Math::Min(timedUpdateRect.left, updateRect.left);
				timedUpdateRect.top	= (Int) Math::Min(timedUpdateRect.top, updateRect.top);
				timedUpdateRect.right	= (Int) Math::Max(timedUpdateRect.right, updateRect.right);
				timedUpdateRect.bottom	= (Int) Math::Max(timedUpdateRect.bottom, updateRect.bottom);
			}

			paintTimer = new System::Timer();
			paintTimer->onInterval.Connect(&Window::PaintTimer, this);
			paintTimer->Start(50);
		}
		else if (paintTimer == NIL)
		{
			timedUpdateRect = updateRect;

			PaintTimer();
		}

		surface->EndPaint();
	}

	LeaveProtectedRegion();

	return Success;
}

S::Void S::GUI::Window::CalculateOffsets()
{
	if (type == ToolWindow::classID) return;

	Widget	*operat;
	Widget	*lastWidget = NIL;
	Int	 rightobjcount = 0;
	Int	 leftobjcount = 0;
	Int	 btmobjcount = 0;
	Int	 topobjcount = 0;
	Int	 i;

	innerOffset = Rect(Point(3, 3), Size(0, 0));

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetOrientation() == OR_TOP)
		{
			topobjcount++;

			lastWidget = operat;

			operat->pos.x	= innerOffset.left;
			operat->pos.y	= innerOffset.top;
			operat->size.cx	= size.cx - innerOffset.left - innerOffset.right;

			innerOffset.top += operat->size.cy;

			if (operat->subtype == WO_SEPARATOR) innerOffset.top += 3;
		}
	}

	if (topobjcount > 0)
	{
		innerOffset.top += 3;

		if (lastWidget->subtype == WO_NOSEPARATOR) innerOffset.top += 3;
	}

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetOrientation() == OR_BOTTOM)
		{
			btmobjcount++;

			operat->pos.x	= innerOffset.left;
			operat->pos.y	= size.cy - innerOffset.bottom - operat->size.cy;
			operat->size.cx	= size.cx - innerOffset.left - innerOffset.right;

			innerOffset.bottom += operat->size.cy;
		}
	}

	if (btmobjcount > 0) innerOffset.bottom += 4;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetOrientation() == OR_LEFT)
		{
			leftobjcount++;

			operat->pos.x	= innerOffset.left;
			operat->pos.y	= innerOffset.top;
			operat->size.cy	= size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.left += operat->size.cx;
		}
	}

	if (leftobjcount > 0) innerOffset.left += 3;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetOrientation() == OR_RIGHT)
		{
			rightobjcount++;

			operat->pos.x	= size.cx - innerOffset.right - operat->size.cx;
			operat->pos.y	= innerOffset.top;
			operat->size.cy	= size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.right += operat->size.cx;
		}
	}

	if (rightobjcount > 0) innerOffset.right += 3;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetOrientation() == OR_CENTER)
		{
			operat->pos.x	= innerOffset.left;
			operat->pos.y	= innerOffset.top;
			operat->size.cx	= size.cx - innerOffset.left - innerOffset.right;
			operat->size.cy	= size.cy - innerOffset.top - innerOffset.bottom;
		}
	}
}

S::Int S::GUI::Window::MouseX()
{
	if (Setup::rightToLeft)	return size.cx - (Input::MouseX() - pos.x) - 1;
	else			return Input::MouseX() - pos.x;
}

S::Int S::GUI::Window::MouseY()
{
	return Input::MouseY() - pos.y;
}

S::Bool S::GUI::Window::IsMouseOn(Rect rect)
{
	Surface	*surface = GetDrawSurface();

	if (surface->GetSystemSurface() == NIL) return False;

#ifdef __WIN32__
	if (!PtVisible((HDC) surface->GetSystemSurface(), Input::MouseX() - pos.x, Input::MouseY() - pos.y)) return False;
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

S::Void S::GUI::Window::PopupProc()
{
	if (trackMenu != NIL)
	{
		trackMenu->Hide();

		DeleteObject(trackMenu);

		trackMenu = NIL;
	}
}

S::Int S::GUI::Window::RegisterObject(Widget *object)
{
	if (object == NIL) return Failure;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->GetHandle());

			object->SetContainer(this);
			object->SetRegisteredFlag(True);

			if (object->GetObjectType() == Titlebar::classID)
			{
				if (!Binary::IsFlagSet(object->GetFlags(), TB_MAXBUTTON)) flags = flags | WF_NORESIZE;
			}
			else if (object->GetObjectType() == Statusbar::classID)
			{
				SetDefaultStatusText(object->GetText());
			}
			else if (object->GetObjectType() == ToolWindow::classID)
			{
				if (Setup::rightToLeft)	object->pos.x = size.cx - ((object->pos.x - pos.x) + object->size.cx) + pos.x;
				((Window *) object)->Create();
			}

			CalculateOffsets();

			object->onRegister.Emit(this);
			object->Show();

			return Success;
		}
	}
	else
	{
		return mainLayer->RegisterObject(object);
	}

	return Failure;
}

S::Int S::GUI::Window::UnregisterObject(Widget *object)
{
	if (object == NIL) return Failure;

	if (containerType == &object->possibleContainers)
	{
		if (GetNOfObjects() > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->GetHandle()) == True)
			{
				object->onUnregister.Emit(this);
				object->Hide();

				object->SetRegisteredFlag(False);
				object->SetContainer(NIL);

				CalculateOffsets();

				return Success;
			}
		}
	}
	else
	{
		return mainLayer->UnregisterObject(object);
	}

	return Failure;
}

S::Void S::GUI::Window::PaintTimer()
{
	if (paintTimer != NIL)
	{
		DeleteObject(paintTimer);

		paintTimer = NIL;
	}

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(timedUpdateRect);

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		Widget	*object = assocObjects.GetNthEntry(j);

		if (object == NIL) continue;

		if (object->IsVisible() && object->IsAffected(timedUpdateRect) && object->GetObjectType() == Layer::classID) object->Paint(SP_PAINT);
	}

	onPaint.Emit();

	surface->EndPaint();

	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	LeaveProtectedRegion();
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
