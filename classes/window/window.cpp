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

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_WINDOW = S::Object::RequestObjectID();
S::Int	 S::Window::nOfActiveWindows = 0;

#ifdef __WIN32__
LRESULT CALLBACK S::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window	*smoothWindow = SMOOTH::GetWindow(window);
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

	if (retVal != -1)			return retVal;
	else if (SMOOTH::Setup::enableUnicode)	return DefWindowProcW(window, originalMessage, wParam, lParam);
	else					return DefWindowProcA(window, originalMessage, wParam, lParam);
}
#endif

S::Window::Window(String title)
{
	self = this;

	containerType = OBJ_WINDOW;

	possibleContainers.AddEntry(OBJ_APPLICATION);

#ifdef __WIN32__
	style		= WS_THICKFRAME|WS_SYSMENU|WS_POPUP;
#endif

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

	className = String(title).Append(String::IntToString(System::RequestGUID()));

	value = 0;

	offset = Rect(Point(3, 3), Size(0, 0));

	icon = NIL;

#ifdef __WIN32__
	sysicon = LoadIconA(NIL, MAKEINTRESOURCEA(32512));
#endif

	created		= False;
	visible		= False;
	destroyed	= False;
	cursorset	= False;
	initshow	= False;

	objectProperties->size.cx = Math::Round(200 * SMOOTH::Setup::FontSize);
	objectProperties->size.cy = Math::Round(200 * SMOOTH::Setup::FontSize);

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

	mainLayer = new Layer();

	RegisterObject(mainLayer);

	minSize.cx = Math::Round(160 * SMOOTH::Setup::FontSize);
	minSize.cy = METRIC_TITLEBARHEIGHT + 5;

	hwnd = NIL;
}

S::Window::~Window()
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

S::Int S::Window::SetMetrics(Point newPos, Size newSize)
{
	objectProperties->pos.x		= Math::Round(newPos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y		= Math::Round(newPos.y * SMOOTH::Setup::FontSize);
	objectProperties->size.cx	= Math::Round(newSize.cx * SMOOTH::Setup::FontSize);
	objectProperties->size.cy	= Math::Round(newSize.cy * SMOOTH::Setup::FontSize);

	updateRect.left		= 0;
	updateRect.top		= 0;
	updateRect.right	= updateRect.left + objectProperties->size.cx;
	updateRect.bottom	= updateRect.top + objectProperties->size.cy;

#ifdef __WIN32__
	if (created) SetWindowPos(hwnd, 0, objectProperties->pos.x, objectProperties->pos.x, objectProperties->size.cx, objectProperties->size.cy, 0);
#endif

	return Success;
}

S::Void S::Window::SetPositionFlag(HWND pf)
{
#ifdef __WIN32__
	SetWindowPos(hwnd, pf, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
#endif
}

S::Void S::Window::SetKillProc(KillProcParam, Void *procParam)
{
	killProc = (KillProcType) newProc;
	killProcParam = procParam;
}

S::Void S::Window::SetPaintProc(ProcParam, Void *procParam)
{
	paintProc = (ProcType) newProc;
	paintProcParam = procParam;
}

S::Void S::Window::SetPeekProc(ProcParam, Void *procParam)
{
	peekProc = (ProcType) newProc;
	peekProcParam = procParam;

	if (peekProc != NIL) peekLoop++;
}

S::Void S::Window::SetMessageProc(MessageProcParam, Void *procParam)
{
	messageProc = (MessageProcType) newProc;
	messageProcParam = procParam;
}

S::Void S::Window::SetStyle(Int s)
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

#ifdef __WIN32__
			if (!created) exstyle = exstyle | WS_EX_TOPMOST;
#endif

			break;
		case SS_NORESIZE:
#ifdef __WIN32__
			style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;
#endif

			break;
		default:
			if (!created) style = style | s;
			break;
	}
}

S::Void S::Window::SetExStyle(Int es)
{
	if (!created) exstyle = exstyle | es;
}

S::Int S::Window::SetIcon(HBITMAP newicon)
{
#ifdef __WIN32__
	if (newicon == SI_DEFAULT) newicon = DEFAULTICON;
#endif

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

S::Int S::Window::SetApplicationIcon(HICON newicon)
{
#ifdef __WIN32__
	sysicon = newicon;

	return Success;
#else
	return Error;
#endif
}

S::Int S::Window::SetApplicationIcon(Int newicon)
{
#ifdef __WIN32__
	HICON	 ic = LoadIconA(hInstance, MAKEINTRESOURCEA(newicon));

	sysicon = ic;
#endif

	return Success;
}

S::Int S::Window::SetText(String title)
{
	objectProperties->text = title;

	if (!created) return Success;

#ifdef __WIN32__
	if (SMOOTH::Setup::enableUnicode)	SetWindowTextW(hwnd, title);
	else					SetWindowTextA(hwnd, title);
#endif

	SMOOTH::SendMessage(this, SM_WINDOWTITLECHANGED, 0, 0);

	return Success;
}

S::Int S::Window::SetStatusText(String newStatus)
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			object->SetText(newStatus);

			return Success;
		}
	}

	return Error;
}

S::String S::Window::GetStatusText()
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		Object *object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == OBJ_STATUSBAR)
		{
			return object->GetText();
		}
	}

	return NIL;
}

