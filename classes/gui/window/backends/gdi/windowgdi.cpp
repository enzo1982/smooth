 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/loop.h>

S::GUI::WindowBackend *CreateWindowGDI()
{
	return new S::GUI::WindowGDI();
}

S::Int	 windowGDITmp = S::GUI::WindowBackend::AddBackend(&CreateWindowGDI);

S::Array<S::GUI::WindowGDI *, S::Void *>	 S::GUI::WindowGDI::windowBackends;

LRESULT CALLBACK S::GUI::WindowGDI::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowGDI	*smoothWindow = WindowGDI::GetWindowBackend(window);
	Int		 retVal = -1;
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

	if (smoothWindow != NIL)
	{
		retVal = smoothWindow->ProcessSystemMessages(message, wParam, lParam);

		if (retVal == Success()) retVal = smoothWindow->onEvent.Call(message, param1, param2);
	}

	if (retVal == Break)		return 0;
	else if (Setup::enableUnicode)	return DefWindowProcW(window, originalMessage, wParam, lParam);
	else				return DefWindowProcA(window, originalMessage, wParam, lParam);
}

S::GUI::WindowGDI::WindowGDI(Void *iWindow)
{
	type = WINDOW_GDI;

	hwnd		= NIL;
	wndclass	= NIL;

	className	= String::FromInt(System::System::RequestGUID());

	id		= windowBackends.AddEntry(this);

	minSize		= Size(160, 24);

	maximized	= False;

	if (Setup::enableUnicode)	sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	else				sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	destroyIcon	= False;
}

S::GUI::WindowGDI::~WindowGDI()
{
	if (destroyIcon) DestroyIcon(sysIcon);

	windowBackends.RemoveEntry(id);
}

S::Void *S::GUI::WindowGDI::GetSystemWindow() const
{
	return (Void *) hwnd;
}

S::GUI::WindowGDI *S::GUI::WindowGDI::GetWindowBackend(HWND hwnd)
{
	if (hwnd == NIL) return NIL;

	for (Int i = 0; i < windowBackends.GetNOfEntries(); i++)
	{
		WindowGDI	*window = windowBackends.GetNthEntry(i);

		if (window != NIL)
		{
			if (window->hwnd == hwnd) return window;
		}
	}

	return NIL;
}

