 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/gdi/windowgdi.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
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
#include <smooth/color.h>
#include <smooth/graphics/gdi/surfacegdi.h>
#include <smooth/objectproperties.h>
#include <smooth/menubar.h>
#include <smooth/system.h>
#include <smooth/mdiwindow.h>
#include <smooth/input.h>
#include <smooth/dllmain.h>
#include <smooth/resources.h>
#include <smooth/binary.h>

LRESULT CALLBACK S::GUI::WindowGDI::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowGDI	*smoothWindow = WindowGDI::GetWindow(window);
	Int		 retVal;
	Int		 originalMessage = message;
	Int		 param1 = wParam;
	Int		 param2 = lParam;

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

S::GUI::WindowGDI::WindowGDI(String title) : WindowBase(title)
{
	style		= WS_THICKFRAME | WS_SYSMENU | WS_POPUP;
	className	= String(title).Append(String::FromInt(System::RequestGUID()));

	if (Setup::enableUnicode)	sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	else				sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	hwnd = NIL;
}

S::GUI::WindowGDI::~WindowGDI()
{
	if (created && !destroyed)
	{
		delete drawSurface;

		drawSurface = nullSurface;

		ReleaseDC(hwnd, windowDC);

		DestroyWindow(hwnd);
	}

	if (created)
	{
		if (Setup::enableUnicode)	UnregisterClassW(className, hInstance);
		else				UnregisterClassA(className, hInstance);
	}
}

S::Int S::GUI::WindowGDI::SetIcon(const Bitmap &nIcon)
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

S::Int S::GUI::WindowGDI::SetApplicationIcon(char *newIcon)
{
	return SetApplicationIcon((wchar_t *) newIcon);
}

