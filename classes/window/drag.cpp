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
#include <smooth/drag.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>
#include <smooth/loop.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_DRAGCONTROL = S::Object::RequestObjectID();

S::GUI::DragControl::DragControl()
{
	type				= OBJ_DRAGCONTROL;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::GUI::DragControl::~DragControl()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::DragControl::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 mPos;
	Point	 m;
	Size	 cpwp;
	Rect	 wRect;
	Rect	 rect;
	Rect	 updateRect;
	Rect	 wndRect;
	Int	 retVal = Success;
	Int	 leftButton;
	HDC	 dc = NIL;

#ifdef __WIN32__
	MSG		 msg;
	PAINTSTRUCT	 ps;
#endif

	switch (message)
	{
		case SM_LBUTTONDOWN:
			wnd->Process(SM_LOOSEFOCUS, 0, 0);

#ifdef __WIN32__
			if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
			else					leftButton = VK_LBUTTON;

			{
				POINT	 mp;
				RECT	 wr;

				GetCursorPos(&mp);
				GetWindowRect(wnd->hwnd, &wr);

				mPos = mp;
				wRect = wr;
			}
#endif

			cpwp.cx = mPos.x - wRect.left;
			cpwp.cy = mPos.y - wRect.top;

			if (!wnd->IsMaximized())
			{
#ifdef __WIN32__
				do
				{
					if (peekLoop > 0)
					{
						if (SMOOTH::Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
						else					PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);
					}
					else
					{
						if (SMOOTH::Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
						else					GetMessageA(&msg, NIL, 0, 0);
					}

					if (msg.message == WM_PAINT)
					{
						RECT uRect;

						if (GetUpdateRect(msg.hwnd, &uRect, 0))
						{
							updateRect = uRect;

							dc = BeginPaint(msg.hwnd, &ps);

							{
								RECT r;

								GetClientRect(msg.hwnd, &r);

								rect = r;
							}

							EndPaint(msg.hwnd, &ps);

							SMOOTH::GetWindow(msg.hwnd)->SetUpdateRect(updateRect);
							SMOOTH::GetWindow(msg.hwnd)->Paint(SP_PAINT);
						}
						else
						{
							updateRect = uRect;
						}
					}
					else
					{
						TranslateMessage(&msg);

						if (SMOOTH::Setup::enableUnicode)	DispatchMessageW(&msg);
						else					DispatchMessageA(&msg);
					}

					if (peekLoop > 0)
					{
						if (SMOOTH::Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
						else					PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
					}

					{
						POINT	 mp;

						GetCursorPos(&mp);

						m = mp;
					}

					SetWindowPos(wnd->hwnd, 0, m.x-cpwp.cx, m.y-cpwp.cy, wnd->GetObjectProperties()->size.cx, wnd->GetObjectProperties()->size.cy, 0);

					{
						RECT wRect;

						GetWindowRect(wnd->hwnd, &wRect);

						wndRect = wRect;
					}

					wnd->GetObjectProperties()->pos.x	= wndRect.left;
					wnd->GetObjectProperties()->pos.y	= wndRect.top;
					wnd->GetObjectProperties()->size.cx	= wndRect.right - wndRect.left;
					wnd->GetObjectProperties()->size.cy	= wndRect.bottom - wndRect.top;
				}
				while (GetAsyncKeyState(leftButton) != 0);
#endif
			}

			retVal = Break;

			break;
	}

	return retVal;
}
