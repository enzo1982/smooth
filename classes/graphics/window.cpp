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

LRESULT CALLBACK S::GUI::Window::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
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

S::GUI::Window::Window(String title, Void *iWindow)
{
#ifdef __WIN32__
	backend = new WindowGDI(iWindow);
#else
	backend = new WindowBackend(iWindow);
#endif

	self = this;

	containerType = classID;

	possibleContainers.AddEntry(Application::classID);

	exstyle		= 0;
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

	minSize.cx = Math::Round(160 * Setup::FontSize);
	minSize.cy = METRIC_TITLEBARHEIGHT + 5;

	doQuit.Connect(&Window::DummyExitProc, this);

	onCreate.SetParentObject(this);
	onPaint.SetParentObject(this);
	onResize.SetParentObject(this);
	onPeek.SetParentObject(this);
	onEvent.SetParentObject(this);

	style		= WS_THICKFRAME | WS_SYSMENU | WS_POPUP;
	className	= String(title).Append(String::FromInt(System::RequestGUID()));

	if (Setup::enableUnicode)	sysIcon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
	else				sysIcon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(32512), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);

	hwnd = NIL;
}

S::GUI::Window::~Window()
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

S::Bool S::GUI::Window::DummyExitProc()
{
	return True;
}

S::Int S::GUI::Window::SetMetrics(Point newPos, Size newSize)
{
	updateRect = Rect();

	objectProperties->pos.x		= Math::Round(newPos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(newPos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(newSize.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(newSize.cy * Setup::FontSize);

	if (created) SetWindowPos(hwnd, 0, Math::Round(newPos.x * Setup::FontSize), Math::Round(newPos.y * Setup::FontSize), Math::Round(newSize.cx * Setup::FontSize), Math::Round(newSize.cy * Setup::FontSize), SWP_NOZORDER);

	return Success;
}

S::Void S::GUI::Window::SetStyle(Int s)
{
	if (!created) style = style | s;
}

S::Void S::GUI::Window::SetExStyle(Int es)
{
	if (!created) exstyle = exstyle | es;
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

S::Int S::GUI::Window::SetApplicationIcon(char *newIcon)
{
	return SetApplicationIcon((wchar_t *) newIcon);
}

S::Int S::GUI::Window::SetApplicationIcon(wchar_t *newIcon)
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

		if (Setup::enableUnicode)	SetWindowTextW(hwnd, title);
		else				SetWindowTextA(hwnd, title);
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

S::Int S::GUI::Window::Show()
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

	initshow	= True;
	visible		= True;

	onShow.Emit();

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

	onHide.Emit();

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

S::Int S::GUI::Window::Restore()
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

S::Rect S::GUI::Window::GetClientRect()
{
	return innerOffset;
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
	minSize = newMinSize;

	return Success;
}

S::Int S::GUI::Window::SetMaximumSize(Size newMaxSize)
{
	maxSize = newMaxSize;

	return Success;
}

S::Bool S::GUI::Window::Create()
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

			drawSurface = new Surface(windowDC);
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

S::Int S::GUI::Window::Stay()
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

S::Int S::GUI::Window::Close()
{
	if (hwnd == NIL) return Error;

	Process(SM_LOOSEFOCUS, 0, 0);

	if (Setup::enableUnicode)	::PostMessageW(hwnd, WM_CLOSE, 0, 0);
	else				::PostMessageA(hwnd, WM_CLOSE, 0, 0);

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
							if (object->handle > handle && (object->GetFlags() & WF_MODAL)) SetActiveWindow(((Window *) object)->hwnd);
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

S::GUI::Window *S::GUI::Window::GetWindow(HWND hwnd)
{
	if (hwnd == NIL) return NIL;

	for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
	{
		Object	*window = mainObjectManager->GetNthObject(i);

		if (window != NIL)
		{
			if (window->GetObjectType() == Window::classID || window->GetObjectType() == MDIWindow::classID || window->GetObjectType() == ToolWindow::classID)
			{
				if (((Window *) window)->hwnd == hwnd) return (Window *) window;
			}
		}
	}

	return NIL;
}
