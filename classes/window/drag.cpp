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
#include <smooth/drag.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/loop.h>
#include <smooth/input.h>

const S::Int	 S::GUI::DragControl::classID = S::Object::RequestClassID();

S::GUI::DragControl::DragControl()
{
	type				= classID;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(Window::classID);
}

S::GUI::DragControl::~DragControl()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::DragControl::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Point	 mPos;
	Size	 cpwp;
	Rect	 wRect;
	Rect	 rect;
	Int	 retVal = Success;
	Int	 leftButton;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			wnd->Process(SM_LOOSEFOCUS, 0, 0);

			if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
			else					leftButton = VK_LBUTTON;

			wRect = wnd->GetWindowRect();

			cpwp.cx = Input::MouseX() - wRect.left;
			cpwp.cy = Input::MouseY() - wRect.top;

			if (!wnd->IsMaximized())
			{
				do
				{
					MSG	 msg;

					if (peekLoop > 0)
					{
						if (Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
						else				PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);
					}
					else
					{
						if (Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
						else				GetMessageA(&msg, NIL, 0, 0);
					}

					TranslateMessage(&msg);

					if (Setup::enableUnicode)	DispatchMessageW(&msg);
					else				DispatchMessageA(&msg);

					if (peekLoop > 0)
					{
						if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
						else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
					}

					SetWindowPos((HWND) wnd->GetSystemWindow(), 0, Input::MouseX() - cpwp.cx, Input::MouseY() - cpwp.cy, wnd->GetObjectProperties()->size.cx, wnd->GetObjectProperties()->size.cy, 0);

					Rect	 wndRect = wnd->GetWindowRect();

					wnd->GetObjectProperties()->pos.x	= wndRect.left;
					wnd->GetObjectProperties()->pos.y	= wndRect.top;
					wnd->GetObjectProperties()->size.cx	= wndRect.right - wndRect.left;
					wnd->GetObjectProperties()->size.cy	= wndRect.bottom - wndRect.top;
				}
				while (GetAsyncKeyState(leftButton) != 0);
			}

			retVal = Break;

			break;
	}

	return retVal;
}
