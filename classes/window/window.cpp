 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/titlebar.h>
#include <smooth/statusbar.h>
#include <smooth/popupmenu.h>
#include <smooth/divider.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/toolwindow.h>
#include <smooth/timer.h>
#include <smooth/stk.h>
#include <smooth/surfacegdi.h>
#include <smooth/objectproperties.h>
#include <smooth/menubar.h>
#include <smooth/system.h>
#include <smooth/mdiwindow.h>
#include <smooth/input.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_WINDOW = S::Object::RequestObjectID();
S::Int	 S::GUI::Window::nOfActiveWindows = 0;

LRESULT CALLBACK S::GUI::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window	*smoothWindow = Window::GetWindow(window);
	Int	 retVal;
	Int	 originalMessage = message;
	Int	 param1 = wParam;
	Int	 param2 = lParam;

	switch (message)
	{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			message = SM_MOUSEMOVE;
			param1 = 0;
			param2 = 0;
			break;
		case WM_MOUSEWHEEL:
			message = SM_MOUSEWHEEL;
			param1 = (short) HIWORD(wParam);
			param2 = 0;
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			message = SM_LBUTTONDOWN;
			param1 = 0;
			param2 = 0;
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			message = SM_LBUTTONUP;
			param1 = 0;
			param2 = 0;
			break;
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			message = SM_RBUTTONDOWN;
			param1 = 0;
			param2 = 0;
			break;
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			message = SM_RBUTTONUP;
			param1 = 0;
			param2 = 0;
			break;
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
			message = SM_LBUTTONDBLCLK;
			param1 = 0;
			param2 = 0;
			break;
		case WM_RBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
			message = SM_RBUTTONDBLCLK;
			param1 = 0;
			param2 = 0;
			break;
		case WM_TIMER:
			message = SM_TIMER;
			param1 = wParam;
			param2 = 0;
			break;
	}

	if (smoothWindow != NIL)	retVal = smoothWindow->Process(message, param1, param2);
	else				retVal = -1;

	if (retVal != -1)		return retVal;
	else if (Setup::enableUnicode)	return DefWindowProcW(window, originalMessage, wParam, lParam);
	else				return DefWindowProcA(window, originalMessage, wParam, lParam);
}

S::GUI::Window::Window(String title)
{
	self = this;

	containerType = OBJ_WINDOW;

	possibleContainers.AddEntry(OBJ_APPLICATION);

	parentWindow = NIL;

	style		= WS_THICKFRAME | WS_SYSMENU | WS_POPUP;

	exstyle		= 0;
	modal		= False;
	sysmodal	= False;
	stay		= False;
	apptopmost	= False;
	maximized	= False;

	nOfActiveWindows++;

	type = OBJ_WINDOW;

	if (title != NIL)	objectProperties->text = title;
	else			objectProperties->text = TXT_SMOOTHAPPLICATION;

	className = String(title).Append(String::FromInt(System::RequestGUID()));

	value = 0;

	offset = Rect(Point(3, 3), Size(0, 0));

	icon = NIL;

	sysicon = LoadIconA(NIL, MAKEINTRESOURCEA(32512));

	created		= False;
	visible		= False;
	destroyed	= False;
	initshow	= False;

	objectProperties->size.cx = Math::Round(200 * Setup::FontSize);
	objectProperties->size.cy = Math::Round(200 * Setup::FontSize);

	updateRect.left		= 0;
	updateRect.top		= 0;
	updateRect.right	= updateRect.left + objectProperties->size.cx;
	updateRect.bottom	= updateRect.top + objectProperties->size.cy;

	popupMenu = NIL;

	mainLayer = new Layer();

	RegisterObject(mainLayer);

	minSize.cx = Math::Round(160 * Setup::FontSize);
	minSize.cy = METRIC_TITLEBARHEIGHT + 5;

	maxSize.cx = 32768;
	maxSize.cy = 32768;

	hwnd = NIL;

	doQuit.Connect(&Window::DummyExitProc, this);
}