S::Int S::GUI::WindowGDI::SetApplicationIcon(wchar_t *newIcon)
{
	if (newIcon == NIL) { sysIcon = NIL; return Success; }

	if (newIcon == MAKEINTRESOURCEW(32512) || newIcon == MAKEINTRESOURCEW(32516) || newIcon == MAKEINTRESOURCEW(32515) || newIcon == MAKEINTRESOURCEW(32513) || newIcon == MAKEINTRESOURCEW(32514) || newIcon == MAKEINTRESOURCEW(32517))
	{
		if (Setup::enableUnicode)	sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(newIcon), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		else				sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(newIcon), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	}
	else
	{
		if (Setup::enableUnicode)	sysIcon = (HICON) LoadImageW(hInstance, MAKEINTRESOURCEW(newIcon), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		else				sysIcon = (HICON) LoadImageA(hInstance, MAKEINTRESOURCEA(newIcon), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	}

	return Success;
}

S::Int S::GUI::WindowGDI::SetMetrics(Point newPos, Size newSize)
{
	WindowBase::SetMetrics(newPos, newSize);

	if (created) SetWindowPos(hwnd, 0, Math::Round(newPos.x * Setup::FontSize), Math::Round(newPos.y * Setup::FontSize), Math::Round(newSize.cx * Setup::FontSize), Math::Round(newSize.cy * Setup::FontSize), SWP_NOZORDER);

	return Success;
}

S::Int S::GUI::WindowGDI::SetText(String title)
{
	WindowBase::SetText(title);

	if (created)
	{
		if (Setup::enableUnicode)	SetWindowTextW(hwnd, title);
		else				SetWindowTextA(hwnd, title);
	}

	return Success;
}

S::Int S::GUI::WindowGDI::Show()
{
	if (!created) Create();

	if (maximized && !initshow)
	{
		ShowWindow(hwnd, SW_HIDE);

		maximized = False;
		Maximize();

		ShowWindow(hwnd, SW_SHOW);
	}
	else
	{
		ShowWindow(hwnd, SW_SHOW);
	}

	WindowBase::Show();

	return Success;
}

S::Int S::GUI::WindowGDI::Hide()
{
	if (!created) Create();

	ShowWindow(hwnd, SW_HIDE);

	if (maximized && !initshow)
	{
		maximized = False;
		Maximize();
	}

	WindowBase::Hide();

	return Success;
}

S::Int S::GUI::WindowGDI::Maximize()
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

		nonmaxrect = Rect(objectProperties->pos, objectProperties->size);
	}

	SetWindowPos(hwnd, 0, workArea.left - 2, workArea.top - 2, workArea.right - workArea.left + 4, workArea.bottom - workArea.top + 4, 0);

	if (Setup::enableUnicode)
	{
		origWndStyle = GetWindowLongW(hwnd, GWL_STYLE);

		SetWindowLongW(hwnd, GWL_STYLE, (origWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);
	}

	maximized = True;

	return Success;
}

S::Int S::GUI::WindowGDI::Restore()
{
	if (!maximized) return Success;

	if (!created)
	{
		maximized = False;

		return Success;
	}

	SetWindowPos(hwnd, 0, nonmaxrect.left, nonmaxrect.top, (Int) Math::Max(minSize.cx, nonmaxrect.right - nonmaxrect.left), (Int) Math::Max(minSize.cy, nonmaxrect.bottom - nonmaxrect.top), 0);

	maximized = False;

	if (Setup::enableUnicode) SetWindowLongW(hwnd, GWL_STYLE, origWndStyle | WS_VISIBLE);

	return Success;
}

S::Bool S::GUI::WindowGDI::Create()
{
	if (registered && !created)
	{
		if (flags & WF_NORESIZE)	style	= (style ^ WS_THICKFRAME) | WS_DLGFRAME;
		if (flags & WF_TOPMOST)		exstyle	= exstyle | WS_EX_TOPMOST;
		if (flags & WF_NOTASKBUTTON)	exstyle = exstyle | WS_EX_TOOLWINDOW;

		if (Setup::enableUnicode)
		{
			WNDCLASSEXW	 wndclassw;

			wndclassw.cbSize	= sizeof(wndclassw);
			wndclassw.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | ((exstyle & WS_EX_TOOLWINDOW) && ((unsigned int) style == (WS_BORDER | WS_POPUP)) ? CS_SAVEBITS : 0);
			wndclassw.lpfnWndProc	= WindowProc;
			wndclassw.cbClsExtra	= 0;
			wndclassw.cbWndExtra	= 0;
			wndclassw.hInstance	= hInstance;
			wndclassw.hIcon		= sysIcon;
			wndclassw.hCursor	= (HCURSOR) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
			wndclassw.hbrBackground	= NIL;
			wndclassw.lpszMenuName	= NIL;
			wndclassw.lpszClassName	= className;
			wndclassw.hIconSm	= sysIcon;

			RegisterClassExW(&wndclassw);

			hwnd = CreateWindowExW(exstyle, className, objectProperties->text, style, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, NIL, NIL, hInstance, NIL);
		}
		else
		{
			WNDCLASSEXA	 wndclassa;

			wndclassa.cbSize	= sizeof(wndclassa);
			wndclassa.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | ((exstyle & WS_EX_TOOLWINDOW) && ((unsigned int) style == (WS_BORDER | WS_POPUP)) ? CS_SAVEBITS : 0);
			wndclassa.lpfnWndProc	= WindowProc;
			wndclassa.cbClsExtra	= 0;
			wndclassa.cbWndExtra	= 0;
			wndclassa.hInstance	= hInstance;
			wndclassa.hIcon		= sysIcon;
			wndclassa.hCursor	= (HCURSOR) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
			wndclassa.hbrBackground	= NIL;
			wndclassa.lpszMenuName	= NIL;
			wndclassa.lpszClassName	= className;
			wndclassa.hIconSm	= sysIcon;

			RegisterClassExA(&wndclassa);

			hwnd = CreateWindowExA(exstyle, className, objectProperties->text, style, objectProperties->pos.x, objectProperties->pos.y, objectProperties->size.cx, objectProperties->size.cy, NIL, NIL, hInstance, NIL);
		}

		if (hwnd != NIL)
		{
			created = True;
			visible = False;

			CalculateOffsets();

			windowDC = GetWindowDC(hwnd);

			drawSurface = new SurfaceGDI(windowDC);
			drawSurface->SetSize(objectProperties->size);

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

S::Int S::GUI::WindowGDI::Stay()
{
	if (!registered) return value;

	MSG	 msg;

	SetFlags(flags | WF_MODAL);

	if (!created)	Create();
	if (!visible)	Show();

	stay	= True;

	if (Setup::enableUnicode)	::SendMessageW(hwnd, WM_KILLFOCUS, 0, 0);
	else				::SendMessageA(hwnd, WM_KILLFOCUS, 0, 0);

	if (Setup::enableUnicode)	::SendMessageW(hwnd, WM_ACTIVATEAPP, 1, 0);
	else				::SendMessageA(hwnd, WM_ACTIVATEAPP, 1, 0);

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

S::Int S::GUI::WindowGDI::Close()
{
	if (hwnd == NIL) return Error;

	Process(SM_LOOSEFOCUS, 0, 0);

	if (Setup::enableUnicode)	::PostMessageW(hwnd, WM_CLOSE, 0, 0);
	else				::PostMessageA(hwnd, WM_CLOSE, 0, 0);

	return Success;
}

S::Int S::GUI::WindowGDI::Process(Int message, Int wParam, Int lParam)
{
	if (!created) return Success;

	EnterProtectedRegion();

	if (trackMenu != NIL && (message == SM_LBUTTONDOWN || message == SM_RBUTTONDOWN || message == WM_KILLFOCUS))
	{
		Bool	 destroyPopup = True;

		if (message == WM_KILLFOCUS && WindowGDI::GetWindow((HWND) wParam) != NIL) if (WindowGDI::GetWindow((HWND) wParam)->handle >= trackMenu->handle) destroyPopup = False;

		if (destroyPopup)
		{
			UnregisterObject(trackMenu);
			DeleteObject(trackMenu);

			trackMenu = NIL;
		}
	}

	switch (message)
	{
		case WM_NCDESTROY:
			LeaveProtectedRegion();

			return 0;
		case WM_CLOSE:
			if (doQuit.Call())
			{
				SetFlags((flags | WF_MODAL) ^ WF_MODAL);
				SetFlags((flags | WF_SYSTEMMODAL) ^ WF_SYSTEMMODAL);
				SetFlags((flags | WF_TOPMOST) ^ WF_TOPMOST);
				SetFlags((flags | WF_APPTOPMOST) ^ WF_APPTOPMOST);

				delete drawSurface;

				drawSurface = nullSurface;

				ReleaseDC(hwnd, windowDC);

				DestroyWindow(hwnd);

				hwnd = NIL;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_DESTROY:
			destroyed = True;

			if (nOfActiveWindows == 0 && loopActive)
			{
				if (Setup::enableUnicode)	::SendMessageW(hwnd, WM_QUIT, 0, 0);
				else				::SendMessageA(hwnd, WM_QUIT, 0, 0);
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
				RECT	 rect;

				if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
				else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

				SetWindowPos(hwnd, 0, rect.left - 2, rect.top - 2, rect.right - rect.left + 4, rect.bottom - rect.top + 4, 0);
			}

			LeaveProtectedRegion();

			return 0;
		case WM_SYSCOLORCHANGE:
			GetColors();

			break;
		case WM_NCPAINT:
			if (Setup::enableUnicode)	PostMessageW(hwnd, WM_PAINT, 0, 0);
			else				PostMessageA(hwnd, WM_PAINT, 0, 0);

			break;
		case WM_PAINT:
			{
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = uRect;

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right += 5;
					updateRect.bottom += 5;

					PAINTSTRUCT	 ps;

					BeginPaint(hwnd, &ps);

					if (Math::Abs((updateRect.right - updateRect.left) - objectProperties->size.cx) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - objectProperties->size.cy) < 20)	Paint(SP_DELAYED);
					else																						Paint(SP_UPDATE);

					EndPaint(hwnd, &ps);
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
							if (object->handle > handle && (object->GetFlags() & WF_MODAL)) SetActiveWindow(((WindowGDI *) object)->hwnd);
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

				if (actWnd == hwnd) break;

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
					if (GetWindow(SetActiveWindow(hwnd)) != NIL)	activate = True;
					else						activate = False;
				}

				if (activate)	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				else		SetWindowPos(hwnd, message == WM_KILLFOCUS ? HWND_NOTOPMOST : GetForegroundWindow(), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			}

			if (flags & WF_APPTOPMOST && (message == WM_ACTIVATEAPP || message == WM_KILLFOCUS))
			{
				Bool	 activate = False;
				HWND	 actWnd = GetActiveWindow();

				if (actWnd == hwnd) break;

				if (GetWindow(actWnd) == NIL)					activate = False;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	break;
				else								activate = True;

				if (activate && message == WM_ACTIVATEAPP)
				{
					if (wParam)	activate = True;
					else		activate = False;
				}

				if (activate) SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				else
				{
					SetWindowPos(hwnd, GetForegroundWindow(), 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
					SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				}
			}

			if (flags & WF_SYSTEMMODAL && message == WM_KILLFOCUS)
			{
				Bool	 activate = False;
				HWND	 actWnd = GetForegroundWindow();

				if (actWnd == hwnd) break;

				if (GetWindow(actWnd) == NIL)					activate = True;
				else if (GetWindow(actWnd)->type == ToolWindow::classID)	activate = False;
				else if (GetWindow(actWnd)->handle < handle)			activate = True;
				else if (GetWindow(actWnd)->handle > handle)			GetWindow(actWnd)->SetFlags(WF_SYSTEMMODAL);

				if (activate)
				{
					SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetForegroundWindow(hwnd);
				}
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

				if (maxSize.cx > 0) ((LPMINMAXINFO) lParam)->ptMaxTrackSize.x = maxSize.cx;
				if (maxSize.cy > 0) ((LPMINMAXINFO) lParam)->ptMaxTrackSize.y = maxSize.cy;
			}
			break;
	}

	WindowBase::Process(message, wParam, lParam);

	LeaveProtectedRegion();

	return -1;
}

S::Bool S::GUI::WindowGDI::IsMouseOn(Rect rect)
{
	Surface	*surface = GetDrawSurface();

	if (surface->GetSurfaceType() != SURFACE_GDI) return False;

	if (!PtVisible(((SurfaceGDI *) surface)->GetContext(), Input::MouseX() - objectProperties->pos.x, Input::MouseY() - objectProperties->pos.y)) return False;

	if ((MouseX() >= rect.left) && (MouseX() <= rect.right) && (MouseY() >= rect.top) && (MouseY() <= rect.bottom))	return True;
	else														return False;
}

S::GUI::WindowGDI *S::GUI::WindowGDI::GetWindow(HWND hwnd)
{
	if (hwnd == NIL) return NIL;

	for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
	{
		Object	*window = mainObjectManager->GetNthObject(i);

		if (window != NIL)
		{
			if (window->GetObjectType() == WindowBase::classID || window->GetObjectType() == MDIWindow::classID || window->GetObjectType() == ToolWindow::classID)
			{
				if (((WindowGDI *) window)->hwnd == hwnd) return (WindowGDI *) window;
			}
		}
	}

	return NIL;
}