S::Int S::Window::Show()
{
	if (!created) Create();

	initshow	= True;
	visible		= True;

#ifdef __WIN32__
	ShowWindow(hwnd, SW_SHOW);
#endif

	if (apptopmost || modal || sysmodal || type == OBJ_TOOLWINDOW)
	{
#ifdef __WIN32__
		SetWindowPos(hwnd, HWND_TOPMOST, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, SWP_NOACTIVATE|SWP_SHOWWINDOW);
#endif
	}

	return Success;
}

S::Int S::Window::Hide()
{
	if (!created) Create();

	initshow = True;

	visible = False;

#ifdef __WIN32__
	ShowWindow(hwnd, SW_HIDE);
#endif

	return Success;
}

S::Bool S::Window::IsMaximized()
{
	if (!created) return False;

	return maximized;
}

S::Rect S::Window::GetUpdateRect()
{
	return updateRect;
}

S::Int S::Window::SetUpdateRect(Rect newUpdateRect)
{
	updateRect = newUpdateRect;

	return Success;
}

S::Int S::Window::SetMinimumSize(Size newMinSize)
{
	minSize = newMinSize;

	return Success;
}

HWND S::Window::Create()
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

			return hwnd;
		}
		else
		{
			return NIL;
		}
	}

	return NIL;
}

S::Int S::Window::Stay()
{
	if (!registered) return value;

	SetStyle(SS_APPTOPMOST);

#ifdef __WIN32__
	MSG	 msg;
#endif

	if (!created)	Create();
	if (!visible)	Show();

	modal	= True;
	stay	= True;

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

S::Int S::Window::Close()
{
	if (hwnd == NIL) return Error;

	if (SMOOTH::Setup::enableUnicode)	::SendMessageW(hwnd, WM_CLOSE, 0, 0);
	else					::SendMessageA(hwnd, WM_CLOSE, 0, 0);

	return Success;
}

S::Int S::Window::Process(Int message, Int wParam, Int lParam)
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
	Menubar	*menubar = NIL;
	int	 i;


#ifdef __WIN32__
	PAINTSTRUCT	 ps;
	WINDOWPOS	*wndpos;
#endif

	Object	*operat;

	if (!(message == SM_MOUSEMOVE && wParam == 1)) MessageProcCall(messageProc, messageProcParam, message, wParam, lParam);

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

				if (SMOOTH::GetWindow(activeWindow) != NIL)
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
#ifdef __WIN32__
		case WM_NCDESTROY:
			LeaveProtectedRegion();

			return 0;
		case SM_EXECUTEPEEK:
			ProcCall(peekProc, peekProcParam);

			LeaveProtectedRegion();

			return 0;
		case WM_CLOSE:
			if (KillProcCall(killProc, killProcParam))
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
			destroyed = True;

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
					menubar = (Menubar *) operat;

					break;
				}
			}

			if (menubar != NIL)
			{
				PopupMenu	*popup = NIL;

				Process(SM_LOOSEFOCUS, handle, 0);

				if (popupMenu != NIL)
				{
					PopupMenu	*popup = popupMenu;

					UnregisterObject(popup);

					DeleteObject(popup);
				}

				popup = new PopupMenu();

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
			if ((LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) && popupMenu == NIL && PopupMenu::status != POPUP_PENDING) SMOOTH::SendMessage(NIL, SM_CHECKPOPUPS, 0, 0);
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
				else if (act != hwnd && SMOOTH::GetWindow(act)->sysmodal == False && SMOOTH::GetWindow(act)->modal == False) SetForegroundWindow(hwnd);
				else if (act != hwnd && SMOOTH::GetWindow(act)->handle < handle) SetForegroundWindow(hwnd);
			}
			else if (modal)
			{
				act = GetActiveWindow();

				if (SMOOTH::GetWindow(act) == 0) break;
				else if (SMOOTH::GetWindow(act)->type == OBJ_TOOLWINDOW) break;

				if (act != hwnd && SMOOTH::GetWindow(act)->modal == False && SMOOTH::GetWindow(act)->sysmodal == False) SetActiveWindow(hwnd);
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

		if (operat->Process(message, wParam, lParam) == Break)
		{
			LeaveProtectedRegion();

			return 0;
		}
	}

	LeaveProtectedRegion();

	return -1;
}

S::Int S::Window::Paint(Int message)
{
	EnterProtectedRegion();

	Object	*operat;
	Object	*lastoperat = NIL;
	HDC	 dc;
	Point	 doublebar1;
	Point	 doublebar2;
	int	 bias = 0;
	int	 topoffset = 3;
	int	 rightobjcount = 0;
	int	 leftobjcount = 0;
	int	 btmobjcount = 0;
	int	 topobjcount = 0;

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

		ProcCall(paintProc, paintProcParam);

		FreeContext(this, dc);
	}

	LeaveProtectedRegion();

	return Success;
}

S::Void S::Window::CalculateOffsets()
{
	if (type == OBJ_TOOLWINDOW) return;

	Object	*operat;
	Object	*lastoperat = NIL;
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

S::Int S::Window::RegisterObject(Object *object)
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
#ifdef __WIN32__
				if (!((Titlebar *) object)->max)	style = (style ^ WS_THICKFRAME) | WS_DLGFRAME;
#endif
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

			object->Show();

			return Success;
		}
	}
	else
	{
		return mainLayer->RegisterObject(object);
	}

	return Error;
}

S::Int S::Window::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.DeleteEntry(object->handle) == True)
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