S::GUI::Window::~Window()
{
	UnregisterObject(mainLayer);
	DeleteObject(mainLayer);

	if (popupMenu != NIL)
	{
		PopupMenu	*popup = popupMenu;

		UnregisterObject(popup);

		DeleteObject(popup);
	}

	if (created && !destroyed)
	{
		delete drawSurface;

		drawSurface = nullSurface;

		FreeContext(this, windowDC);

		DestroyWindow(hwnd);
	}

	if (created)
	{
		if (Setup::enableUnicode)	UnregisterClassW(className, hInstance);
		else				UnregisterClassA(className, hInstance);
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop--;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Window::SetParentWindow(Window *pWnd)
{
	parentWindow = pWnd;

	return Success;
}

S::Bool S::GUI::Window::DummyExitProc()
{
	return True;
}

S::Int S::GUI::Window::SetMetrics(Point newPos, Size newSize)
{
	objectProperties->pos.x		= Math::Round(newPos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(newPos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(newSize.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(newSize.cy * Setup::FontSize);

	updateRect.left		= 0;
	updateRect.top		= 0;
	updateRect.right	= updateRect.left + objectProperties->size.cx;
	updateRect.bottom	= updateRect.top + objectProperties->size.cy;

	if (created) SetWindowPos(hwnd, 0, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, 0);

	return Success;
}

S::Void S::GUI::Window::SetStyle(Int s)
{
	switch (s)
	{
		case SS_MODAL:
			modal		= True;
			sysmodal	= False;
			break;
		case SS_SYSMODAL:
			modal		= False;
			sysmodal	= True;
			break;
		case SS_APPTOPMOST:
			apptopmost	= True;

			if (!created) exstyle = exstyle | WS_EX_TOPMOST;

			break;
		case SS_NORESIZE:
			style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;

			break;
		default:
			if (!created) style = style | s;
			break;
	}
}

S::Void S::GUI::Window::SetExStyle(Int es)
{
	if (!created) exstyle = exstyle | es;
}

S::Int S::GUI::Window::SetIcon(HBITMAP newicon)
{
	if (newicon == SI_DEFAULT) newicon = DEFAULTICON;

	if (newicon != NIL)
	{
		if (GetBitmapSizeX(newicon) != 20 || GetBitmapSizeY(newicon) != 20)
		{
			return Error;
		}
		else
		{
			newicon = DetectTransparentRegions(newicon);

			icon = newicon;

			return Success;
		}
	}
	else
	{
		return Success;
	}
}

S::Int S::GUI::Window::SetApplicationIcon(HICON newicon)
{
	sysicon = newicon;

	return Success;
}

S::Int S::GUI::Window::SetApplicationIcon(Int newicon)
{
	HICON	 ic = LoadIconA(hInstance, MAKEINTRESOURCEA(newicon));

	sysicon = ic;

	return Success;
}

S::Int S::GUI::Window::SetText(String title)
{
	objectProperties->text = title;

	if (!created) return Success;

	if (Setup::enableUnicode)	SetWindowTextW(hwnd, title);
	else				SetWindowTextA(hwnd, title);

	SMOOTH::SendMessage(this, SM_WINDOWTITLECHANGED, 0, 0);

	return Success;
}

S::Int S::GUI::Window::SetStatusText(String newStatus)
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			((Statusbar *) object)->SetText(newStatus);

			return Success;
		}
	}

	return Error;
}

S::String S::GUI::Window::GetStatusText()
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			return ((Statusbar *) object)->GetText();
		}
	}

	return NIL;
}

