 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window/window.h>
#include <smooth/window/drag.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/loop.h>
#include <smooth/input.h>
#include <smooth/system/event.h>

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
				System::EventProcessor	*event = new System::EventProcessor();

				do
				{
					if (wnd->IsMaximized()) break;

					if (peekLoop > 0)	event->ProcessNextEvent(False);
					else			event->ProcessNextEvent(True);

					SetWindowPos((HWND) wnd->GetSystemWindow(), 0, Input::MouseX() - cpwp.cx, Input::MouseY() - cpwp.cy, wnd->GetObjectProperties()->size.cx, wnd->GetObjectProperties()->size.cy, 0);

					Rect	 wndRect = wnd->GetWindowRect();

					wnd->GetObjectProperties()->pos = Point(wndRect.left, wndRect.top);
					wnd->GetObjectProperties()->size = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
				}
				while (GetAsyncKeyState(leftButton) != 0);

				delete event;
			}

			retVal = Break;

			break;
	}

	return retVal;
}
