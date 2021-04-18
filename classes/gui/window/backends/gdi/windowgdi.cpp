 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/gdi/windowgdi.h>
#include <smooth/gui/window/backends/gdi/droptarget.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/application/application.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/system/screen.h>
#include <smooth/threads/thread.h>
#include <smooth/foreach.h>
#include <smooth/backends/win32/backendwin32.h>

#if !defined SM_CXPADDEDBORDER
#	define SM_CXPADDEDBORDER 92
#endif

S::GUI::WindowBackend *CreateWindowGDI()
{
	return new S::GUI::WindowGDI();
}

S::Int	 windowGDITmp = S::GUI::WindowBackend::SetBackend(&CreateWindowGDI);

S::Int	 addWindowGDIInitTmp = S::AddInitFunction(&S::GUI::WindowGDI::Initialize);
S::Int	 addWindowGDIFreeTmp = S::AddFreeFunction(&S::GUI::WindowGDI::Free);

S::Array<S::GUI::WindowGDI *, S::Void *>	 S::GUI::WindowGDI::windowBackends;

S::GUI::Cursor		*S::GUI::WindowGDI::activeCursor = NIL;

LRESULT CALLBACK S::GUI::WindowGDI::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowGDI	*smoothWindow = WindowGDI::GetWindowBackend(window);

	if (smoothWindow != NIL)
	{
		Int	 result = smoothWindow->ProcessSystemMessages(message, wParam, lParam);

		if	(result == MessageProcessed) return 0;
		else if (result != MessageUnknown)   return result;
	}

	return DefWindowProc(window, message, wParam, lParam);
}

S::Int S::GUI::WindowGDI::Initialize()
{
	/* Register for cursor events.
	 */
	Cursor::internalSetCursor.Connect(&WindowGDI::SetCursor);
	Cursor::internalRemoveCursor.Connect(&WindowGDI::RemoveCursor);

	return Success();
}

S::Int S::GUI::WindowGDI::Free()
{
	/* Unregister cursor event handlers.
	 */
	Cursor::internalSetCursor.Disconnect(&WindowGDI::SetCursor);
	Cursor::internalRemoveCursor.Disconnect(&WindowGDI::RemoveCursor);

	return Success();
}

