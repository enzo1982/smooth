 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_WINDOW_
#define __OBJSMOOTH_WINDOW_

#include <smooth/window.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/titlebar.h>
#include <smooth/statusbar.h>
#include <smooth/popupmenu.h>
#include <smooth/divisionbar.h>
#include <smooth/layer.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/toolwindow.h>
#include <smooth/timer.h>
#include <smooth/stk.h>
#include <smooth/surfacegdi.h>
#include <smooth/objectproperties.h>
#include <smooth/menubar.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_WINDOW = SMOOTH::RequestObjectID();
SMOOTHInt	 SMOOTHWindow::nOfActiveWindows = 0;

#ifdef __WIN32__
LRESULT CALLBACK SMOOTHWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	SMOOTHWindow	*smoothWindow = SMOOTH::GetWindow(window);
	SMOOTHInt	 retVal;
	SMOOTHInt	 originalMessage = message;
	SMOOTHInt	 param1 = wParam;
	SMOOTHInt	 param2 = lParam;

	switch (message)
	{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			message = SM_MOUSEMOVE;
			param1 = 0;
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

	if (retVal != -1)			return retVal;
	else if (SMOOTH::Setup::enableUnicode)	return DefWindowProcW(window, originalMessage, wParam, lParam);
	else					return DefWindowProcA(window, originalMessage, wParam, lParam);
}
#endif

SMOOTHWindow::SMOOTHWindow(SMOOTHString title)
{
	self = this;

	containerType = OBJ_WINDOW;

	possibleContainers.AddEntry(OBJ_APPLICATION);

#ifdef __WIN32__
	style		= WS_THICKFRAME|WS_SYSMENU|WS_POPUP;
#endif

	exstyle		= 0;
	modal		= SMOOTH::False;
	sysmodal	= SMOOTH::False;
	stay		= SMOOTH::False;
	apptopmost	= SMOOTH::False;
	maximized	= SMOOTH::False;

	nOfActiveWindows++;

	type = OBJ_WINDOW;

	if (title != NIL)	objectProperties->text = title;
	else			objectProperties->text = TXT_SMOOTHAPPLICATION;

	className = SMOOTHString(title).Append(SMOOTHString::IntToString(SMOOTH::RequestGUID()));

	value = 0;

	offset = SMOOTHRect(SMOOTHPoint(3, 3), SMOOTHSize(0, 0));

	icon = NIL;

#ifdef __WIN32__
	sysicon = LoadIconA(NIL, MAKEINTRESOURCEA(32512));
#endif

	created		= SMOOTH::False;
	visible		= SMOOTH::False;
	destroyed	= SMOOTH::False;
	cursorset	= SMOOTH::False;
	initshow	= SMOOTH::False;

	objectProperties->size.cx = roundtoint(200 * SMOOTH::Setup::FontSize);
	objectProperties->size.cy = roundtoint(200 * SMOOTH::Setup::FontSize);

	updateRect.left		= 0;
	updateRect.top		= 0;
	updateRect.right	= updateRect.left + objectProperties->size.cx;
	updateRect.bottom	= updateRect.top + objectProperties->size.cy;

	killProc		= NIL;
	killProcParam		= NIL;
	paintProc		= NIL;
	paintProcParam		= NIL;
	peekProc		= NIL;
	peekProcParam		= NIL;
	messageProc		= NIL;
	messageProcParam	= NIL;

	popupMenu = NIL;

	minSize.cx = roundtoint(160 * SMOOTH::Setup::FontSize);
	minSize.cy = METRIC_TITLEBARHEIGHT + 5;

	hwnd = NIL;
}