S::Int S::GUI::WindowGDI::ProcessSystemMessages(Int message, Int wParam, Int lParam)
{
	switch (message)
	{
		case WM_SETTINGCHANGE:
			if ((wParam == SPI_SETWORKAREA) && maximized)
			{
				RECT	 rect;

				if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
				else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

				SetMetrics(Point(rect.left - 2, rect.top - 2), Size(rect.right - rect.left + 4, rect.bottom - rect.top + 4));
			}

			return Break;
		case WM_GETMINMAXINFO:
			{
				RECT	 windowRect;
				Int	 windowStyle;

				GetWindowRect(hwnd, &windowRect);

				if (Setup::enableUnicode)	windowStyle = GetWindowLongW(hwnd, GWL_STYLE);
				else				windowStyle = GetWindowLongA(hwnd, GWL_STYLE);

				if (windowStyle & WS_DLGFRAME)
				{
					((LPMINMAXINFO) lParam)->ptMaxSize = Point(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
					((LPMINMAXINFO) lParam)->ptMaxPosition = Point(windowRect.left, windowRect.top);
				}
				else
				{
					((LPMINMAXINFO) lParam)->ptMinTrackSize.x = minSize.cx;
					((LPMINMAXINFO) lParam)->ptMinTrackSize.y = minSize.cy;

					if (maxSize.cx > 0) ((LPMINMAXINFO) lParam)->ptMaxTrackSize.x = maxSize.cx;
					if (maxSize.cy > 0) ((LPMINMAXINFO) lParam)->ptMaxTrackSize.y = maxSize.cy;
				}
			}

			return Break;
	}

	return Success();
}

S::Int S::GUI::WindowGDI::Open(const String &title, const Point &pos, const Size &size, Int flags)
{
	Int	 style		= WS_THICKFRAME | WS_SYSMENU | WS_POPUP;
	Int	 extStyle	= 0;

	if (flags & WF_NORESIZE)	style		= (style & ~WS_THICKFRAME) | WS_DLGFRAME;
	if (flags & WF_THINBORDER)	style		= (style & ~WS_THICKFRAME & ~WS_SYSMENU);
	if (flags & WF_TOPMOST)		extStyle	= extStyle | WS_EX_TOPMOST;
	if (flags & WF_NOTASKBUTTON)	extStyle	= extStyle | WS_EX_TOOLWINDOW;

	if (Setup::enableUnicode)
	{
		wndclass = new WNDCLASSEXW;

		WNDCLASSEXW	*wndclassw = (WNDCLASSEXW *) wndclass;

		wndclassw->cbSize	= sizeof(WNDCLASSEXW);
		wndclassw->style	= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclassw->lpfnWndProc	= WindowProc;
		wndclassw->cbClsExtra	= 0;
		wndclassw->cbWndExtra	= 0;
		wndclassw->hInstance	= hDllInstance;
		wndclassw->hIcon	= sysIcon;
		wndclassw->hCursor	= (HCURSOR) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		wndclassw->hbrBackground= NIL;
		wndclassw->lpszMenuName	= NIL;
		wndclassw->lpszClassName= className;
		wndclassw->hIconSm	= sysIcon;

		RegisterClassExW(wndclassw);

		hwnd = CreateWindowExW(extStyle, className, title, style, pos.x, pos.y, size.cx, size.cy, NIL, NIL, hDllInstance, NIL);
	}
	else
	{
		wndclass = new WNDCLASSEXA;

		WNDCLASSEXA	*wndclassa = (WNDCLASSEXA *) wndclass;

		wndclassa->cbSize	= sizeof(WNDCLASSEXA);
		wndclassa->style	= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclassa->lpfnWndProc	= WindowProc;
		wndclassa->cbClsExtra	= 0;
		wndclassa->cbWndExtra	= 0;
		wndclassa->hInstance	= hDllInstance;
		wndclassa->hIcon	= sysIcon;
		wndclassa->hCursor	= (HCURSOR) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
		wndclassa->hbrBackground= NIL;
		wndclassa->lpszMenuName	= NIL;
		wndclassa->lpszClassName= className;
		wndclassa->hIconSm	= sysIcon;

		RegisterClassExA(wndclassa);

		hwnd = CreateWindowExA(extStyle, className, title, style, pos.x, pos.y, size.cx, size.cy, NIL, NIL, hDllInstance, NIL);
	}

	if (hwnd != NIL)
	{
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

S::Int S::GUI::WindowGDI::SetTitle(const String &nTitle)
{
	if (Setup::enableUnicode)	SetWindowTextW(hwnd, nTitle);
	else				SetWindowTextA(hwnd, nTitle);

	return Success();
}

S::Int S::GUI::WindowGDI::SetIcon(const Bitmap &newIcon)
{
	if (destroyIcon) DestroyIcon(sysIcon);

	ICONINFO	 icon;
	Bitmap		 mask = newIcon;

	for (Int y = 0; y < mask.GetSize().cy; y++)
	{
		for (Int x = 0; x < mask.GetSize().cx; x++)
		{
			if (mask.GetPixel(x, y) == (unsigned) Setup::BackgroundColor)	mask.SetPixel(x, y, 16777215);
			else								mask.SetPixel(x, y, 0);
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

	if (Setup::enableUnicode)	origWndStyle = GetWindowLongW(hwnd, GWL_STYLE);
	else				origWndStyle = GetWindowLongA(hwnd, GWL_STYLE);

	if (Setup::enableUnicode)	SetWindowLongW(hwnd, GWL_STYLE, (origWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);
	else				SetWindowLongA(hwnd, GWL_STYLE, (origWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);

	maximized = True;

	return Success();
}

S::Int S::GUI::WindowGDI::Restore()
{
	SetMetrics(Point(nonMaxRect.left, nonMaxRect.top), Size((Int) Math::Max(minSize.cx, nonMaxRect.right - nonMaxRect.left), (Int) Math::Max(minSize.cy, nonMaxRect.bottom - nonMaxRect.top)));

	if (Setup::enableUnicode)	SetWindowLongW(hwnd, GWL_STYLE, origWndStyle | WS_VISIBLE);
	else				SetWindowLongA(hwnd, GWL_STYLE, origWndStyle | WS_VISIBLE);

	maximized = False;

	return Success();
}
