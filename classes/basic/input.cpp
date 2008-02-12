 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/input.h>

#ifndef __WIN32__
#  include <smooth/backends/xlib/backendxlib.h>
#endif

S::GUI::Point	 S::Input::mousePosition = S::GUI::Point();

S::Input::Input()
{
}

S::Input::Input(const Input &)
{
}

const S::GUI::Point &S::Input::GetMousePosition()
{
#ifdef __WIN32__
	POINT	 point;

	GetCursorPos(&point);

	mousePosition.x = point.x;
	mousePosition.y = point.y;
#else
	Display	*display = NIL;

	for (Int i = 0; i < Backends::Backend::GetNOfBackends(); i++)
	{
		if (Backends::Backend::GetNthBackend(i)->GetBackendType() == Backends::BACKEND_XLIB) display = ((Backends::BackendXLib *) Backends::Backend::GetNthBackend(i))->GetDisplay();
	}

	XEvent	 e;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &e.xbutton.root, &e.xbutton.window, &e.xbutton.x_root, &e.xbutton.y_root, &e.xbutton.x, &e.xbutton.y, &e.xbutton.state);

	mousePosition.x = e.xbutton.x;
	mousePosition.y = e.xbutton.y;
#endif

	return mousePosition;
}
