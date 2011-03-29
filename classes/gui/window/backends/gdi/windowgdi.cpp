 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/gdi/windowgdi.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/gui/window/window.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/system/multimonitor.h>
#include <smooth/init.h>
#include <smooth/system/timer.h>
#include <smooth/input/pointer.h>

#if !defined WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL 522
#endif

#if !defined CS_DROPSHADOW
#	define CS_DROPSHADOW 0x00020000
#endif

S::GUI::WindowBackend *CreateWindowGDI()
{
	return new S::GUI::WindowGDI();
}

S::Int	 windowGDITmp = S::GUI::WindowBackend::SetBackend(&CreateWindowGDI);

S::Array<S::GUI::WindowGDI *, S::Void *>	 S::GUI::WindowGDI::windowBackends;

S::System::Timer	*S::GUI::WindowGDI::mouseNotifyTimer = NIL;

LRESULT CALLBACK S::GUI::WindowGDI::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowGDI	*smoothWindow = WindowGDI::GetWindowBackend(window);

	if (smoothWindow != NIL)
	{
		if (smoothWindow->ProcessSystemMessages(message, wParam, lParam) == Break) return 0;
	}

	if (Setup::enableUnicode) return DefWindowProcW(window, message, wParam, lParam);
	else			  return DefWindowProcA(window, message, wParam, lParam);
}