S::Int S::GUI::Window::Show()
{
	if (!created) Create();

	ShowWindow(hwnd, SW_SHOW);

	if (apptopmost || modal || sysmodal || type == OBJ_TOOLWINDOW)
	{
		SetWindowPos(hwnd, HWND_TOPMOST, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	if (maximized && !initshow)
	{
		maximized = False;
		Maximize();
	}

	initshow	= True;
	visible		= True;

	return Success;
}

S::Int S::GUI::Window::Hide()
{
	if (!created) Create();

	ShowWindow(hwnd, SW_HIDE);

	if (maximized && !initshow)
	{
		maximized = False;
		Maximize();
	}

	initshow	= True;
	visible		= False;

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

	Rect	 workArea;

	{
		RECT rect;

		if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
		else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

		workArea = rect;

		::GetWindowRect(hwnd, &rect);

		nonmaxrect = rect;
	}

	SetWindowPos(hwnd, 0, workArea.left - 2, workArea.top - 2, workArea.right - workArea.left + 4, workArea.bottom - workArea.top + 4, 0);

	maximized = True;

	if (Setup::enableUnicode)	origwndstyle = GetWindowLongW(hwnd, GWL_STYLE);
	else				origwndstyle = GetWindowLongA(hwnd, GWL_STYLE);

	if (Setup::enableUnicode)	SetWindowLongW(hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);
	else				SetWindowLongA(hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);

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

	SetWindowPos(hwnd, 0, nonmaxrect.left, nonmaxrect.top, nonmaxrect.right - nonmaxrect.left, nonmaxrect.bottom - nonmaxrect.top, 0);

	maximized = False;

	if (Setup::enableUnicode)	SetWindowLongW(hwnd, GWL_STYLE, origwndstyle);
	else				SetWindowLongA(hwnd, GWL_STYLE, origwndstyle);

	return Success;
}

S::Bool S::GUI::Window::IsMaximized()
{
	if (!created) return False;

	return maximized;
}

S::Rect S::GUI::Window::GetWindowRect()
{
	if (maximized)	return nonmaxrect;
	else		return Rect(objectProperties->pos, objectProperties->size);
}

S::Rect S::GUI::Window::GetUpdateRect()
{
	return updateRect;
}

S::Int S::GUI::Window::SetUpdateRect(Rect newUpdateRect)
{
	updateRect = newUpdateRect;

	return Success;
}

S::Int S::GUI::Window::SetMinimumSize(Size newMinSize)
{
	minSize = newMinSize;

	return Success;
}

S::Int S::GUI::Window::SetMaximumSize(Size newMaxSize)
{
	maxSize = newMaxSize;

	return Success;
}

HWND S::GUI::Window::Create()
{
	if (registered)
	{
		hwnd = CreateSimpleWindow(Rect(objectProperties->pos, objectProperties->size), objectProperties->text, className, sysicon, style, exstyle);

		if (hwnd != NIL)
		{
			created = True;
			visible = False;

			CalculateOffsets();

			windowDC = GetContext(this);

			drawSurface = new SurfaceGDI(windowDC);

			onCreate.Emit();

			return hwnd;
		}
		else
		{
			return NIL;
		}
	}

	if (onPeek.GetNOfConnectedSlots() > 0) peekLoop++;

	return NIL;
}

S::Int S::GUI::Window::Stay()
{
	if (!registered) return value;

	MSG	 msg;

	if (!created)	Create();
	if (!visible)	Show();

	modal	= True;
	stay	= True;

	if (Setup::enableUnicode)	SendMessageW(hwnd, WM_KILLFOCUS, 0, 0);
	else				SendMessageA(hwnd, WM_KILLFOCUS, 0, 0);

	if (Setup::enableUnicode)	SendMessageW(hwnd, WM_ACTIVATEAPP, 1, 0);
	else				SendMessageA(hwnd, WM_ACTIVATEAPP, 1, 0);

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
	if (hwnd == NIL) return Error;

	if (Setup::enableUnicode)	::SendMessageW(hwnd, WM_CLOSE, 0, 0);
	else				::SendMessageA(hwnd, WM_CLOSE, 0, 0);

	return Success;
}

S::Int S::GUI::Window::Process(Int message, Int wParam, Int lParam)
{
	EnterProtectedRegion();

	if (popupMenu != NIL && PopupMenu::status == POPUP_FINISHED)
	{
		PopupMenu	*popup = popupMenu;

		UnregisterObject(popup);

		DeleteObject(popup);
	}

	HDC	 dc;
	Rect	 rect;
	HWND	 act;
	Menu	*trackMenu = NIL;
	Int	 i;

	Object	*object;

	PAINTSTRUCT	 ps;
	WINDOWPOS	*wndpos;

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

	switch (message)
	{
		case WM_ACTIVATEAPP:
			if (wParam != 0) break;
		case SM_LBUTTONDOWN:
		case SM_RBUTTONDOWN:
		case SM_CHECKPOPUPS:
			if (popupMenu != NIL)
			{
				PopupMenu	*popup = popupMenu;

				UnregisterObject(popup);

				DeleteObject(popup);
			}

			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			if (popupMenu != NIL)
			{
				Bool	 deletePopup = True;
				HWND	 activeWindow = (HWND) wParam;

				if (GetWindow(activeWindow) != NIL)
				{
					PopupMenu	*popup = popupMenu;

					do
					{
						if (activeWindow == popup->toolwnd->hwnd)
						{
							deletePopup = False;

							break;
						}
						else if (popup->nextPopup != NIL)
						{
							popup = popup->nextPopup;
						}
						else
						{
							break;
						}
					}
					while (True);
				}

				if (deletePopup)
				{
					PopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					DeleteObject(popup);
				}
			}

			break;
	}

	switch (message)
	{
		case WM_NCDESTROY:
			LeaveProtectedRegion();

			return 0;
		case SM_EXECUTEPEEK:
			onPeek.Emit();

			LeaveProtectedRegion();

			return 0;
		case WM_CLOSE:
			if (doQuit.Call())
			{
				delete drawSurface;

				drawSurface = nullSurface;

				FreeContext(this, windowDC);

				DestroyWindow(hwnd);

				hwnd = NIL;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_DESTROY:
			destroyed = True;

			if (nOfActiveWindows == 0 && loopActive)
			{
				if (Setup::enableUnicode)	SendMessageW(hwnd, WM_QUIT, 0, 0);
				else				SendMessageA(hwnd, WM_QUIT, 0, 0);
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
		case WM_SETTINGCHANGE:
			if ((wParam == SPI_SETWORKAREA) && maximized)
			{
				if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
				else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

				SetWindowPos(hwnd, 0, rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4, 0);
			}

			LeaveProtectedRegion();

			return 0;
		case WM_SYSCOLORCHANGE:
			GetColors();

			break;
		case WM_PAINT:
			if (::GetUpdateRect(hwnd, NULL, 0) == 0)
			{
				updateRect.left = 0;
				updateRect.top = 0;
				updateRect.right = objectProperties->size.cx;
				updateRect.bottom = objectProperties->size.cy;

				dc = BeginPaint(hwnd, &ps);

				Paint(SP_PAINT);

				EndPaint(hwnd, &ps);
			}
			else
			{
				RECT	 uRect = updateRect;

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right += 5;
					updateRect.bottom += 5;

					dc = BeginPaint(hwnd, &ps);

					if (Math::Abs((updateRect.right - updateRect.left) - objectProperties->size.cx) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - objectProperties->size.cy) < 20)	Paint(SP_PAINT);
					else																						Paint(SP_UPDATE);

					EndPaint(hwnd, &ps);
				}
				else
				{
					updateRect = uRect;
				}
			}

			LeaveProtectedRegion();

			return 0;
		case WM_WINDOWPOSCHANGED:
			wndpos = (LPWINDOWPOS) lParam;

			objectProperties->pos.x		= wndpos->x;
			objectProperties->pos.y		= wndpos->y;
			objectProperties->size.cx	= wndpos->cx;
			objectProperties->size.cy	= wndpos->cy;

			updateRect.left = 0;
			updateRect.top = 0;
			updateRect.right = updateRect.left + objectProperties->size.cx;
			updateRect.bottom = updateRect.top + objectProperties->size.cy;

			CalculateOffsets();

			break;
		case SM_RBUTTONDOWN:
			trackMenu = getTrackMenu.Call(MouseX(), MouseY());

			if (trackMenu != NIL)
			{
				PopupMenu	*popup = NIL;

				Process(SM_LOOSEFOCUS, handle, 0);

				if (popupMenu != NIL)
				{
					PopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					DeleteObject(popup);
				}

				popup = new PopupMenu(trackMenu);

				popup->GetObjectProperties()->pos.x = MouseX();
				popup->GetObjectProperties()->pos.y = MouseY();

				RegisterObject(popup);

				LeaveProtectedRegion();

				return 0;
			}
			break;
		case SM_MOUSEMOVE:
			dc = GetContext(this);

			if (!PtVisible(dc, MouseX(), MouseY())) message = SM_MOUSELEAVE;

			FreeContext(this, dc);

			break;
		case WM_ACTIVATE:
			if ((LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) && popupMenu == NIL && PopupMenu::status != POPUP_PENDING) SMOOTH::SendMessage(NIL, SM_CHECKPOPUPS, 0, 0);
			break;
		case WM_ACTIVATEAPP:
			if (apptopmost)
			{
				if (wParam != 0)
				{
					SetWindowPos(hwnd, HWND_TOPMOST, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE | SWP_SHOWWINDOW);
				}
				else
				{
					if (parentWindow != NIL)
					{
						SetWindowPos(hwnd, parentWindow->hwnd, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE);
						SetWindowPos(parentWindow->hwnd, hwnd, parentWindow->objectProperties->pos.x, parentWindow->objectProperties->pos.y, parentWindow->objectProperties->size.cx, parentWindow->objectProperties->size.cy, SWP_NOACTIVATE);
					}
					else
					{
						SetWindowPos(hwnd, HWND_BOTTOM, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE);
					}
				}
			}
		case WM_KILLFOCUS:
			if (sysmodal)
			{
				act = GetForegroundWindow();

				if (GetWindow(act) == NIL) SetForegroundWindow(hwnd);
				else if (GetWindow(act)->type == OBJ_TOOLWINDOW) break;
				else if (act != hwnd && GetWindow(act)->sysmodal == False && GetWindow(act)->modal == False) SetForegroundWindow(hwnd);
				else if (act != hwnd && GetWindow(act)->handle < handle) SetForegroundWindow(hwnd);
			}
			else if (modal)
			{
				act = GetActiveWindow();

				if (GetWindow(act) == NIL) break;
				else if (GetWindow(act)->type == OBJ_TOOLWINDOW) break;

				if (act != hwnd && GetWindow(act)->modal == False && GetWindow(act)->sysmodal == False) SetActiveWindow(hwnd);
				else if (act != hwnd && GetWindow(act)->handle < handle) SetActiveWindow(hwnd);
			}
			break;
		case WM_GETMINMAXINFO:
			if (style & WS_DLGFRAME)
			{
				((LPMINMAXINFO) lParam)->ptMaxSize.x = objectProperties->size.cx;
				((LPMINMAXINFO) lParam)->ptMaxSize.y = objectProperties->size.cy;
				((LPMINMAXINFO) lParam)->ptMaxPosition = objectProperties->pos;
			}
			else
			{
				((LPMINMAXINFO) lParam)->ptMinTrackSize.x = minSize.cx;
				((LPMINMAXINFO) lParam)->ptMinTrackSize.y = minSize.cy;
				((LPMINMAXINFO) lParam)->ptMaxTrackSize.x = maxSize.cx;
				((LPMINMAXINFO) lParam)->ptMaxTrackSize.y = maxSize.cy;
			}
			break;
	}

	for (i = nOfObjects - 1; i >= 0; i--)
	{
		object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_WIDGET)
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

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();
	Object	*object;

	if (updateRect.left < 2)				updateRect.left = 2;
	if (updateRect.top < 2)					updateRect.top = 2;
	if (objectProperties->size.cx - updateRect.right < 2)	updateRect.right = objectProperties->size.cx - 2;
	if (objectProperties->size.cy - updateRect.bottom < 2)	updateRect.bottom = objectProperties->size.cy - 2;

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		surface->StartPaint(updateRect);

		surface->Box(updateRect, GetSysColor(COLOR_BTNFACE), FILLED);

		onPaint.Emit();

		Widget	*lastWidget = NIL;
		Point	 doublebar1;
		Point	 doublebar2;
		int	 bias = 0;
		int	 topoffset = 3;
		int	 rightobjcount = 0;
		int	 leftobjcount = 0;
		int	 btmobjcount = 0;
		int	 topobjcount = 0;

		for (Int i = 0; i < nOfObjects; i++)
		{
			object = assocObjects.GetNthEntry(i);

			if (object->GetObjectType() == OBJ_WIDGET)
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
			doublebar1.y = offset.top - 2 + bias;
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
			doublebar1.y = objectProperties->size.cy - offset.bottom;
			doublebar2.x = objectProperties->size.cx - 4;
			doublebar2.y = doublebar1.y;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			surface->Bar(doublebar1, doublebar2, OR_HORZ);
		}

		if (leftobjcount > 0)
		{
			doublebar1.x = offset.left - 3;
			doublebar1.y = offset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - offset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		if (rightobjcount > 0)
		{
			doublebar1.x = objectProperties->size.cx - offset.right + 1;
			doublebar1.y = offset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - offset.bottom - 2;

			surface->Bar(doublebar1, doublebar2, OR_VERT);
		}

		for (int j = 0; j < nOfObjects; j++)
		{
			object = assocObjects.GetNthEntry(j);

			if (object == NIL) continue;

			if (object->GetObjectType() == OBJ_WIDGET)
			{
				if (((Widget *) object)->IsVisible() && Affected(object, updateRect)) ((Widget *) object)->Paint(SP_PAINT);
			}
		}

		surface->EndPaint();
	}

	LeaveProtectedRegion();

	return Success;
}

S::Void S::GUI::Window::CalculateOffsets()
{
	if (type == OBJ_TOOLWINDOW) return;

	Object	*operat;
	Widget	*lastWidget = NIL;
	Int	 rightobjcount = 0;
	Int	 leftobjcount = 0;
	Int	 btmobjcount = 0;
	Int	 topobjcount = 0;
	Int	 i;

	offset = Rect(Point(3, 3), Size(0, 0));

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_TOP)
		{
			topobjcount++;

			lastWidget = (Widget *) operat;

			operat->GetObjectProperties()->pos.x	= offset.left;
			operat->GetObjectProperties()->pos.y	= offset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - offset.left - offset.right;

			offset.top += operat->GetObjectProperties()->size.cy;

			if (((Widget *) operat)->subtype == WO_SEPARATOR) offset.top += 3;
		}
	}

	if (topobjcount > 0)
	{
		offset.top += 3;

		if (lastWidget->subtype == WO_NOSEPARATOR) offset.top += 3;
	}

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_BOTTOM)
		{
			btmobjcount++;

			operat->GetObjectProperties()->pos.x	= offset.left;
			operat->GetObjectProperties()->pos.y	= objectProperties->size.cy - offset.bottom - operat->GetObjectProperties()->size.cy;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - offset.left - offset.right;

			offset.bottom += operat->GetObjectProperties()->size.cy;
		}
	}

	if (btmobjcount > 0) offset.bottom += 4;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_LEFT)
		{
			leftobjcount++;

			operat->GetObjectProperties()->pos.x	= offset.left;
			operat->GetObjectProperties()->pos.y	= offset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - offset.top - offset.bottom;

			offset.left += operat->GetObjectProperties()->size.cx;
		}
	}

	if (leftobjcount > 0) offset.left += 3;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_RIGHT)
		{
			rightobjcount++;

			operat->GetObjectProperties()->pos.x	= objectProperties->size.cx - offset.right - operat->GetObjectProperties()->size.cx;
			operat->GetObjectProperties()->pos.y	= offset.top;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - offset.top - offset.bottom;

			offset.right += operat->GetObjectProperties()->size.cx;
		}
	}

	if (rightobjcount > 0) offset.right += 3;

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_CENTER)
		{
			operat->GetObjectProperties()->pos.x	= offset.left;
			operat->GetObjectProperties()->pos.y	= offset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - offset.left - offset.right;
			operat->GetObjectProperties()->size.cy	= objectProperties->size.cy - offset.top - offset.bottom;
		}
	}
}

