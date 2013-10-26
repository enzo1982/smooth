 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/gdi/windowgdi.h>
#include <smooth/gui/window/window.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/system/timer.h>
#include <smooth/system/screen.h>
#include <smooth/init.h>
#include <smooth/backends/win32/backendwin32.h>

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
	type	       = WINDOW_GDI;

	hwnd	       = NIL;
	wndclass       = NIL;

	className      = String::FromInt(System::System::RequestGUID());

	id	       = windowBackends.Add(this);

	minSize	       = Size(160, 24);

	minimized      = False;
	maximized      = False;

	frameSize      = Size(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));
	fontSize       = Surface().GetSurfaceDPI() / 96.0;

	flags	       = 0;

	if (Setup::enableUnicode) sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	else			  sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	destroyIcon    = False;

	nonMaxWndStyle = 0;
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

S::Input::Keyboard::Key S::GUI::WindowGDI::ConvertKey(Int keySym)
{
	Input::Keyboard::Key	 key = Input::Keyboard::KeyOther;

	switch (keySym)
	{
		case VK_LEFT:	 key = Input::Keyboard::KeyLeft;    break;
		case VK_UP:	 key = Input::Keyboard::KeyUp;	    break;
		case VK_RIGHT:	 key = Input::Keyboard::KeyRight;   break;
		case VK_DOWN:	 key = Input::Keyboard::KeyDown;    break;

		case VK_HOME:	 key = Input::Keyboard::KeyHome;    break;
		case VK_END:	 key = Input::Keyboard::KeyEnd;	    break;
		case VK_INSERT:	 key = Input::Keyboard::KeyInsert;  break;
		case VK_DELETE:	 key = Input::Keyboard::KeyDelete;  break;
		case VK_PRIOR:	 key = Input::Keyboard::KeyPrior;   break;
		case VK_NEXT:	 key = Input::Keyboard::KeyNext;    break;

		case VK_RETURN:	 key = Input::Keyboard::KeyReturn;  break;
		case VK_BACK:	 key = Input::Keyboard::KeyBack;    break;
		case VK_TAB:	 key = Input::Keyboard::KeyTab;	    break;

		case VK_SPACE:	 key = Input::Keyboard::KeySpace;   break;

		case VK_SHIFT:	 key = Input::Keyboard::KeyShift;   break;
		case VK_CONTROL: key = Input::Keyboard::KeyControl; break;
		case VK_MENU:	 key = Input::Keyboard::KeyAlt;	    break;

		case VK_ESCAPE:	 key = Input::Keyboard::KeyEscape;  break;

		case VK_F1:	 key = Input::Keyboard::KeyF1;	    break;
		case VK_F2:	 key = Input::Keyboard::KeyF2;	    break;
		case VK_F3:	 key = Input::Keyboard::KeyF3;	    break;
		case VK_F4:	 key = Input::Keyboard::KeyF4;	    break;
		case VK_F5:	 key = Input::Keyboard::KeyF5;	    break;
		case VK_F6:	 key = Input::Keyboard::KeyF6;	    break;
		case VK_F7:	 key = Input::Keyboard::KeyF7;	    break;
		case VK_F8:	 key = Input::Keyboard::KeyF8;	    break;
		case VK_F9:	 key = Input::Keyboard::KeyF9;	    break;
		case VK_F10:	 key = Input::Keyboard::KeyF10;	    break;
		case VK_F11:	 key = Input::Keyboard::KeyF11;	    break;
		case VK_F12:	 key = Input::Keyboard::KeyF12;	    break;
		case VK_F13:	 key = Input::Keyboard::KeyF13;	    break;
		case VK_F14:	 key = Input::Keyboard::KeyF14;	    break;
		case VK_F15:	 key = Input::Keyboard::KeyF15;	    break;
		case VK_F16:	 key = Input::Keyboard::KeyF16;	    break;
		case VK_F17:	 key = Input::Keyboard::KeyF17;	    break;
		case VK_F18:	 key = Input::Keyboard::KeyF18;	    break;
		case VK_F19:	 key = Input::Keyboard::KeyF19;	    break;
		case VK_F20:	 key = Input::Keyboard::KeyF20;	    break;
		case VK_F21:	 key = Input::Keyboard::KeyF21;	    break;
		case VK_F22:	 key = Input::Keyboard::KeyF22;	    break;
		case VK_F23:	 key = Input::Keyboard::KeyF23;	    break;
		case VK_F24:	 key = Input::Keyboard::KeyF24;	    break;
	}

	if	(keySym >= '0' && keySym <= '9') key = (Input::Keyboard::Key) keySym;
	else if	(keySym >= 'A' && keySym <= 'Z') key = (Input::Keyboard::Key) keySym;

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
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				const WindowGDI	*backend = WindowGDI::GetWindowBackend((HWND) lParam);

				if (backend != NIL)
				{
					if ((backend->flags & WF_TOPMOST)      &&
					    (backend->flags & WF_NOTASKBUTTON) &&
					    (backend->flags & WF_THINBORDER)   && backend->id > id)
					{
						PostMessage(hwnd, WM_NCACTIVATE, True, 0);
					}
				}
			}

			return Break;
		case WM_ACTIVATEAPP:
			if (wParam == True && !IsWindowEnabled(hwnd))
			{
				/* If disabled, activate the next modal window.
				 */
				for (Int i = 0; i < windowBackends.Length(); i++)
				{
					WindowGDI	*backend = windowBackends.GetNth(i);

					if (backend->hwnd != NIL && backend->id > id && (backend->flags & WF_MODAL))
					{
						backend->Raise();

						break;
					}
				}
			}

			break;
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
				Rect	 workArea = System::Screen::GetActiveScreenWorkArea();

				SetMetrics(Point(workArea.left - (frameSize.cx - 2), workArea.top - (frameSize.cy - 2)), Size(workArea.GetWidth() + (2 * frameSize.cx - 4), workArea.GetHeight() + (2 * frameSize.cy - 4)) / fontSize);
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
					minMaxInfo->ptMinTrackSize.x = Math::Round(minSize.cx * fontSize) + sizeModifier.cx;
					minMaxInfo->ptMinTrackSize.y = Math::Round(minSize.cy * fontSize) + sizeModifier.cy;

					if (maxSize.cx > 0) minMaxInfo->ptMaxTrackSize.x = Math::Round(maxSize.cx * fontSize) + sizeModifier.cx;
					if (maxSize.cy > 0) minMaxInfo->ptMaxTrackSize.y = Math::Round(maxSize.cy * fontSize) + sizeModifier.cy;
				}
			}

			return Break;
		case WM_SYSCOLORCHANGE:
			GetColors();

			break;
	}

	/* Convert Windows messages to smooth messages.
	 */
	switch (message)
	{
		/* Mouse messages:
		 */
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			/* Update pointer position in Input::Pointer.
			 */
			{
				POINT	 point;

				GetCursorPos(&point);

				Input::Pointer::UpdatePosition(Window::GetWindow(hwnd), point.x, point.y);
			}

			return onEvent.Call(SM_MOUSEMOVE, 0, 0);
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
			/* Update pointer position in Input::Pointer.
			 */
			{
				POINT	 point;

				GetCursorPos(&point);

				Input::Pointer::UpdatePosition(Window::GetWindow(hwnd), point.x, point.y);
			}

			/* Pass message to smooth window.
			 */
			if	(message == WM_LBUTTONDOWN   || message == WM_NCLBUTTONDOWN)   return onEvent.Call(SM_LBUTTONDOWN, 0, 0);
			else if (message == WM_LBUTTONUP     || message == WM_NCLBUTTONUP)     return onEvent.Call(SM_LBUTTONUP, 0, 0);
			else if (message == WM_LBUTTONDBLCLK || message == WM_NCLBUTTONDBLCLK) return onEvent.Call(SM_LBUTTONDBLCLK, 0, 0);

			else if	(message == WM_RBUTTONDOWN   || message == WM_NCRBUTTONDOWN)   return onEvent.Call(SM_RBUTTONDOWN, 0, 0);
			else if (message == WM_RBUTTONUP     || message == WM_NCRBUTTONUP)     return onEvent.Call(SM_RBUTTONUP, 0, 0);
			else if (message == WM_RBUTTONDBLCLK || message == WM_NCRBUTTONDBLCLK) return onEvent.Call(SM_RBUTTONDBLCLK, 0, 0);

			else if	(message == WM_MBUTTONDOWN   || message == WM_NCMBUTTONDOWN)   return onEvent.Call(SM_MBUTTONDOWN, 0, 0);
			else if (message == WM_MBUTTONUP     || message == WM_NCMBUTTONUP)     return onEvent.Call(SM_MBUTTONUP, 0, 0);
			else if (message == WM_MBUTTONDBLCLK || message == WM_NCMBUTTONDBLCLK) return onEvent.Call(SM_MBUTTONDBLCLK, 0, 0);
		case WM_MOUSEWHEEL:
			/* Update pointer position in Input::Pointer.
			 */
			{
				POINT	 point;

				GetCursorPos(&point);

				Input::Pointer::UpdatePosition(Window::GetWindow(hwnd), point.x, point.y);
			}

			return onEvent.Call(SM_MOUSEWHEEL, (short) HIWORD(wParam), 0);

		/* Keyboard messages:
		 */
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			Input::Keyboard::UpdateKeyState(ConvertKey(wParam), True);

			return onEvent.Call(SM_KEYDOWN, ConvertKey(wParam), lParam);
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Input::Keyboard::UpdateKeyState(ConvertKey(wParam), False);

			return onEvent.Call(SM_KEYUP, ConvertKey(wParam), lParam);
		case WM_CHAR:
			return onEvent.Call(SM_CHAR, wParam, lParam);

		/* Paint messages:
		 */
		case WM_ERASEBKGND:
			{
				static Size	 prevSize;

				RECT		 windowRect;

				GetWindowRect(hwnd, &windowRect);

				Size		 size = Size(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);

				if (size.cx > prevSize.cx || size.cy > prevSize.cy)
				{
					HBRUSH	 brush = CreateSolidBrush(Setup::BackgroundColor);

					if (size.cx > prevSize.cx) { RECT rect = { prevSize.cx - 2 * frameSize.cx + 2, 0, size.cx - 2 * frameSize.cx + 2, size.cy - 2 * frameSize.cy + 2 }; FillRect((HDC) wParam, &rect, brush); }
					if (size.cy > prevSize.cy) { RECT rect = { 0, prevSize.cy - 2 * frameSize.cy + 2, size.cx - 2 * frameSize.cx + 2, size.cy - 2 * frameSize.cy + 2 }; FillRect((HDC) wParam, &rect, brush); }

					::DeleteObject(brush);
				}

				prevSize = size;
			}

			return Break;
		case WM_PAINT:
			{
				Int	 rVal = Break;
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = Rect();

				if (::GetUpdateRect(hwnd, &uRect, 0))
				{
					updateRect = Rect(Point(uRect.left, uRect.top), Size(uRect.right - uRect.left, uRect.bottom - uRect.top));
					updateRect = updateRect + frameSize - sizeModifier;

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
				WINDOWPOS	*windowPos = (LPWINDOWPOS) lParam;

				if (!(windowPos->flags & SWP_NOMOVE) && !(windowPos->flags & SWP_NOSIZE))
				{
					if (drawSurface != NIL) drawSurface->SetSize(Size(windowPos->cx, windowPos->cy));

					onEvent.Call(SM_WINDOWMETRICS, ((	     windowPos->x	       + 32768) << 16) | (	      windowPos->y		+ 32768),
								       ((Math::Floor(windowPos->cx / fontSize) + 32768) << 16) | (Math::Floor(windowPos->cy / fontSize) + 32768));
				}
				else if (!(windowPos->flags & SWP_NOMOVE && windowPos->flags & SWP_NOSIZE))
				{
					RECT	 windowRect;

					GetWindowRect(hwnd, &windowRect);

					if (windowPos->flags & SWP_NOMOVE)
					{
						if (drawSurface != NIL) drawSurface->SetSize(Size(windowPos->cx, windowPos->cy));

						onEvent.Call(SM_WINDOWMETRICS, ((	      windowRect.left				      + 32768) << 16) | (	      windowRect.top				      + 32768),
									       ((Math::Floor( windowPos->cx			  / fontSize) + 32768) << 16) | (Math::Floor( windowPos->cy			  / fontSize) + 32768));
					}
					else if (windowPos->flags & SWP_NOSIZE)
					{
						onEvent.Call(SM_WINDOWMETRICS, ((	      windowPos->x				      + 32768) << 16) | (	      windowPos->y				      + 32768),
									       ((Math::Floor((windowRect.right - windowRect.left) / fontSize) + 32768) << 16) | (Math::Floor((windowRect.bottom - windowRect.top) / fontSize) + 32768));
					}
				}
			}

			return Success();
		case WM_SETFOCUS:
			return onEvent.Call(SM_GETFOCUS, 0, 0);
		case WM_KILLFOCUS:
			Input::Keyboard::ResetKeyState();

			/* Get the window that now has the focus.
			 */
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
	 * FixMe: Windows messages should not be propagated to smooth applications.
	 *	  Define a smooth message for every Windows message we need and
	 *	  replace the next line with return Success();.
	 */
	return onEvent.Call(message, wParam, lParam);
}

S::Int S::GUI::WindowGDI::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	flags = iFlags;

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
		if ((flags & WF_TOPMOST) && (flags & WF_NOTASKBUTTON) && (flags & WF_THINBORDER)) 
		{
			OSVERSIONINFOA	 vInfo;

			vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

			GetVersionExA(&vInfo);

			if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && ( vInfo.dwMajorVersion >= 6 ||
									    (vInfo.dwMajorVersion == 5 && vInfo.dwMinorVersion >= 1)))
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

		hwnd = CreateWindowExW(extStyle, className, title, style, pos.x, pos.y, Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy, NIL, NIL, hDllInstance, NIL);
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

			if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && ( vInfo.dwMajorVersion >= 6 ||
									    (vInfo.dwMajorVersion == 5 && vInfo.dwMinorVersion >= 1)))
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

		hwnd = CreateWindowExA(extStyle, className, title, style, pos.x, pos.y, Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy, NIL, NIL, hDllInstance, NIL);
	}

	if (hwnd != NIL)
	{
		/* Process the WM_CREATE message again, as GetWindowBackend
		 * cannot find the correct backend until hwnd is set.
		 */
		ProcessSystemMessages(WM_CREATE, 0, 0);

		if ((flags & WF_THINBORDER) || (flags & WF_NORESIZE)) drawSurface = new Surface(hwnd, size * fontSize + sizeModifier);
		else						      drawSurface = new Surface(hwnd);

		drawSurface->SetSize(size * fontSize + sizeModifier);

		/* Disable leader window if we are modal.
		 */
		if (flags & WF_MODAL)
		{
			WindowGDI	*leader = FindLeaderWindow();

			if (leader != NIL)
			{
				EnableWindow(leader->hwnd, False);

				Raise();
			}
		}

		return Success();
	}

	return Error();
}