S::GUI::WindowGDI::WindowGDI(Void *iWindow)
{
	type = WINDOW_GDI;

	hwnd		= NIL;
	wndclass	= NIL;

	className	= String::FromInt(System::System::RequestGUID());

	id		= windowBackends.Add(this);

	minSize		= Size(160, 24);

	maximized	= False;

	frameSize	= Size(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

	if (Setup::enableUnicode) sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	else			  sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	destroyIcon	= False;
}

S::GUI::WindowGDI::~WindowGDI()
{
	if (destroyIcon) DestroyIcon(sysIcon);

	windowBackends.Remove(id);
}

S::Void *S::GUI::WindowGDI::GetSystemWindow() const
{
	return (Void *) hwnd;
}

S::GUI::WindowGDI *S::GUI::WindowGDI::GetWindowBackend(HWND hwnd)
{
	if (hwnd == NIL) return NIL;

	for (Int i = 0; i < windowBackends.Length(); i++)
	{
		WindowGDI	*window = windowBackends.GetNth(i);

		if (window != NIL)
		{
			if (window->hwnd == hwnd) return window;
		}
	}

	return NIL;
}

S::Key S::GUI::WindowGDI::ConvertKey(Int keySym)
{
	Key	 key = SK_OTHER;

	switch (keySym)
	{
		case VK_LEFT:	 key = SK_LEFT;	   break;
		case VK_UP:	 key = SK_UP;	   break;
		case VK_RIGHT:	 key = SK_RIGHT;   break;
		case VK_DOWN:	 key = SK_DOWN;	   break;

		case VK_HOME:	 key = SK_HOME;	   break;
		case VK_END:	 key = SK_END;	   break;
		case VK_INSERT:	 key = SK_INSERT;  break;
		case VK_DELETE:	 key = SK_DELETE;  break;
		case VK_PRIOR:	 key = SK_PRIOR;   break;
		case VK_NEXT:	 key = SK_NEXT;	   break;

		case VK_RETURN:	 key = SK_RETURN;  break;
		case VK_BACK:	 key = SK_BACK;	   break;
		case VK_TAB:	 key = SK_TAB;	   break;

		case VK_SPACE:	 key = SK_SPACE;   break;

		case VK_SHIFT:	 key = SK_SHIFT;   break;
		case VK_CONTROL: key = SK_CONTROL; break;
		case VK_MENU:	 key = SK_ALT;	   break;

		case VK_ESCAPE:	 key = SK_ESCAPE;  break;

		case VK_F1:	 key = SK_F1;	   break;
		case VK_F2:	 key = SK_F2;	   break;
		case VK_F3:	 key = SK_F3;	   break;
		case VK_F4:	 key = SK_F4;	   break;
		case VK_F5:	 key = SK_F5;	   break;
		case VK_F6:	 key = SK_F6;	   break;
		case VK_F7:	 key = SK_F7;	   break;
		case VK_F8:	 key = SK_F8;	   break;
		case VK_F9:	 key = SK_F9;	   break;
		case VK_F10:	 key = SK_F10;	   break;
		case VK_F11:	 key = SK_F11;	   break;
		case VK_F12:	 key = SK_F12;	   break;
		case VK_F13:	 key = SK_F13;	   break;
		case VK_F14:	 key = SK_F14;	   break;
		case VK_F15:	 key = SK_F15;	   break;
		case VK_F16:	 key = SK_F16;	   break;
		case VK_F17:	 key = SK_F17;	   break;
		case VK_F18:	 key = SK_F18;	   break;
		case VK_F19:	 key = SK_F19;	   break;
		case VK_F20:	 key = SK_F20;	   break;
		case VK_F21:	 key = SK_F21;	   break;
		case VK_F22:	 key = SK_F22;	   break;
		case VK_F23:	 key = SK_F23;	   break;
		case VK_F24:	 key = SK_F24;	   break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Key) keySym;
	else if	(keySym >= 'A' && keySym <= 'Z') key = (Key) keySym;

	return key;
}

S::Int S::GUI::WindowGDI::ProcessSystemMessages(Int message, Int wParam, Int lParam)
{
	/* Process system messages not relevant
	 * to portable Window implementation.
	 */
	switch (message)
	{
		case WM_CLOSE:
			if (doClose.Call()) Close();

			return Break;
		case WM_CREATE:
			onCreate.Emit();

			return Break;
		case WM_DESTROY:
			onDestroy.Emit();

			return Break;
		case WM_SIZE:
			if (!minimized && wParam == SIZE_MINIMIZED)
			{
				minimized = True;

				onMinimize.Emit();
			}
			else if (minimized && wParam == SIZE_RESTORED)
			{
				minimized = False;

				onRestore.Emit();
			}

			break;
		case WM_SETTINGCHANGE:
			if ((wParam == SPI_SETWORKAREA) && maximized)
			{
				Rect	 workArea = System::MultiMonitor::GetActiveMonitorWorkArea();

				SetMetrics(Point(workArea.left - (frameSize.cx - 2), workArea.top - (frameSize.cy - 2)), Size(workArea.right - workArea.left + (2 * frameSize.cx - 4), workArea.bottom - workArea.top + (2 * frameSize.cy - 4)));
			}

			return Break;
		case WM_GETMINMAXINFO:
			{
				RECT		 windowRect;
				Int		 windowStyle;
				LPMINMAXINFO	 minMaxInfo = (LPMINMAXINFO) lParam;

				GetWindowRect(hwnd, &windowRect);

				if (Setup::enableUnicode) windowStyle = GetWindowLongW(hwnd, GWL_STYLE);
				else			  windowStyle = GetWindowLongA(hwnd, GWL_STYLE);

				if (windowStyle & WS_DLGFRAME)
				{
					POINT	 maxSize     = { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };
					POINT	 maxPosition = { windowRect.left, windowRect.top };

					minMaxInfo->ptMaxSize	  = maxSize;
					minMaxInfo->ptMaxPosition = maxPosition;
				}
				else
				{
					minMaxInfo->ptMinTrackSize.x = minSize.cx;
					minMaxInfo->ptMinTrackSize.y = minSize.cy;

					if (maxSize.cx > 0) minMaxInfo->ptMaxTrackSize.x = maxSize.cx;
					if (maxSize.cy > 0) minMaxInfo->ptMaxTrackSize.y = maxSize.cy;
				}
			}

			return Break;
	}

	/* Update cursor position when receiving mouse messages.
	 */
	if (message == WM_MOUSEMOVE	|| message == WM_NCMOUSEMOVE	 ||
	    message == WM_LBUTTONDOWN	|| message == WM_NCLBUTTONDOWN	 ||
	    message == WM_RBUTTONDOWN	|| message == WM_NCRBUTTONDOWN	 ||
	    message == WM_LBUTTONUP	|| message == WM_NCLBUTTONUP	 ||
	    message == WM_RBUTTONUP	|| message == WM_NCRBUTTONUP	 ||
	    message == WM_LBUTTONDBLCLK	|| message == WM_NCLBUTTONDBLCLK ||
	    message == WM_RBUTTONDBLCLK	|| message == WM_NCRBUTTONDBLCLK)
	{
		POINT	 point;

		GetCursorPos(&point);

		Input::Pointer::UpdatePosition(point.x, point.y);
	}

	/* Convert Windows messages to smooth messages.
	 */
	switch (message)
	{
		/* Mouse messages:
		 */
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			return onEvent.Call(SM_MOUSEMOVE, 0, 0);
		case WM_MOUSEWHEEL:
			return onEvent.Call(SM_MOUSEWHEEL, (short) HIWORD(wParam), 0);
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			return onEvent.Call(SM_LBUTTONDOWN, 0, 0);
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			return onEvent.Call(SM_LBUTTONUP, 0, 0);
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			return onEvent.Call(SM_RBUTTONDOWN, 0, 0);
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			return onEvent.Call(SM_RBUTTONUP, 0, 0);
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
			return onEvent.Call(SM_LBUTTONDBLCLK, 0, 0);
		case WM_RBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
			return onEvent.Call(SM_RBUTTONDBLCLK, 0, 0);

		/* Keyboard messages:
		 */
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			return onEvent.Call(SM_KEYDOWN, ConvertKey(wParam), lParam);
		case WM_KEYUP:
		case WM_SYSKEYUP:
			return onEvent.Call(SM_KEYUP, ConvertKey(wParam), lParam);
		case WM_CHAR:
			return onEvent.Call(SM_CHAR, wParam, lParam);

		/* Paint messages:
		 */
		case WM_PAINT:
			{
				Int	 rVal = Break;
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = Rect();

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = Rect(Point(uRect.left, uRect.top), Size(uRect.right - uRect.left, uRect.bottom - uRect.top));
					updateRect = updateRect + frameSize;

					PAINTSTRUCT	 ps;

					BeginPaint(hwnd, &ps);

					rVal = onEvent.Call(SM_PAINT, wParam, lParam);

					EndPaint(hwnd, &ps);
				}

				return rVal;
			}

		/* Window state change messages:
		 */
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndpos = (LPWINDOWPOS) lParam;

				onEvent.Call(SM_WINDOWMETRICS, ((wndpos->x + 32768) << 16) | (wndpos->y + 32768), ((wndpos->cx + 32768) << 16) | (wndpos->cy + 32768));
			}

			return Success();
		case WM_SETFOCUS:
			return onEvent.Call(SM_GETFOCUS, 0, 0);
		case WM_KILLFOCUS:
			{
				Window	*focusWnd = Window::GetWindow((HWND) wParam);

				return onEvent.Call(SM_LOSEFOCUS, focusWnd != NIL ? focusWnd->GetHandle() : -1, 0);
			}

		/* Other messages:
		 */
		case WM_TIMER:
			return onEvent.Call(SM_TIMER, wParam, 0);
	}

	/* Call event for any other Windows messages.
	 *
	 * FIXME:
	 *
	 * Windows messages should not be propagated to smooth applications.
	 * Define a smooth message for every Windows message we need and
	 * replace the next line with return Success();.
	 */
	return onEvent.Call(message, wParam, lParam);;
}