S::Int S::GUI::Window::MouseX()
{
	return Input::MouseX() - objectProperties->pos.x;
}

S::Int S::GUI::Window::MouseY()
{
	return Input::MouseY() - objectProperties->pos.y;
}

S::Bool S::GUI::Window::IsMouseOn(Rect rect)
{
	if ((MouseX() >= rect.left) && (MouseX() <= rect.right) && (MouseY() >= rect.top) && (MouseY() <= rect.bottom))	return True;
	else														return False;
}

S::Int S::GUI::Window::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (object->GetObjectType() == OBJ_TITLEBAR)
			{
				if (!((Titlebar *) object)->max)	style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;
			}
			else if (object->GetObjectType() == OBJ_TOOLWINDOW)
			{
				((ToolWindow *) object)->Create();
			}
			else if (object->GetObjectType() == OBJ_POPUP)
			{
				if (popupMenu != NIL)
				{
					PopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					DeleteObject(popup);
				}

				popupMenu = (PopupMenu *) object;
			}

			CalculateOffsets();

			if (object->GetObjectType() == OBJ_WIDGET)
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
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->handle) == True)
			{
				nOfObjects--;

				if (object->GetObjectType() == OBJ_WIDGET)
				{
					((Widget *) object)->onUnregister.Emit(this);
					((Widget *) object)->Hide();
				}

				if (object->GetObjectType() == OBJ_POPUP)
				{
					popupMenu = NIL;
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

S::GUI::Window *S::GUI::Window::GetWindow(HWND hwnd)
{
	Window	*window;

	for (int i = 0; i < Object::objectCount; i++)
	{
		window = (Window *) mainObjectManager->RequestObject(i);

		if (window != NIL)
		{
			if (window->GetObjectType() == OBJ_WINDOW || window->GetObjectType() == OBJ_MDIWINDOW || window->GetObjectType() == OBJ_TOOLWINDOW)
			{
				if (window->hwnd == hwnd) return window;
			}
		}
	}

	return NIL;
}
