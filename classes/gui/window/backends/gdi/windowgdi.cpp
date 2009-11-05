 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/basic/input.h>

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
			if (wParam == SIZE_MINIMIZED)
			{
				minimized = True;

				onMinimize.Emit();
			}
			else if (wParam == SIZE_RESTORED)
			{
				minimized = False;

				onRestore.Emit();
			}

			break;
		case WM_SETTINGCHANGE:
			if ((wParam == SPI_SETWORKAREA) && maximized)
			{
				RECT	 rect;

				if (Setup::enableUnicode) SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
				else			  SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

				SetMetrics(Point(rect.left - 2, rect.top - 2), Size(rect.right - rect.left + 4, rect.bottom - rect.top + 4));
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
					minMaxInfo->ptMaxSize = Point(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
					minMaxInfo->ptMaxPosition = Point(windowRect.left, windowRect.top);
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
			return onEvent.Call(SM_KEYDOWN, wParam, lParam);
		case WM_KEYUP:
		case WM_SYSKEYUP:
			return onEvent.Call(SM_KEYUP, wParam, lParam);
		case WM_CHAR:
			return onEvent.Call(SM_CHAR, wParam, lParam);

		/* Paint messages:
		 */
		case WM_PAINT:
			{
				Int	 rVal = Break;
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = uRect;

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = uRect;
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

				onEvent.Call(SM_WINDOWMETRICS, (wndpos->x << 16) | wndpos->y, (wndpos->cx << 16) | wndpos->cy);
			}

			return Success();

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
		wndclassw->lpfnWndProc	 = WindowProc;
		wndclassw->cbClsExtra	 = 0;
		wndclassw->cbWndExtra	 = 0;
		wndclassw->hInstance	 = hDllInstance;
		wndclassw->hIcon	 = sysIcon;
		wndclassw->hCursor	 = (HCURSOR) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
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
		wndclassa->lpfnWndProc	 = WindowProc;
		wndclassa->cbClsExtra	 = 0;
		wndclassa->cbWndExtra	 = 0;
		wndclassa->hInstance	 = hDllInstance;
		wndclassa->hIcon	 = sysIcon;
		wndclassa->hCursor	 = (HCURSOR) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
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
	if (Setup::enableUnicode) SendMessageW(hwnd, WM_CLOSE, 0, 0);
	else			  SendMessageA(hwnd, WM_CLOSE, 0, 0);

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

S::Int S::GUI::WindowGDI::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	return Success();
}

S::Int S::GUI::WindowGDI::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

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

		nonMaxRect = rect;
	}

	SetMetrics(Point(workArea.left - 2, workArea.top - 2), Size(workArea.right - workArea.left + 4, workArea.bottom - workArea.top + 4));

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

S::Void S::GUI::WindowGDI::InitMouseNotifier()
{
	mouseNotifyTimer = new System::Timer();

	mouseNotifyTimer->onInterval.Connect(&WindowGDI::MouseNotifier);
	mouseNotifyTimer->Start(50);
}

S::Void S::GUI::WindowGDI::FreeMouseNotifier()
{
	mouseNotifyTimer->Stop();
	
	delete mouseNotifyTimer;
}

S::Void S::GUI::WindowGDI::MouseNotifier()
{
	static Point	 savedMousePos	 = Point(0, 0);
	const Point	&currentMousePos = Input::GetMousePosition();

	if (currentMousePos != savedMousePos)
	{
		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);

			if (window->IsInUse()) window->Process(SM_MOUSEMOVE, 1, 0);
		}

		savedMousePos = currentMousePos;
	}
}