S::Int S::GUI::WindowGDI::Open(const String &title, const Point &pos, const Size &size, Int flags)
{
	Int	 style	  = WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
	Int	 extStyle = 0;

	if (flags & WF_NORESIZE)	style	 = (style & ~WS_THICKFRAME) | WS_DLGFRAME;
	if (flags & WF_THINBORDER)	style	 = (style & ~WS_THICKFRAME & ~WS_SYSMENU);
	if (flags & WF_TOPMOST)		extStyle = extStyle | WS_EX_TOPMOST;
	if (flags & WF_NOTASKBUTTON)	extStyle = extStyle | WS_EX_TOOLWINDOW;

	if (Setup::enableUnicode)
	{
		wndclass = new WNDCLASSEXW;

		WNDCLASSEXW	*wndclassw = (WNDCLASSEXW *) wndclass;

		wndclassw->cbSize	 = sizeof(WNDCLASSEXW);
		wndclassw->style	 = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

		/* Enable shadows for tool windows.
		 */
		if ((flags & WF_NOTASKBUTTON) && (flags & WF_TOPMOST) && (flags & WF_THINBORDER)) 
		{
			OSVERSIONINFOA	 vInfo;

			vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

			GetVersionExA(&vInfo);

			if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && (vInfo.dwMajorVersion >= 6 || (vInfo.dwMajorVersion == 5 && vInfo.dwMinorVersion >= 1)))
			{
				wndclassw->style = wndclassw->style | CS_DROPSHADOW;
			}
		}

		wndclassw->lpfnWndProc	 = WindowProc;
		wndclassw->cbClsExtra	 = 0;
		wndclassw->cbWndExtra	 = 0;
		wndclassw->hInstance	 = hDllInstance;
		wndclassw->hIcon	 = sysIcon;
		wndclassw->hCursor	 = LoadCursorW(NIL, MAKEINTRESOURCEW(32512));
		wndclassw->hbrBackground = NIL;
		wndclassw->lpszMenuName	 = NIL;
		wndclassw->lpszClassName = className;
		wndclassw->hIconSm	 = sysIcon;

		RegisterClassExW(wndclassw);

		hwnd = CreateWindowExW(extStyle, className, title, style, pos.x, pos.y, size.cx, size.cy, NIL, NIL, hDllInstance, NIL);
	}
	else
	{
		wndclass = new WNDCLASSEXA;

		WNDCLASSEXA	*wndclassa = (WNDCLASSEXA *) wndclass;

		wndclassa->cbSize	 = sizeof(WNDCLASSEXA);
		wndclassa->style	 = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

		/* Enable shadows for tool windows.
		 */
		if ((flags & WF_NOTASKBUTTON) && (flags & WF_TOPMOST) && (flags & WF_THINBORDER)) 
		{
			OSVERSIONINFOA	 vInfo;

			vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

			GetVersionExA(&vInfo);

			if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && (vInfo.dwMajorVersion >= 6 || (vInfo.dwMajorVersion == 5 && vInfo.dwMinorVersion >= 1)))
			{
				wndclassa->style = wndclassa->style | CS_DROPSHADOW;
			}
		}

		wndclassa->lpfnWndProc	 = WindowProc;
		wndclassa->cbClsExtra	 = 0;
		wndclassa->cbWndExtra	 = 0;
		wndclassa->hInstance	 = hDllInstance;
		wndclassa->hIcon	 = sysIcon;
		wndclassa->hCursor	 = LoadCursorA(NIL, MAKEINTRESOURCEA(32512));
		wndclassa->hbrBackground = NIL;
		wndclassa->lpszMenuName	 = NIL;
		wndclassa->lpszClassName = className;
		wndclassa->hIconSm	 = sysIcon;

		RegisterClassExA(wndclassa);

		hwnd = CreateWindowExA(extStyle, className, title, style, pos.x, pos.y, size.cx, size.cy, NIL, NIL, hDllInstance, NIL);
	}

	if (hwnd != NIL)
	{
		/* Process the WM_CREATE message again, as GetWindowBackend
		 * cannot find the correct backend until hwnd is set.
		 */
		ProcessSystemMessages(WM_CREATE, 0, 0);

		if (flags & WF_THINBORDER || flags & WF_NORESIZE) drawSurface = new Surface(hwnd, size);
		else						  drawSurface = new Surface(hwnd);

		drawSurface->SetSize(size);

		return Success();
	}

	return Error();
}

