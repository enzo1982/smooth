 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/window.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/titlebar.h>
#include <smooth/statusbar.h>
#include <smooth/menu/popupmenu.h>
#include <smooth/divider.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/toolwindow.h>
#include <smooth/timer.h>
#include <smooth/color.h>
#include <smooth/objectproperties.h>
#include <smooth/menu/menubar.h>
#include <smooth/system.h>
#include <smooth/mdiwindow.h>
#include <smooth/input.h>
#include <smooth/resources.h>
#include <smooth/binary.h>
#include <smooth/graphics/surface.h>
#include <smooth/dllmain.h>

#ifdef __WIN32__
#include <smooth/graphics/gdi/windowgdi.h>
#endif

const S::Int	 S::GUI::Window::classID = S::Object::RequestClassID();
S::Int		 S::GUI::Window::nOfActiveWindows = 0;

S::GUI::Window::Window(String title, Void *iWindow)
{
#ifdef __WIN32__
	backend = new WindowGDI(iWindow);
#else
	backend = new WindowBackend(iWindow);
#endif

	backend->onEvent.Connect(&Window::Process, this);

	self = this;

	containerType = classID;

	possibleContainers.AddEntry(Application::classID);

	stay		= False;
	maximized	= False;

	nOfActiveWindows++;

	type = classID;

	if (title != NIL)	objectProperties->text = title;
	else			objectProperties->text = "smooth Application";

	value = 0;

	innerOffset = Rect(Point(3, 3), Size(0, 0));

	updateRect = Rect(Point(-1, -1), Size(0, 0));
	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	icon = NIL;

	created		= False;
	visible		= False;
	destroyed	= False;
	initshow	= False;

	objectProperties->size.cx = Math::Round(200 * Setup::FontSize);
	objectProperties->size.cy = Math::Round(200 * Setup::FontSize);

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
	if (created && !destroyed)
	{
		backend->Close();
	}

	UnregisterObject(mainLayer);
	DeleteObject(mainLayer);

	if (trackMenu != NIL)
	{
		UnregisterObject(trackMenu);
		DeleteObject(trackMenu);
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop--;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	delete backend;
}

S::Int S::GUI::Window::SetMetrics(const Point &nPos, const Size &nSize)
{
	updateRect = Rect();

	if (created) backend->SetMetrics(Point(Math::Round(nPos.x * Setup::FontSize), Math::Round(nPos.y * Setup::FontSize)), Size(Math::Round(nSize.cx * Setup::FontSize), Math::Round(nSize.cy * Setup::FontSize)));

	objectProperties->pos.x		= Math::Round(nPos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(nPos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(nSize.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(nSize.cy * Setup::FontSize);

	return Success;
}

S::GUI::Bitmap &S::GUI::Window::GetIcon()
{
	return icon;
}

S::Int S::GUI::Window::SetIcon(const Bitmap &nIcon)
{
	Bitmap	 newIcon;

	if (&nIcon == &SI_DEFAULT)	newIcon = Bitmap((HBITMAP) LoadImageA(hDllInstance, MAKEINTRESOURCEA(IDB_ICON), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_SHARED));
	else				newIcon = nIcon;

	if (newIcon != NIL)
	{
		if (newIcon.GetSize().cx != 20 || newIcon.GetSize().cy != 20)
		{
			return Error;
		}
		else
		{
			icon = newIcon;

			icon.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

			return Success;
		}
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

S::Int S::GUI::Window::SetText(String title)
{
	objectProperties->text = title;

	if (created)
	{
		Process(SM_WINDOWTITLECHANGED, 0, 0);

		backend->SetTitle(title);
	}

	return Success;
}

S::Int S::GUI::Window::SetStatusText(String newStatus)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Statusbar::classID)
		{
			((Statusbar *) object)->SetText(newStatus);

			return Success;
		}
	}

	return Error;
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

S::Int S::GUI::Window::SetDefaultStatusText(String newStatus)
{
	defaultStatus = newStatus;

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

	initshow	= True;
	visible		= False;

	onHide.Emit();

	return Success;
}

S::Int S::GUI::Window::Minimize()
{
	return backend->Minimize();
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
	if (!maximized) return Success;

	if (!created)
	{
		maximized = False;

		return Success;
	}

	backend->Restore();

	maximized = False;

	return Success;
}

S::Bool S::GUI::Window::IsMaximized()
{
	return maximized;
}

S::Rect S::GUI::Window::GetWindowRect()
{
	return Rect(objectProperties->pos, objectProperties->size);
}

S::Rect S::GUI::Window::GetClientRect()
{
	return innerOffset;
}

S::Rect S::GUI::Window::GetRestoredWindowRect()
{
	return backend->GetRestoredWindowRect();
}

S::Rect S::GUI::Window::GetUpdateRect()
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
		if (backend->Open(objectProperties->text, objectProperties->pos, objectProperties->size, flags) == Success)
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

	MSG	 msg;

	SetFlags(flags | WF_MODAL);

	if (!created)	Create();
	if (!visible)	Show();

	stay	= True;

	if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_KILLFOCUS, 0, 0);
	else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_KILLFOCUS, 0, 0);

	if (Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_ACTIVATEAPP, 1, 0);
	else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_ACTIVATEAPP, 1, 0);

	while (!destroyed)
	{
		if (peekLoop > 0)
		{
			if (Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
			else				PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

			TranslateMessage(&msg);

			if (Setup::enableUnicode)	DispatchMessageW(&msg);
			else				DispatchMessageA(&msg);

			if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
			else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
		}
		else
		{
			if (Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
			else				GetMessageA(&msg, NIL, 0, 0);

			TranslateMessage(&msg);

			if (Setup::enableUnicode)	DispatchMessageW(&msg);
			else				DispatchMessageA(&msg);
		}
	}

	if (nOfActiveWindows == 0 && !initializing) PostQuitMessage(0);

	return value;
}

S::Int S::GUI::Window::Close()
{
	Process(SM_LOOSEFOCUS, 0, 0);

	if (Setup::enableUnicode)	::PostMessageW((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);
	else				::PostMessageA((HWND) backend->GetSystemWindow(), WM_CLOSE, 0, 0);

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

	if (trackMenu != NIL && (message == SM_LBUTTONDOWN || message == SM_RBUTTONDOWN || message == WM_KILLFOCUS))
	{
		Bool	 destroyPopup = True;

		if (message == WM_KILLFOCUS && Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->handle >= trackMenu->handle) destroyPopup = False;

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
			if (doQuit.Call())
			{
				backend->Close();
			}

			LeaveProtectedRegion();

			return 0;
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

			LeaveProtectedRegion();

			return 0;
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

					if (Math::Abs((updateRect.right - updateRect.left) - objectProperties->size.cx) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - objectProperties->size.cy) < 20)	Paint(SP_DELAYED);
					else																						Paint(SP_UPDATE);

					EndPaint((HWND) backend->GetSystemWindow(), &ps);
				}
			}

			LeaveProtectedRegion();

			return 0;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndpos = (LPWINDOWPOS) lParam;
				Bool		 resized = (objectProperties->size.cx != wndpos->cx || objectProperties->size.cy != wndpos->cy);

				objectProperties->pos.x		= wndpos->x;
				objectProperties->pos.y		= wndpos->y;
				objectProperties->size.cx	= wndpos->cx;
				objectProperties->size.cy	= wndpos->cy;

				drawSurface->SetSize(objectProperties->size);

				if (resized)
				{
					updateRect.left = 0;
					updateRect.top = 0;
					updateRect.right = updateRect.left + objectProperties->size.cx;
					updateRect.bottom = updateRect.top + objectProperties->size.cy;

					CalculateOffsets();

					onResize.Emit();
				}
			}

			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE && !(flags & WF_SYSTEMMODAL))
			{
				for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
				{
					Object	*object = mainObjectManager->GetNthObject(i);

					if (object != NIL)
					{
						if (object->GetObjectType() == classID)
						{
							if (object->handle > handle && (object->GetFlags() & WF_MODAL)) SetActiveWindow((HWND) ((Window *) object)->GetSystemWindow());
						}
					}
				}
			}

			break;
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
			if (flags & WF_MODAL && (message == WM_ACTIVATEAPP || message == WM_KILLFOCUS))
			{
				Bool	 activate = False;
				HWND	 actWnd = GetActiveWindow();

				if (actWnd == (HWND) backend->GetSystemWindow()) break;

				if (GetWindow(actWnd) == NIL)					activate = False;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	break;
				else if (GetWindow(actWnd)->handle < handle)			activate = True;
				else if (GetWindow(actWnd)->handle > handle)			GetWindow(actWnd)->SetFlags(WF_MODAL);

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

			if (flags & WF_APPTOPMOST && (message == WM_ACTIVATEAPP || message == WM_KILLFOCUS))
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
				else if (GetWindow(actWnd)->handle < handle)			activate = True;
				else if (GetWindow(actWnd)->handle > handle)			GetWindow(actWnd)->SetFlags(WF_SYSTEMMODAL);

				if (activate)
				{
					SetWindowPos((HWND) backend->GetSystemWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetForegroundWindow((HWND) backend->GetSystemWindow());
				}
			}

			break;
		case SM_EXECUTEPEEK:
			onPeek.Emit();

			LeaveProtectedRegion();

			return 0;
		case SM_RBUTTONDOWN:
			{
				Menu	*track = getTrackMenu.Call(MouseX(), MouseY());

				if (track != NIL)
				{
					trackMenu = new PopupMenu(track);

					trackMenu->GetObjectProperties()->pos.x = MouseX();
					trackMenu->GetObjectProperties()->pos.y = MouseY();

					trackMenu->onClick.Connect(&Window::PopupProc, this);

					RegisterObject(trackMenu);

					LeaveProtectedRegion();

					return 0;
				}
			}

			break;
	}

	for (Int i = GetNOfObjects() - 1; i >= 0; i--)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->Process(message, wParam, lParam) == Break)
			{
				LeaveProtectedRegion();

				return 0;
			}
		}
	}

	LeaveProtectedRegion();

	return -1;
}