SMOOTHWindow::~SMOOTHWindow()
{
	if (popupMenu != NIL)
	{
		SMOOTHPopupMenu	*popup = popupMenu;

		UnregisterObject(popup);

		SMOOTH::DeleteObject(popup);
	}

	if (created && !destroyed)
	{
#ifdef __WIN32__
		DestroyWindow(hwnd);
#endif
	}

	if (created)
	{
		if (SMOOTH::Setup::enableUnicode)	UnregisterClassW(className, hInstance);
		else					UnregisterClassA(className, hInstance);
	}

	if (peekProc != NIL) peekLoop--;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHWindow::SetMetrics(SMOOTHPoint newPos, SMOOTHSize newSize)
{
	objectProperties->pos.x		= roundtoint(newPos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y		= roundtoint(newPos.y * SMOOTH::Setup::FontSize);
	objectProperties->size.cx	= roundtoint(newSize.cx * SMOOTH::Setup::FontSize);
	objectProperties->size.cy	= roundtoint(newSize.cy * SMOOTH::Setup::FontSize);

	updateRect.left		= 0;
	updateRect.top		= 0;
	updateRect.right	= updateRect.left + objectProperties->size.cx;
	updateRect.bottom	= updateRect.top + objectProperties->size.cy;

#ifdef __WIN32__
	if (created) SetWindowPos(hwnd, 0, objectProperties->pos.x, objectProperties->pos.x, objectProperties->size.cx, objectProperties->size.cy, 0);
#endif

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHWindow::SetPositionFlag(HWND pf)
{
#ifdef __WIN32__
	SetWindowPos(hwnd, pf, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
#endif
}

SMOOTHVoid SMOOTHWindow::SetKillProc(SMOOTHKillProcParam, SMOOTHVoid *procParam)
{
	killProc = (SMOOTHKillProcType) newProc;
	killProcParam = procParam;
}

SMOOTHVoid SMOOTHWindow::SetPaintProc(SMOOTHPaintProcParam, SMOOTHVoid *procParam)
{
	paintProc = (SMOOTHPaintProcType) newProc;
	paintProcParam = procParam;
}

SMOOTHVoid SMOOTHWindow::SetPeekProc(SMOOTHPeekProcParam, SMOOTHVoid *procParam)
{
	peekProc = (SMOOTHPeekProcType) newProc;
	peekProcParam = procParam;

	if (peekProc != NIL) peekLoop++;
}

SMOOTHVoid SMOOTHWindow::SetMessageProc(SMOOTHMessageProcParam, SMOOTHVoid *procParam)
{
	messageProc = (SMOOTHMessageProcType) newProc;
	messageProcParam = procParam;
}

SMOOTHVoid SMOOTHWindow::SetStyle(SMOOTHInt s)
{
	switch (s)
	{
		case SS_MODAL:
			modal = SMOOTH::True;
			sysmodal = SMOOTH::False;
			break;
		case SS_SYSMODAL:
			modal = SMOOTH::False;
			sysmodal = SMOOTH::True;
			break;
		case SS_APPTOPMOST:
			apptopmost = SMOOTH::True;

#ifdef __WIN32__
			if (!created) exstyle = exstyle|WS_EX_TOPMOST;
#endif

			break;
		case SS_NORESIZE:
#ifdef __WIN32__
			style = (style^WS_THICKFRAME)|WS_DLGFRAME;
#endif

			break;
		default:
			if (!created) style = style|s;
			break;
	}
}

SMOOTHVoid SMOOTHWindow::SetExStyle(SMOOTHInt es)
{
	if (!created) exstyle = exstyle|es;
}

SMOOTHInt SMOOTHWindow::SetIcon(HBITMAP newicon)
{
#ifdef __WIN32__
	if (newicon == SI_DEFAULT) newicon = DEFAULTICON;
#endif

	if (newicon != NIL)
	{
		if (GetBitmapSizeX(newicon) != 20 || GetBitmapSizeY(newicon) != 20)
		{
			return SMOOTH::Error;
		}
		else
		{
			newicon = DetectTransparentRegions(newicon);

			icon = newicon;

			return SMOOTH::Success;
		}
	}
	else
	{
		return SMOOTH::Success;
	}
}

SMOOTHInt SMOOTHWindow::SetApplicationIcon(HICON newicon)
{
#ifdef __WIN32__
	sysicon = newicon;

	return SMOOTH::Success;
#else
	return SMOOTH::Error;
#endif
}

SMOOTHInt SMOOTHWindow::SetApplicationIcon(SMOOTHInt newicon)
{
#ifdef __WIN32__
	HICON	 ic = LoadIconA(hInstance, MAKEINTRESOURCEA(newicon));

	sysicon = ic;
#endif

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHWindow::SetText(SMOOTHString title)
{
	objectProperties->text = title;

	if (!created) return SMOOTH::Success;

#ifdef __WIN32__
	if (SMOOTH::Setup::enableUnicode)	SetWindowTextW(hwnd, title);
	else					SetWindowTextA(hwnd, title);
#endif

	SMOOTH::SendMessage(this, SM_WINDOWTITLECHANGED, 0, 0);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHWindow::SetStatusText(SMOOTHString newStatus)
{
	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		SMOOTHObject *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			object->SetText(newStatus);

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHString SMOOTHWindow::GetStatusText()
{
	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		SMOOTHObject *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			return object->GetText();
		}
	}

	return NIL;
}

SMOOTHInt SMOOTHWindow::Show()
{
	if (!created) Create();

	initshow = SMOOTH::True;

	visible = SMOOTH::True;

#ifdef __WIN32__
	ShowWindow(hwnd, SW_SHOW);
#endif

	if (apptopmost || modal || sysmodal || type == OBJ_TOOLWINDOW)
	{
#ifdef __WIN32__
		SetWindowPos(hwnd, HWND_TOPMOST, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE|SWP_SHOWWINDOW);
#endif
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHWindow::Hide()
{
	if (!created) Create();

	initshow = SMOOTH::True;

	visible = SMOOTH::False;

#ifdef __WIN32__
	ShowWindow(hwnd, SW_HIDE);
#endif

	return SMOOTH::Success;
}

SMOOTHBool SMOOTHWindow::IsMaximized()
{
	if (!created) return SMOOTH::False;

	return maximized;
}

SMOOTHRect SMOOTHWindow::GetUpdateRect()
{
	return updateRect;
}

SMOOTHInt SMOOTHWindow::SetUpdateRect(SMOOTHRect newUpdateRect)
{
	updateRect = newUpdateRect;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHWindow::SetMinimumSize(SMOOTHSize newMinSize)
{
	minSize = newMinSize;

	return SMOOTH::Success;
}

HWND SMOOTHWindow::Create()
{
	if (registered)
	{
		hwnd = CreateSimpleWindow(SMOOTHRect(objectProperties->pos, objectProperties->size), objectProperties->text, className, sysicon, style, exstyle);

		if (hwnd != NIL)
		{
			created = SMOOTH::True;
			visible = SMOOTH::False;

			CalculateOffsets();

			windowDC = GetContext(this);

			drawSurface = new SMOOTHSurfaceGDI(windowDC);

			return hwnd;
		}
		else
		{
			return NIL;
		}
	}

	return NIL;
}

SMOOTHInt SMOOTHWindow::Stay()
{
	if (!registered) return value;

	SetStyle(SS_APPTOPMOST);

#ifdef __WIN32__
	MSG	 msg;
#endif

	if (!created)	Create();
	if (!visible)	Show();

	modal = SMOOTH::True;
	stay = SMOOTH::True;

#ifdef __WIN32__
	if (SMOOTH::Setup::enableUnicode)	SendMessageW(hwnd, WM_KILLFOCUS, 0, 0);
	else					SendMessageA(hwnd, WM_KILLFOCUS, 0, 0);

	if (SMOOTH::Setup::enableUnicode)	SendMessageW(hwnd, WM_ACTIVATEAPP, 1, 0);
	else					SendMessageA(hwnd, WM_ACTIVATEAPP, 1, 0);
#endif

	while (!destroyed)
	{
		if (peekLoop > 0)
		{
#ifdef __WIN32__
			if (SMOOTH::Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
			else					PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

			TranslateMessage(&msg);

			if (SMOOTH::Setup::enableUnicode)	DispatchMessageW(&msg);
			else					DispatchMessageA(&msg);

			if (SMOOTH::Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
			else					PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
#endif
		}
		else
		{
#ifdef __WIN32__
			if (SMOOTH::Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
			else					GetMessageA(&msg, NIL, 0, 0);

			TranslateMessage(&msg);

			if (SMOOTH::Setup::enableUnicode)	DispatchMessageW(&msg);
			else					DispatchMessageA(&msg);
#endif
		}
	}

#ifdef __WIN32__
	if (nOfActiveWindows == 0 && !initializing) PostQuitMessage(0);
#endif

	return value;
}

SMOOTHInt SMOOTHWindow::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	EnterProtectedRegion();

	if (popupMenu != NIL && SMOOTHPopupMenu::status == POPUP_FINISHED)
	{
		SMOOTHPopupMenu	*popup = popupMenu;

		UnregisterObject(popup);

		SMOOTH::DeleteObject(popup);
	}

	HDC		 dc;
	SMOOTHRect	 rect;
	HWND		 act;
	SMOOTHMenubar	*menubar = NIL;
	int		 i;


#ifdef __WIN32__
	PAINTSTRUCT	 ps;
	WINDOWPOS	*wndpos;
#endif

	SMOOTHObject	*operat;

	if (!(message == SM_MOUSEMOVE && wParam == 1)) SMOOTHMessageProcCall(messageProc, messageProcParam, message, wParam, lParam);

	switch (message)
	{
		case WM_ACTIVATEAPP:
			if (wParam != 0) break;
		case SM_LBUTTONDOWN:
		case SM_RBUTTONDOWN:
		case SM_CHECKPOPUPS:
			if (popupMenu != NIL)
			{
				SMOOTHPopupMenu	*popup = popupMenu;

				UnregisterObject(popup);

				SMOOTH::DeleteObject(popup);
			}

			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			if (popupMenu != NIL)
			{
				SMOOTHBool	 deletePopup = SMOOTH::True;
				HWND		 activeWindow = (HWND) wParam;

				if (SMOOTH::GetWindow(activeWindow) != NIL)
				{
					SMOOTHPopupMenu	*popup = popupMenu;

					do
					{
						if (activeWindow == popup->toolwnd->hwnd)
						{
							deletePopup = SMOOTH::False;

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
					while (SMOOTH::True);
				}

				if (deletePopup)
				{
					SMOOTHPopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					SMOOTH::DeleteObject(popup);
				}
			}

			break;
	}

	switch (message)
	{
#ifdef __WIN32__
		case WM_NCDESTROY:
			LeaveProtectedRegion();

			return 0;
		case SM_EXECUTEPEEK:
			SMOOTHPeekProcCall(peekProc, peekProcParam);

			LeaveProtectedRegion();

			return 0;
		case WM_CLOSE:
			if (SMOOTHKillProcCall(killProc, killProcParam))
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
			destroyed = SMOOTH::True;

			if (nOfActiveWindows == 1 && loopActive)
			{
				if (SMOOTH::Setup::enableUnicode)	SendMessageW(hwnd, WM_QUIT, 0, 0);
				else					SendMessageA(hwnd, WM_QUIT, 0, 0);
			}
			else
			{
				nOfActiveWindows--;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_QUIT:
			destroyed = SMOOTH::True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return 0;
		case WM_SETTINGCHANGE:
			if ((wParam == SPI_SETWORKAREA) && maximized)
			{
				if (SMOOTH::Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
				else					SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

				SetWindowPos(hwnd, 0, rect.left-2, rect.top-2, rect.right-rect.left+4, rect.bottom-rect.top+4, 0);
			}

			LeaveProtectedRegion();

			return 0;
		case WM_SYSCOLORCHANGE:
			SMOOTHGetColors();

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
					Paint(SP_PAINT);
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
			for (i = 0; i < nOfObjects; i++)
			{
				operat = assocObjects.GetNthEntry(i);

				if (operat->GetObjectType() == OBJ_MENUBAR)
				{
					menubar = (SMOOTHMenubar *) operat;

					break;
				}
			}

			if (menubar != NIL)
			{
				SMOOTHPopupMenu	*popup = NIL;

				Process(SM_LOOSEFOCUS, handle, 0);

				if (popupMenu != NIL)
				{
					SMOOTHPopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					SMOOTH::DeleteObject(popup);
				}

				popup = new SMOOTHPopupMenu();

				popup->GetObjectProperties()->pos.x = MouseX(hwnd, WINDOW);
				popup->GetObjectProperties()->pos.y = MouseY(hwnd, WINDOW);

				popup->MenuToPopup(menubar);

				RegisterObject(popup);

				LeaveProtectedRegion();

				return 0;
			}
			break;
		case SM_MOUSEMOVE:
			dc = GetContext(this);

			if (PtVisible(dc, MouseX(hwnd, WINDOW), MouseY(hwnd, WINDOW)) && (MouseX(hwnd, WINDOW) > 2 && MouseY(hwnd, WINDOW) > 2 && MouseX(hwnd, WINDOW) < objectProperties->size.cx - 3 && MouseY(hwnd, WINDOW) < objectProperties->size.cy - 3))
			{
				if (!cursorset) LiSASetMouseCursor(LiSA_MOUSE_ARROW);
			}
			else if (!PtVisible(dc, MouseX(hwnd, WINDOW), MouseY(hwnd, WINDOW)))
			{
				message = SM_MOUSELEAVE;
			}

			FreeContext(this, dc);

			break;
		case WM_ACTIVATE:
			if ((LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) && popupMenu == NIL && SMOOTHPopupMenu::status != POPUP_PENDING) SMOOTH::SendMessage(NIL, SM_CHECKPOPUPS, 0, 0);
			break;
		case WM_ACTIVATEAPP:
			if (apptopmost)
			{
				if (wParam != 0)	SetWindowPos(hwnd, HWND_TOPMOST, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE|SWP_SHOWWINDOW);
				else			SetWindowPos(hwnd, HWND_BOTTOM, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE);
			}
		case WM_KILLFOCUS:
			if (sysmodal)
			{
				act = GetForegroundWindow();

				if (SMOOTH::GetWindow(act) == 0) SetForegroundWindow(hwnd);
				else if (SMOOTH::GetWindow(act)->type == OBJ_TOOLWINDOW) break;
				else if (act != hwnd && SMOOTH::GetWindow(act)->sysmodal == SMOOTH::False && SMOOTH::GetWindow(act)->modal == SMOOTH::False) SetForegroundWindow(hwnd);
				else if (act != hwnd && SMOOTH::GetWindow(act)->handle < handle) SetForegroundWindow(hwnd);
			}
			else if (modal)
			{
				act = GetActiveWindow();

				if (SMOOTH::GetWindow(act) == 0) break;
				else if (SMOOTH::GetWindow(act)->type == OBJ_TOOLWINDOW) break;

				if (act != hwnd && SMOOTH::GetWindow(act)->modal == SMOOTH::False && SMOOTH::GetWindow(act)->sysmodal == SMOOTH::False) SetActiveWindow(hwnd);
				else if (act != hwnd && SMOOTH::GetWindow(act)->handle < handle) SetActiveWindow(hwnd);
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
			}
			break;
#endif
	}

	for (i = nOfObjects - 1; i >= 0; i--)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat == NIL) continue;

		if (operat->Process(message, wParam, lParam) == SMOOTH::Break)
		{
			LeaveProtectedRegion();

			return 0;
		}
	}

	LeaveProtectedRegion();

	return -1;
}

SMOOTHInt SMOOTHWindow::Paint(SMOOTHInt message)
{
	EnterProtectedRegion();

	SMOOTHObject	*operat;
	SMOOTHObject	*lastoperat = NIL;
	HDC		 dc;
	SMOOTHPoint	 doublebar1;
	SMOOTHPoint	 doublebar2;
	int		 bias = 0;
	int		 topoffset = 3;
	int		 rightobjcount = 0;
	int		 leftobjcount = 0;
	int		 btmobjcount = 0;
	int		 topobjcount = 0;

	if (created && visible)
	{
		dc = GetContext(this);

		for (int i = 0; i < nOfObjects; i++)
		{
			operat = assocObjects.GetNthEntry(i);

			if (operat->GetObjectProperties()->orientation == OR_TOP)
			{
				topobjcount++;

				lastoperat = operat;

				if (operat->subtype == WO_SEPARATOR)
				{
					bias = -3;

					topoffset += operat->GetObjectProperties()->size.cy + 3;

					doublebar1.x = 4;
					doublebar1.y = topoffset - 2;
					doublebar2.x = objectProperties->size.cx - 4;
					doublebar2.y = doublebar1.y;

					if (icon != NIL) doublebar1.x += METRIC_TITLEBARHEIGHT - 2;

					HBar(dc, doublebar1, doublebar2);
				}
				else
				{
					bias = 0;

					topoffset += operat->GetObjectProperties()->size.cy;
				}
			}
			else if (operat->GetObjectProperties()->orientation == OR_BOTTOM)
			{
				btmobjcount++;
			}
			else if (operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				leftobjcount++;
			}
			else if (operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				rightobjcount++;
			}
		}

		if (topobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = offset.top - 2 + bias;
			doublebar2.x = objectProperties->size.cx - 4;

			if (topobjcount > 0) if (lastoperat->subtype == WO_NOSEPARATOR) doublebar1.y -= 3;

			doublebar2.y = doublebar1.y;

			HBar(dc, doublebar1, doublebar2);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			HBar(dc, doublebar1, doublebar2);
		}

		if (btmobjcount > 0)
		{
			doublebar1.x = 4;
			doublebar1.y = objectProperties->size.cy - offset.bottom;
			doublebar2.x = objectProperties->size.cx - 4;
			doublebar2.y = doublebar1.y;

			HBar(dc, doublebar1, doublebar2);

			doublebar1.y = doublebar1.y + 2;
			doublebar2.y = doublebar2.y + 2;

			HBar(dc, doublebar1, doublebar2);
		}

		if (leftobjcount > 0)
		{
			doublebar1.x = offset.left - 3;
			doublebar1.y = offset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - offset.bottom - 2;

			VBar(dc, doublebar1, doublebar2);
		}

		if (rightobjcount > 0)
		{
			doublebar1.x = objectProperties->size.cx - offset.right + 1;
			doublebar1.y = offset.top;
			doublebar2.x = doublebar1.x;
			doublebar2.y = objectProperties->size.cy - offset.bottom - 2;

			VBar(dc, doublebar1, doublebar2);
		}

		for (int j = 0; j < nOfObjects; j++)
		{
			operat = assocObjects.GetNthEntry(j);

			if (operat == NIL) continue;

			if (operat->IsVisible() && Affected(operat, updateRect)) operat->Paint(SP_PAINT);
		}

		SMOOTHPaintProcCall(paintProc, paintProcParam);

		FreeContext(this, dc);
	}

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHWindow::CalculateOffsets()
{
	if (type == OBJ_TOOLWINDOW) return;

	SMOOTHObject	*operat;
	SMOOTHObject	*lastoperat = NIL;
	SMOOTHInt	 rightobjcount = 0;
	SMOOTHInt	 leftobjcount = 0;
	SMOOTHInt	 btmobjcount = 0;
	SMOOTHInt	 topobjcount = 0;
	SMOOTHInt	 i;

	offset = SMOOTHRect(SMOOTHPoint(3, 3), SMOOTHSize(0, 0));

	for (i = 0; i < nOfObjects; i++)
	{
		operat = assocObjects.GetNthEntry(i);

		if (operat->GetObjectProperties()->orientation == OR_TOP)
		{
			topobjcount++;

			lastoperat = operat;

			operat->GetObjectProperties()->pos.x	= offset.left;
			operat->GetObjectProperties()->pos.y	= offset.top;
			operat->GetObjectProperties()->size.cx	= objectProperties->size.cx - offset.left - offset.right;

			offset.top += operat->GetObjectProperties()->size.cy;

			if (operat->subtype == WO_SEPARATOR) offset.top += 3;
		}
	}

	if (topobjcount > 0)
	{
		offset.top += 3;

		if (lastoperat->subtype == WO_NOSEPARATOR) offset.top += 3;
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

SMOOTHInt SMOOTHWindow::RegisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

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
#ifdef __WIN32__
				if (!((SMOOTHTitlebar *) object)->max)	style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;
#endif
			}
			else if (object->GetObjectType() == OBJ_TOOLWINDOW)
			{
				((SMOOTHToolWindow *) object)->Create();
			}
			else if (object->GetObjectType() == OBJ_POPUP)
			{
				if (popupMenu != NIL)
				{
					SMOOTHPopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					SMOOTH::DeleteObject(popup);
				}

				popupMenu = (SMOOTHPopupMenu *) object;
			}

			CalculateOffsets();

			object->Show();

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHWindow::UnregisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.DeleteEntry(object->handle) == SMOOTH::True)
			{
				nOfObjects--;

				object->Hide();

				if (object->GetObjectType() == OBJ_POPUP)
				{
					popupMenu = NIL;
				}

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				CalculateOffsets();

				return SMOOTH::Success;
			}
		}
	}

	return SMOOTH::Error;
}

#endif