S::Int S::GUI::WindowGDI::Close()
{
	if (hwnd == NIL) return Success();

	/* Reenable leader window if we have one.
	 */
	if (flags & WF_MODAL)
	{
		WindowGDI	*leader = FindLeaderWindow();

		if (leader != NIL)
		{
			EnableWindow(leader->hwnd, True);

			leader->Raise();
		}
	}

	/* Delete surface.
	 */
	if (drawSurface != NIL) delete drawSurface;

	drawSurface = NIL;

	/* Destroy window and unregister class.
	 */
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

S::GUI::WindowGDI *S::GUI::WindowGDI::FindLeaderWindow()
{
	/* The leader window is the newest non topmost window.
	 */
	for (Int i = windowBackends.Length() - 1; i >= 0; i--)
	{
		WindowGDI	*backend = windowBackends.GetNth(i);

		if (backend->id < id && backend->hwnd != NIL && !(backend->flags & WF_TOPMOST)) return backend;
	}

	return NIL;
}

S::Int S::GUI::WindowGDI::SetTitle(const String &nTitle)
{
	if (hwnd == NIL) return Error();

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

	Size	 size  = newIcon.GetSize();
	Int	 depth = newIcon.GetDepth();

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			Color	 pixel = newIcon.GetPixel(Point(x, y));

			if (depth == 32) mask.SetPixel(Point(x, y), pixel.GetAlpha() >=    128  ? transparentPixel : 0);
			else		 mask.SetPixel(Point(x, y), pixel == Color(192,192,192) ? transparentPixel : 0);
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

	SetMetrics(Point(windowRect.left, windowRect.top),
		   Size(Math::Max((Int) Math::Round((windowRect.right  - windowRect.left - sizeModifier.cx) / fontSize), minSize.cx),
			Math::Max((Int) Math::Round((windowRect.bottom - windowRect.top  - sizeModifier.cy) / fontSize), minSize.cy)));

	return Success();
}

S::Int S::GUI::WindowGDI::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	RECT	 windowRect;

	GetWindowRect(hwnd, &windowRect);

	SetMetrics(Point(windowRect.left, windowRect.top),
		   Size(Math::Min((Int) Math::Round((windowRect.right  - windowRect.left - sizeModifier.cx) / fontSize), maxSize.cx),
			Math::Min((Int) Math::Round((windowRect.bottom - windowRect.top  - sizeModifier.cy) / fontSize), maxSize.cy)));

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
	SetWindowPos(hwnd, 0, nPos.x, nPos.y, Math::Round(nSize.cx * fontSize) + sizeModifier.cx, Math::Round(nSize.cy * fontSize) + sizeModifier.cy, SWP_NOZORDER);

	return Success();
}