S::Int S::GUI::Window::Paint(Int message)
{
	if (!registered)	return Error;
	if (!created)		return Success;
	if (!visible)		return Success;

	if ((updateRect.right - updateRect.left == 0) || (updateRect.bottom - updateRect.top == 0)) return Success;

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (updateRect.left < 2)				updateRect.left		= 2;
	if (updateRect.top < 2)					updateRect.top		= 2;
	if (objectProperties->size.cx - updateRect.right < 2)	updateRect.right	= objectProperties->size.cx - 2;
	if (objectProperties->size.cy - updateRect.bottom < 2)	updateRect.bottom	= objectProperties->size.cy - 2;

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
			Object	*object = assocObjects.GetNthEntry(i);

			if (object->GetObjectType() == Widget::classID)
			{
				if (object->GetObjectProperties()->orientation == OR_TOP)
				{
					topobjcount++;

					lastWidget = (Widget *) object;

					if (((Widget *) object)->subtype == WO_SEPARATOR)
					{
						bias = -3;

						topoffset += object->GetObjectProperties()->size.cy + 3;

						doublebar1.x = 4;
						doublebar1.y = topoffset - 2;
						doublebar2.x = objectProperties->size.cx - 4;
						doublebar2.y = doublebar1.y;

						if (icon != NIL) doublebar1.x += METRIC_TITLEBARHEIGHT - 2;
						if (Setup::rightToLeft) doublebar1.x++;

						surface->Bar(doublebar1, doublebar2, OR_HORZ);
					}
					else
					{
						bias = 0;

						topoffset += object->GetObjectProperties()->size.cy;
					}
				}
				else if (object->GetObjectProperties()->orientation == OR_BOTTOM)
				{
					btmobjcount++;
				}
				else if (object->GetObjectProperties()->orientation == OR_LEFT)
				{
					leftobjcount++;
				}
				else if (object->GetObjectProperties()->orientation == OR_RIGHT)
				{
					rightobjcount++;
				}
			}
		}

		if (topobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = innerOffset.top - 2 + bias;
			doublebar2.x = objectProperties->size.cx - 4;

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
			doublebar1.y = objectProperties->size.cy - innerOffset.bottom;
			doublebar2.x = objectProperties->size.cx - 4;
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
			doublebar2.y = objectProperties->size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		if (rightobjcount > 0)
		{
			doublebar1.x = objectProperties->size.cx - innerOffset.right + 1;
			doublebar1.y = innerOffset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - innerOffset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			Object	*object = assocObjects.GetNthEntry(j);

			if (object == NIL) continue;

			if (object->GetObjectType() == Widget::classID)
			{
				if (((Widget *) object)->IsVisible() && ((Widget *) object)->IsAffected(updateRect) && object->GetObjectType() != Layer::classID) ((Widget *) object)->Paint(SP_PAINT);
			}
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

			paintTimer = new Timer();
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

	Object	*operat;
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

		if (operat->GetObjectProperties()->orientation == OR_TOP)
		{
			topobjcount++;

			lastWidget = (Widget *) operat;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;

			innerOffset.top += operat->GetObjectProperties()->size.cy;

			if (((Widget *) operat)->subtype == WO_SEPARATOR) innerOffset.top += 3;
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

		if (operat->GetObjectProperties()->orientation == OR_BOTTOM)
		{
			btmobjcount++;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= objectProperties->size.cy - innerOffset.bottom - operat->GetObjectProperties()->size.cy;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;

			innerOffset.bottom += operat->GetObjectProperties()->size.cy;
		}
	}

	if (btmobjcount > 0) innerOffset.bottom += 4;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_LEFT)
		{
			leftobjcount++;

			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.left += operat->GetObjectProperties()->size.cx;
		}
	}

	if (leftobjcount > 0) innerOffset.left += 3;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_RIGHT)
		{
			rightobjcount++;

			operat->GetObjectProperties()->pos.x	= objectProperties->size.cx - innerOffset.right - operat->GetObjectProperties()->size.cx;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;

			innerOffset.right += operat->GetObjectProperties()->size.cx;
		}
	}

	if (rightobjcount > 0) innerOffset.right += 3;

	for (i = 0; i < GetNOfObjects(); i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_CENTER)
		{
			operat->GetObjectProperties()->pos.x	= innerOffset.left;
			operat->GetObjectProperties()->pos.y	= innerOffset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - innerOffset.left - innerOffset.right;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - innerOffset.top - innerOffset.bottom;
		}
	}
}