S::Int S::GUI::WindowGDI::Close()
{
	if (drawSurface != nullSurface) delete drawSurface;

	drawSurface = nullSurface;

	DestroyWindow(hwnd);
	
	if (Setup::enableUnicode)
	{
		UnregisterClassW(className, hDllInstance);

		delete (WNDCLASSEXW *) wndclass;
	}
	else
	{
		UnregisterClassA(className, hDllInstance);

		delete (WNDCLASSEXA *) wndclass;
	}

	wndclass = NIL;

	return Success();
}

S::Int S::GUI::WindowGDI::RequestClose()
{
	if (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId())
	{
		if (Setup::enableUnicode) PostMessageW(hwnd, WM_CLOSE, 0, 0);
		else			  PostMessageA(hwnd, WM_CLOSE, 0, 0);
	}
	else
	{
		if (Setup::enableUnicode) SendMessageW(hwnd, WM_CLOSE, 0, 0);
		else			  SendMessageA(hwnd, WM_CLOSE, 0, 0);
	}

	return Success();
}

S::Int S::GUI::WindowGDI::SetTitle(const String &nTitle)
{
	if (Setup::enableUnicode) SetWindowTextW(hwnd, nTitle);
	else			  SetWindowTextA(hwnd, nTitle);

	return Success();
}

S::Int S::GUI::WindowGDI::SetIcon(const Bitmap &newIcon)
{
	if (destroyIcon) DestroyIcon(sysIcon);

	ICONINFO	 icon;
	Bitmap		 mask = newIcon;
	Int		 transparentPixel = 0;

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwMajorVersion > 4) transparentPixel = 16777215;
	else			      transparentPixel = 0;

	for (Int y = 0; y < mask.GetSize().cy; y++)
	{
		for (Int x = 0; x < mask.GetSize().cx; x++)
		{
			if (mask.GetPixel(Point(x, y)) == Setup::BackgroundColor) mask.SetPixel(Point(x, y), transparentPixel);
			else							  mask.SetPixel(Point(x, y), 0);
		}
	}

	icon.fIcon	= true;
	icon.xHotspot	= 0;
	icon.yHotspot	= 0;
	icon.hbmMask	= (HBITMAP) mask.GetSystemBitmap();
	icon.hbmColor	= (HBITMAP) newIcon.GetSystemBitmap();

	sysIcon = CreateIconIndirect(&icon);
	destroyIcon = True;

	return Success();
}