S::Int S::GUI::WindowGDI::Minimize()
{
	ShowWindow(hwnd, SW_MINIMIZE);

	return Success();
}

S::Int S::GUI::WindowGDI::Maximize()
{
	Rect	 workArea = System::Screen::GetActiveScreenWorkArea();

	{
		RECT	 rect;

		GetWindowRect(hwnd, &rect);

		nonMaxRect = Rect(Point(rect.left, rect.top), (Size(rect.right - rect.left, rect.bottom - rect.top) - sizeModifier) / fontSize);
	}

	SetMetrics(Point(workArea.left - (frameSize.cx - 2), workArea.top - (frameSize.cy - 2)), (Size(workArea.GetWidth() + (2 * frameSize.cx - 4), workArea.GetHeight() + (2 * frameSize.cy - 4)) - sizeModifier) / fontSize);

	if (Setup::enableUnicode) nonMaxWndStyle = GetWindowLongW(hwnd, GWL_STYLE);
	else			  nonMaxWndStyle = GetWindowLongA(hwnd, GWL_STYLE);

	if (Setup::enableUnicode) SetWindowLongW(hwnd, GWL_STYLE, (nonMaxWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);
	else			  SetWindowLongA(hwnd, GWL_STYLE, (nonMaxWndStyle ^ WS_THICKFRAME) | WS_DLGFRAME);

	maximized = True;

	onMaximize.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Restore()
{
	SetMetrics(Point(nonMaxRect.left, nonMaxRect.top), Size((Int) Math::Max(minSize.cx, nonMaxRect.GetWidth()), (Int) Math::Max(minSize.cy, nonMaxRect.GetHeight())));

	if (Setup::enableUnicode) SetWindowLongW(hwnd, GWL_STYLE, nonMaxWndStyle | WS_VISIBLE);
	else			  SetWindowLongA(hwnd, GWL_STYLE, nonMaxWndStyle | WS_VISIBLE);

	maximized = False;

	onRestore.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Raise()
{
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

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

	Input::Pointer::UpdatePosition(Window::GetWindow(WindowFromPoint(currentMousePos)), currentMousePos.x, currentMousePos.y);

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
