 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DRAGCONTROL_
#define __OBJSMOOTH_DRAGCONTROL_

#include <smooth/window.h>
#include <smooth/drag.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/stk.h>
#include <smooth/loop.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_DRAGCONTROL = SMOOTH::RequestObjectID();

SMOOTHDragControl::SMOOTHDragControl()
{
	type				= OBJ_DRAGCONTROL;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHDragControl::~SMOOTHDragControl()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHDragControl::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 mPos;
	SMOOTHPoint	 m;
	SMOOTHSize	 cpwp;
	SMOOTHRect	 wRect;
	SMOOTHRect	 rect;
	SMOOTHRect	 updateRect;
	SMOOTHRect	 wndRect;
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHInt	 leftButton;
	HDC		 dc = NIL;

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

			retVal = SMOOTH::Break;

			break;
	}

	return retVal;
}

#endif