S::Int S::GUI::WindowGDI::SetIconDirect(Void *newIcon)
{
	if (destroyIcon) DestroyIcon(sysIcon);

	sysIcon = (HICON) newIcon;
	destroyIcon = False;

	return Success();
}

S::Int S::GUI::WindowGDI::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	RECT	 windowRect;

	GetWindowRect(hwnd, &windowRect);

	SetMetrics(Point(windowRect.left, windowRect.top), Size(Math::Max(windowRect.right - windowRect.left, minSize.cx), Math::Max(windowRect.bottom - windowRect.top, minSize.cy)));

	return Success();
}

S::Int S::GUI::WindowGDI::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	RECT	 windowRect;

	GetWindowRect(hwnd, &windowRect);

	SetMetrics(Point(windowRect.left, windowRect.top), Size(Math::Min(windowRect.right - windowRect.left, maxSize.cx), Math::Min(windowRect.bottom - windowRect.top, maxSize.cy)));

	return Success();
}

S::Int S::GUI::WindowGDI::Show()
{
	ShowWindow(hwnd, SW_SHOW);

	return Success();
}

S::Int S::GUI::WindowGDI::Hide()
{
	ShowWindow(hwnd, SW_HIDE);

	return Success();
}

S::GUI::Rect S::GUI::WindowGDI::GetRestoredWindowRect() const
{
	return nonMaxRect;
}

S::Int S::GUI::WindowGDI::SetMetrics(const Point &nPos, const Size &nSize)
{
	SetWindowPos(hwnd, 0, nPos.x, nPos.y, nSize.cx, nSize.cy, SWP_NOZORDER);

	drawSurface->SetSize(nSize);

	return Success();
}

S::Int S::GUI::WindowGDI::Minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);

	return Success();
}

S::Int S::GUI::WindowGDI::Maximize()
{
	Rect	 workArea = System::MultiMonitor::GetActiveMonitorWorkArea();

	{
		RECT	 rect;

		GetWindowRect(hwnd, &rect);

		nonMaxRect = Rect(Point(rect.left, rect.top), Size(rect.right - rect.left, rect.bottom - rect.top));
	}

	SetMetrics(Point(workArea.left - (frameSize.cx - 2), workArea.top - (frameSize.cy - 2)), Size(workArea.right - workArea.left + (2 * frameSize.cx - 4), workArea.bottom - workArea.top + (2 * frameSize.cy - 4)));

	if (Setup::enableUnicode) origWndStyle = GetWindowLongW(hwnd, GWL_STYLE);
	else			  origWndStyle = GetWindowLongA(hwnd, GWL_STYLE);

	if (Setup::enableUnicode) SetWindowLongW(hwnd, GWL_STYLE, (origWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);
	else			  SetWindowLongA(hwnd, GWL_STYLE, (origWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);

	maximized = True;

	onMaximize.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Restore()
{
	SetMetrics(Point(nonMaxRect.left, nonMaxRect.top), Size((Int) Math::Max(minSize.cx, nonMaxRect.right - nonMaxRect.left), (Int) Math::Max(minSize.cy, nonMaxRect.bottom - nonMaxRect.top)));

	if (Setup::enableUnicode) SetWindowLongW(hwnd, GWL_STYLE, origWndStyle | WS_VISIBLE);
	else			  SetWindowLongA(hwnd, GWL_STYLE, origWndStyle | WS_VISIBLE);

	maximized = False;

	onRestore.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Raise()
{
	SetActiveWindow(hwnd);

	return Success();
}

S::Void S::GUI::WindowGDI::InitMouseNotifier()
{
	mouseNotifyTimer = new System::Timer();

	mouseNotifyTimer->onInterval.Connect(&WindowGDI::MouseNotifier);
	mouseNotifyTimer->Start(25);
}

S::Void S::GUI::WindowGDI::FreeMouseNotifier()
{
	mouseNotifyTimer->Stop();
	
	delete mouseNotifyTimer;
}

S::Void S::GUI::WindowGDI::MouseNotifier()
{
	static Point	 savedMousePos = Point(0, 0);
	POINT		 currentMousePos;

	GetCursorPos(&currentMousePos);

	Input::Pointer::UpdatePosition(currentMousePos.x, currentMousePos.y);

	if (currentMousePos.x != savedMousePos.x || currentMousePos.y != savedMousePos.y)
	{
		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);

			if (window->IsInUse() && !window->IsMouseOn(Rect(Point(), window->GetSize()))) window->Process(SM_MOUSEMOVE, 0, 0);
		}

		savedMousePos = Point(currentMousePos.x, currentMousePos.y);
	}
}