S::GUI::WindowGDI::WindowGDI(Void *iWindow)
{
	type		 = WINDOW_GDI;

	hwnd		 = NIL;
	wndclass	 = NIL;

	taskbar		 = NIL;

	className	 = String::FromInt(System::System::RequestGUID());

	id		 = windowBackends.Add(this);

	minSize		 = Size(160, 24);

	minimized	 = False;
	maximized	 = False;

	frameSize	 = Size(GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER), GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER));
	fontSize	 = Surface().GetSurfaceDPI() / 96.0;

	flags		 = 0;

	sysIcon		 = (HICON) LoadImage(NIL, MAKEINTRESOURCE(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	destroyIcon	 = False;

	hDrop		 = NIL;
	dropTarget	 = NIL;
	enableDropFiles	 = False;

	restoredWndStyle = 0;
}

S::GUI::WindowGDI::~WindowGDI()
{
	if (destroyIcon) DestroyIcon(sysIcon);

	if (taskbar != NIL) taskbar->Release();

	windowBackends.Remove(id);
}

S::Void *S::GUI::WindowGDI::GetSystemWindow() const
{
	return (Void *) hwnd;
}

S::GUI::WindowGDI *S::GUI::WindowGDI::GetWindowBackend(HWND hwnd)
{
	if (hwnd == NIL) return NIL;

	foreach (WindowGDI *window, windowBackends)
	{
		if (window	 != NIL &&
		    window->hwnd == hwnd) return window;
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

S::Int S::GUI::WindowGDI::ProcessSystemMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	/* Lock application while processing messages.
	 */
	Application::Lock	 lock;

	/* Process system messages not relevant
	 * to portable Window implementation.
	 */
	switch (message)
	{
		case WM_CLOSE:
			if (doClose.Call()) Close();

			return MessageProcessed;

		case WM_CREATE:
			onCreate.Emit();

			return MessageProcessed;

		case WM_DESTROY:
			onDestroy.Emit();

			return MessageProcessed;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE && IsWindowEnabled(hwnd))
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

			return MessageProcessed;

		case WM_ACTIVATEAPP:
			if (wParam == TRUE && GetForegroundWindow() == hwnd && !IsWindowEnabled(hwnd))
			{
				/* If disabled, activate the next modal window.
				 */
				foreach (WindowGDI *backend, windowBackends)
				{
					if ( backend->hwnd  != NIL &&
					     backend->id    >  id  &&
					    (backend->flags &  WF_MODAL)) { backend->Raise(); break; }
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

			return MessageProcessed;

		case WM_GETMINMAXINFO:
			{
				RECT		 windowRect;
				Int		 windowStyle = GetWindowLong(hwnd, GWL_STYLE);
				LPMINMAXINFO	 minMaxInfo  = (LPMINMAXINFO) lParam;

				GetWindowRect(hwnd, &windowRect);

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

			return MessageProcessed;

		case WM_SYSCOLORCHANGE:
			Backends::BackendWin32::UpdateColors();

			break;

		case WM_IME_STARTCOMPOSITION:
			if (activeCursor != NIL) activeCursor->SetIMECursor(True);

			break;

		case WM_IME_ENDCOMPOSITION:
			if (activeCursor != NIL) activeCursor->SetIMECursor(False);

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

			/* Register for mouse leave events.
			 */
			{
				TRACKMOUSEEVENT tme;

				tme.cbSize	= sizeof(TRACKMOUSEEVENT);
				tme.dwFlags	= TME_LEAVE;
				tme.hwndTrack	= hwnd;
				tme.dwHoverTime = 0;

				TrackMouseEvent(&tme);
			}

			return onEvent.Call(SM_MOUSEMOVE, 0, 0);

		case WM_MOUSELEAVE:
			/* Update pointer position in Input::Pointer.
			 */
			if (IsWindowEnabled(hwnd))
			{
				POINT	 point;

				GetCursorPos(&point);

				Input::Pointer::UpdatePosition(Window::GetWindow(WindowFromPoint(point)), point.x, point.y);
			}
			else
			{
				/* Clear mouse over status when being disabled.
				 */
				Input::Pointer::UpdatePosition(NIL, -1, -1);
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

			/* Receive all mouse events while button is pressed.
			 */
			if	(message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN) SetCapture(hwnd);
			else if (message == WM_LBUTTONUP   || message == WM_RBUTTONUP	|| message == WM_MBUTTONUP)   ReleaseCapture();

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
			/* Update pointer position in Input::Pointer if this is not a tool window.
			 */
			if (!((flags & WF_TOPMOST) && (flags & WF_NOTASKBUTTON) && (flags & WF_THINBORDER)))
			{
				POINT	 point;

				GetCursorPos(&point);

				Input::Pointer::UpdatePosition(Window::GetWindow(hwnd), point.x, point.y);
			}

			/* Pass message to smooth window.
			 */
			onEvent.Call(SM_MOUSEWHEEL, (short) HIWORD(wParam), 0);

			return MessageProcessed;

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

			return MessageProcessed;

		case WM_PAINT:
			{
				Int	 rVal  = MessageProcessed;
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
			if (!IsIconic(hwnd))
			{
				WINDOWPOS	*windowPos = (LPWINDOWPOS) lParam;

				if (!(windowPos->flags & SWP_NOMOVE && windowPos->flags & SWP_NOSIZE))
				{
					Point	 pos  = Point(windowPos->x, windowPos->y);
					Size	 size = Size(windowPos->cx, windowPos->cy) / fontSize;

					if (windowPos->flags & SWP_NOMOVE || windowPos->flags & SWP_NOSIZE)
					{
						RECT	 windowRect;

						GetWindowRect(hwnd, &windowRect);

						if	(windowPos->flags & SWP_NOMOVE) pos  = Point(windowRect.left, windowRect.top);
						else if (windowPos->flags & SWP_NOSIZE) size = Size(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top) / fontSize;
					}

					if (!(windowPos->flags & SWP_NOSIZE))
					{
						if (drawSurface != NIL) drawSurface->SetSize(Size(windowPos->cx, windowPos->cy));
					}

					onEvent.Call(SM_WINDOWMETRICS, ((pos.x	 + 32768) << 16) | (pos.y   + 32768),
								       ((size.cx + 32768) << 16) | (size.cy + 32768));
				}
			}

			return MessageProcessed;

		case WM_SETFOCUS:
			return onEvent.Call(SM_GETFOCUS, 0, 0);

		case WM_KILLFOCUS:
			Input::Keyboard::ResetKeyState();

			/* Release mouse capture.
			 */
			ReleaseCapture();

			/* Get the window that now has the focus.
			 */
			{
				Window	*focusWnd = Window::GetWindow((HWND) wParam);

				return onEvent.Call(SM_LOSEFOCUS, focusWnd != NIL ? focusWnd->GetHandle() : -1, 0);
			}

		/* Drag & drop messages:
		 */
		case WM_DROPFILES:
			hDrop = (HDROP) wParam;

			/* Compute window-relative drop position and emit event signal.
			 *
			 * Note that DragQueryPoint started to give incorrect values since
			 * Windows 8 or 8.1, so we are using the mouse pointer position instead.
			 */
			{
				RECT	 windowRect;
				POINT	 cursorPos;

				GetWindowRect(hwnd, &windowRect);
				GetCursorPos(&cursorPos);

				onEvent.Call(SM_DROPFILES, cursorPos.x - windowRect.left, cursorPos.y - windowRect.top);
			}

			DragFinish(hDrop);

			hDrop = NIL;

			return MessageProcessed;
	}

	/* Call event for any other Windows messages.
	 *
	 * FixMe: Windows messages should not be propagated to smooth applications.
	 *	  Define a smooth message for every Windows message we need and
	 *	  replace the next line with return MessageUnknown;.
	 *	  Then add a different way to pass system specific messages to the
	 *	  application, like a special onSystemMessage callback.
	 */
	return onEvent.Call(message, wParam, lParam);
}

S::Int S::GUI::WindowGDI::Open(const String &title, const Point &pos, const Size &size, Int iFlags)
{
	flags = iFlags;

	Int	 style	  = WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
	Int	 extStyle = 0;

	if (flags & WF_NORESIZE	   ) { style	^= WS_THICKFRAME;
				       style	|= WS_DLGFRAME; }
	if (flags & WF_THINBORDER  )   style	^= WS_THICKFRAME | WS_SYSMENU;
	if (flags & WF_TOPMOST	   )   extStyle |= WS_EX_TOPMOST;
	if (flags & WF_NOTASKBUTTON)   extStyle |= WS_EX_TOOLWINDOW;

	wndclass	 = new WNDCLASSEX;
	wndclass->cbSize = sizeof(WNDCLASSEX);
	wndclass->style	 = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	/* Enable shadows for tool windows.
	 */
	if ((flags & WF_TOPMOST) && (flags & WF_NOTASKBUTTON) && (flags & WF_THINBORDER))
	{
		if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 5, 1)) wndclass->style = wndclass->style | CS_DROPSHADOW;
	}

	wndclass->lpfnWndProc	= WindowProc;
	wndclass->cbClsExtra	= 0;
	wndclass->cbWndExtra	= 0;
	wndclass->hInstance	= hDllInstance;
	wndclass->hIcon		= sysIcon;
	wndclass->hCursor	= LoadCursor(NIL, MAKEINTRESOURCE(32512));
	wndclass->hbrBackground = NIL;
	wndclass->lpszMenuName	= NIL;
	wndclass->lpszClassName = className;
	wndclass->hIconSm	= sysIcon;

	RegisterClassEx(wndclass);

	hwnd = CreateWindowEx(extStyle, className, title, style, pos.x, pos.y, Math::Round(size.cx * fontSize) + sizeModifier.cx, Math::Round(size.cy * fontSize) + sizeModifier.cy, NIL, NIL, hDllInstance, NIL);

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
				/* If the leader window belongs to another thread,
				 * we need to suspend our application lock in order
				 * for EnableWindow to finish.
				 */
				Int	 suspendCount = (GetWindowThreadProcessId(leader->hwnd, NIL) != GetCurrentThreadId() ? Application::Lock::SuspendLock() : 0);

				/* Now disable the leader window.
				 */
				EnableWindow(leader->hwnd, False);

				/* Resume the application lock.
				 */
				Application::Lock::ResumeLock(suspendCount);

				/* Raise our new window.
				 */
				Raise();
			}
		}

		/* Enable dropping files if requested.
		 */
		if (enableDropFiles) EnableDropFiles(True);

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
			 /* If the leader window belongs to another thread,
			  * we need to suspend our application lock in order
			  * for EnableWindow to finish.
			  */
			Int	 suspendCount = (GetWindowThreadProcessId(leader->hwnd, NIL) != GetCurrentThreadId() ? Application::Lock::SuspendLock() : 0);

			/* Now enable the leader window.
			 */
			EnableWindow(leader->hwnd, True);

			/* Resume the application lock.
			 */
			Application::Lock::ResumeLock(suspendCount);

			/* Raise the leader window.
			 */
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

	UnregisterClass(className, hDllInstance);

	delete wndclass;

	wndclass = NIL;

	return Success();
}

S::Int S::GUI::WindowGDI::RequestClose()
{
	if (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId()) PostMessage(hwnd, WM_CLOSE, 0, 0);
	else								 SendMessage(hwnd, WM_CLOSE, 0, 0);

	return Success();
}

S::GUI::WindowGDI *S::GUI::WindowGDI::FindLeaderWindow()
{
	/* The leader window is the newest non topmost window.
	 */
	foreachreverse (WindowGDI *backend, windowBackends)
	{
		if (  backend->id    <  id  &&
		      backend->hwnd  != NIL &&
		    !(backend->flags &  WF_TOPMOST)) return backend;
	}

	return NIL;
}

S::Int S::GUI::WindowGDI::SetTitle(const String &nTitle)
{
	if (hwnd == NIL) return Error();

	SetWindowText(hwnd, nTitle);

	return Success();
}

S::Int S::GUI::WindowGDI::SetIcon(const Bitmap &newIcon)
{
	if (destroyIcon) DestroyIcon(sysIcon);

	/* Scale to standard icon size.
	 */
	Size	 iconSize	  = Size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
	Bitmap	 iconScaled	  = newIcon.Scale(iconSize);

	/* Set up alpha mask.
	 */
	Bitmap	 iconMask	  = iconScaled;
	Int	 depth		  = iconScaled.GetDepth();
	Int	 transparentPixel = 16777215;

	for (Int y = 0; y < iconSize.cy; y++)
	{
		for (Int x = 0; x < iconSize.cx; x++)
		{
			Color	 pixel = iconScaled.GetPixel(Point(x, y));

			if (depth == 32) iconMask.SetPixel(Point(x, y), pixel.GetAlpha() >=	 128  ? transparentPixel : 0);
			else		 iconMask.SetPixel(Point(x, y), pixel == Color(192, 192, 192) ? transparentPixel : 0);
		}
	}

	/* Create icon.
	 */
	ICONINFO	 icon;

	icon.fIcon	= true;
	icon.xHotspot	= 0;
	icon.yHotspot	= 0;
	icon.hbmMask	= (HBITMAP) iconMask.GetSystemBitmap();
	icon.hbmColor	= (HBITMAP) iconScaled.GetSystemBitmap();

	sysIcon	    = CreateIconIndirect(&icon);
	destroyIcon = True;

	return Success();
}

S::Int S::GUI::WindowGDI::SetIconDirect(Void *newIcon)
{
	if (destroyIcon) DestroyIcon(sysIcon);

	sysIcon	    = (HICON) newIcon;
	destroyIcon = False;

	return Success();
}

S::Int S::GUI::WindowGDI::EnableDropFiles(Bool nEnableDropFiles)
{
	enableDropFiles = nEnableDropFiles;

	if (hwnd == NIL) return Success();

	/* If the window belongs to another thread, we
	 * need to suspend our application lock in order
	 * for DragAcceptFiles to finish.
	 */
	Int	 suspendCount = (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId() ? Application::Lock::SuspendLock() : 0);

	DragAcceptFiles(hwnd, enableDropFiles);

	if (enableDropFiles && dropTarget == NIL)
	{
		dropTarget = new DropTarget(this);

		RegisterDragDrop(hwnd, dropTarget);
	}
	else if (!enableDropFiles && dropTarget != NIL)
	{
		RevokeDragDrop(hwnd);

		dropTarget->Release();
		dropTarget = NIL;
	}

	/* Resume the application lock.
	 */
	Application::Lock::ResumeLock(suspendCount);

	return Success();
}

const S::Array<S::String> &S::GUI::WindowGDI::GetDroppedFiles() const
{
	if (hDrop == NIL && dropTarget == NIL) return WindowBackend::GetDroppedFiles();

	static Array<String>	 fileNames;

	fileNames.RemoveAll();

	if (hDrop == NIL)
	{
		/* Query files from drop target.
		 */
		const Array<String>	&files = ((DropTarget *) dropTarget)->GetDroppedFiles();

		foreach (const String &file, files) fileNames.Add(file);
	}
	else
	{
		fileNames = DropTarget::GetFilesFromHDROP(hDrop);
	}

	return fileNames;
}

S::Int S::GUI::WindowGDI::SetMinimumSize(const Size &nMinSize)
{
	minSize = nMinSize;

	if (hwnd == NIL) return Success();

	RECT	 windowRect;

	GetWindowRect(hwnd, &windowRect);

	SetMetrics(Point(windowRect.left, windowRect.top),
		   Size(Math::Max(Math::Round((windowRect.right  - windowRect.left - sizeModifier.cx) / fontSize), minSize.cx),
			Math::Max(Math::Round((windowRect.bottom - windowRect.top  - sizeModifier.cy) / fontSize), minSize.cy)));

	return Success();
}

S::Int S::GUI::WindowGDI::SetMaximumSize(const Size &nMaxSize)
{
	maxSize = nMaxSize;

	if (hwnd == NIL) return Success();

	RECT	 windowRect;

	GetWindowRect(hwnd, &windowRect);

	SetMetrics(Point(windowRect.left, windowRect.top),
		   Size(Math::Min(Math::Round((windowRect.right  - windowRect.left - sizeModifier.cx) / fontSize), maxSize.cx),
			Math::Min(Math::Round((windowRect.bottom - windowRect.top  - sizeModifier.cy) / fontSize), maxSize.cy)));

	return Success();
}

S::Int S::GUI::WindowGDI::Show()
{
	if (hwnd == NIL) return Success();

	if (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId()) ShowWindowAsync(hwnd, SW_SHOW);
	else								 ShowWindow(hwnd, SW_SHOW);

	return Success();
}

S::Int S::GUI::WindowGDI::Hide()
{
	if (hwnd == NIL) return Success();

	if (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId()) ShowWindowAsync(hwnd, SW_HIDE);
	else								 ShowWindow(hwnd, SW_HIDE);

	return Success();
}

S::Int S::GUI::WindowGDI::SetMetrics(const Point &nPos, const Size &nSize)
{
	if (hwnd == NIL) return Success();

	SetWindowPos(hwnd, 0, nPos.x, nPos.y, Math::Round(nSize.cx * fontSize) + sizeModifier.cx, Math::Round(nSize.cy * fontSize) + sizeModifier.cy, SWP_NOZORDER | SWP_ASYNCWINDOWPOS);

	return Success();
}

S::Int S::GUI::WindowGDI::Minimize()
{
	if (hwnd == NIL) return Success();

	if (GetWindowThreadProcessId(hwnd, NIL) != GetCurrentThreadId()) ShowWindowAsync(hwnd, SW_MINIMIZE);
	else								 ShowWindow(hwnd, SW_MINIMIZE);

	return Success();
}

S::Int S::GUI::WindowGDI::Maximize()
{
	if (hwnd == NIL) return Success();

	Rect	 workArea = System::Screen::GetActiveScreenWorkArea();

	{
		RECT	 rect;

		GetWindowRect(hwnd, &rect);

		restoredRect = Rect(Point(rect.left, rect.top), (Size(rect.right - rect.left, rect.bottom - rect.top) - sizeModifier) / fontSize);
	}

	restoredWndStyle = GetWindowLong(hwnd, GWL_STYLE);

	SetWindowLong(hwnd, GWL_STYLE, restoredWndStyle ^ (WS_THICKFRAME | WS_SYSMENU));

	SetMetrics(Point(workArea.left - (frameSize.cx - 1), workArea.top - (frameSize.cy - 1)), (Size(workArea.GetWidth() + (2 * frameSize.cx - 2), workArea.GetHeight() + (2 * frameSize.cy - 2)) - sizeModifier) / fontSize);

	maximized = True;

	onMaximize.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Restore()
{
	if (hwnd == NIL) return Success();

	SetWindowLong(hwnd, GWL_STYLE, restoredWndStyle | WS_VISIBLE);

	SetMetrics(Point(restoredRect.left, restoredRect.top), Size((Int) Math::Max(minSize.cx, restoredRect.GetWidth()), (Int) Math::Max(minSize.cy, restoredRect.GetHeight())));

	maximized = False;

	onRestore.Emit();

	return Success();
}

S::Int S::GUI::WindowGDI::Raise()
{
	if (hwnd == NIL) return Success();

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return Success();
}

S::Int S::GUI::WindowGDI::SetProgressIndicator(Window::ProgressIndicatorState state, Float value)
{
	HRESULT	 hr = S_OK;

	if (taskbar == NIL) hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void **) &taskbar);

	if (hr != S_OK) return Error();

	switch (state)
	{
		case Window::ProgressIndicatorNone:
			taskbar->SetProgressState(hwnd, TBPF_NOPROGRESS);

			break;
		case Window::ProgressIndicatorNormal:
			taskbar->SetProgressState(hwnd, TBPF_NORMAL);

			if (value >= 0) taskbar->SetProgressValue(hwnd, Math::Round(value * 10.0), 1000);

			break;
		case Window::ProgressIndicatorPaused:
			taskbar->SetProgressState(hwnd, TBPF_PAUSED);

			if (value >= 0) taskbar->SetProgressValue(hwnd, Math::Round(value * 10.0), 1000);

			break;
	}

	return Success();
}

S::Void S::GUI::WindowGDI::SetCursor(Cursor *cursor, const Point &point)
{
	Window	*window = cursor->GetContainerWindow();
	HWND	 hwnd	= (HWND) window->GetSystemWindow();
	HIMC	 himc	= ImmGetContext(hwnd);

	/* Clear composition string.
	 */
	if (cursor != activeCursor) ImmSetCompositionString(himc, SCS_SETSTR, NIL, 0, NIL, 0);

	/* Set composition window information.
	 */
	COMPOSITIONFORM	 info;

	info.dwStyle	    = CFS_DEFAULT;

	ImmSetCompositionWindow(himc, &info);

	info.dwStyle	    = CFS_POINT;
	info.ptCurrentPos.x = point.x - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXPADDEDBORDER);
	info.ptCurrentPos.y = point.y - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CXPADDEDBORDER);

	if (window->GetFlags() & WF_NORESIZE)
	{
		info.ptCurrentPos.x = point.x - GetSystemMetrics(SM_CXDLGFRAME) - 1;
		info.ptCurrentPos.y = point.y - GetSystemMetrics(SM_CYDLGFRAME) - 1;
	}

	ImmSetCompositionWindow(himc, &info);

	/* Set font information.
	 */
	LOGFONT		 lFont;
	const Font	&font	    = cursor->GetFont();
	int		 nameLength = Math::Min(font.GetName().Length() + 1, LF_FACESIZE);

	lFont.lfHeight		= -Math::Round(font.GetSize() * 128.0 / Surface().GetSurfaceDPI());
	lFont.lfWidth		= 0;
	lFont.lfEscapement	= 0;
	lFont.lfOrientation	= 0;
	lFont.lfWeight		= (font.GetWeight() >= Font::Bold) ? FW_BOLD : FW_NORMAL;
	lFont.lfItalic		= (font.GetStyle() & Font::Italic)    ? true : false;
	lFont.lfUnderline	= (font.GetStyle() & Font::Underline) ? true : false;
	lFont.lfStrikeOut	= (font.GetStyle() & Font::StrikeOut) ? true : false;
	lFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lFont.lfCharSet		= DEFAULT_CHARSET;
	lFont.lfQuality		= DEFAULT_QUALITY;
	lFont.lfPitchAndFamily	= DEFAULT_PITCH | FF_ROMAN;

	memcpy(lFont.lfFaceName, (wchar_t *) font.GetName(), sizeof(wchar_t) * nameLength);

	lFont.lfFaceName[nameLength - 1] = 0;

	ImmSetCompositionFont(himc, &lFont);

	ImmReleaseContext(hwnd, himc);

	activeCursor = cursor;
}

S::Void S::GUI::WindowGDI::RemoveCursor(Cursor *cursor)
{
	if (activeCursor != cursor) return;

	HWND	 hwnd = (HWND) cursor->GetContainerWindow()->GetSystemWindow();
	HIMC	 himc = ImmGetContext(hwnd);

	/* Clear composition string.
	 */
	ImmSetCompositionString(himc, SCS_SETSTR, NIL, 0, NIL, 0);

	/* Revert to default composition window.
	 */
	COMPOSITIONFORM	 info;

	info.dwStyle = CFS_DEFAULT;

	ImmSetCompositionWindow(himc, &info);
	ImmSetCompositionWindow(himc, &info);

	ImmReleaseContext(hwnd, himc);

	activeCursor = NIL;
}