S::Int S::GUI::Window::MouseX()
{
	if (Setup::rightToLeft)	return objectProperties->size.cx - (Input::MouseX() - objectProperties->pos.x) - 1;
	else			return Input::MouseX() - objectProperties->pos.x;
}

S::Int S::GUI::Window::MouseY()
{
	return Input::MouseY() - objectProperties->pos.y;
}

S::Bool S::GUI::Window::IsMouseOn(Rect rect)
{
	Surface	*surface = GetDrawSurface();

	if (!PtVisible((HDC) surface->GetSystemSurface(), Input::MouseX() - objectProperties->pos.x, Input::MouseY() - objectProperties->pos.y)) return False;

	if ((MouseX() >= rect.left) && (MouseX() <= rect.right) && (MouseY() >= rect.top) && (MouseY() <= rect.bottom))	return True;
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

S::Int S::GUI::Window::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (object->GetObjectType() == Titlebar::classID)
			{
				if (!Binary::IsFlagSet(object->GetFlags(), TB_MAXBUTTON)) flags = flags | WF_NORESIZE;
			}
			else if (object->GetObjectType() == Statusbar::classID)
			{
				SetDefaultStatusText(object->GetObjectProperties()->text);
			}
			else if (object->GetObjectType() == ToolWindow::classID)
			{
				if (Setup::rightToLeft)	object->GetObjectProperties()->pos.x = objectProperties->size.cx - ((object->GetObjectProperties()->pos.x - objectProperties->pos.x) + object->GetObjectProperties()->size.cx) + objectProperties->pos.x;
				((Window *) object)->Create();
			}

			CalculateOffsets();

			if (object->GetObjectType() == Widget::classID)
			{
				((Widget *) object)->onRegister.Emit(this);
				((Widget *) object)->Show();
			}

			return Success;
		}
	}
	else
	{
		return mainLayer->RegisterObject(object);
	}

	return Error;
}

S::Int S::GUI::Window::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (GetNOfObjects() > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->handle) == True)
			{
				if (object->GetObjectType() == Widget::classID)
				{
					((Widget *) object)->onUnregister.Emit(this);
					((Widget *) object)->Hide();
				}

				object->UnsetRegisteredFlag();
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

	return Error;
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
		Object	*object = assocObjects.GetNthEntry(j);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->IsVisible() && ((Widget *) object)->IsAffected(timedUpdateRect) && object->GetObjectType() == Layer::classID) ((Widget *) object)->Paint(SP_PAINT);
		}
	}

	onPaint.Emit();

	surface->EndPaint();

	timedUpdateRect = Rect(Point(-1, -1), Size(0, 0));

	LeaveProtectedRegion();
}

S::GUI::Window *S::GUI::Window::GetWindow(Void *sysWindow)
{
	if (sysWindow == NIL) return NIL;

	for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
	{
		Object	*window = mainObjectManager->GetNthObject(i);

		if (window != NIL)
		{
			if (window->GetObjectType() == Window::classID || window->GetObjectType() == MDIWindow::classID || window->GetObjectType() == ToolWindow::classID)
			{
				if (((Window *) window)->GetSystemWindow() == sysWindow) return (Window *) window;
			}
		}
	}

	return NIL;
}
