 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/xlib/windowxlib.h>
#include <smooth/gui/window/window.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>
#include <smooth/loop.h>
#include <smooth/backends/xlib/backendxlib.h>

S::GUI::WindowBackend *CreateWindowXLib()
{
	return new S::GUI::WindowXLib();
}

S::Int	 windowXLibTmp = S::GUI::WindowBackend::AddBackend(&CreateWindowXLib);

S::GUI::WindowXLib::WindowXLib(Void *iWindow)
{
	type	= WINDOW_XLIB;

	wnd	= NIL;
	display	= NIL;

	for (Int i = 0; i < Backends::Backend::GetNOfBackends(); i++)
	{
		if (Backends::Backend::GetNthBackend(i)->GetBackendType() == Backends::BACKEND_XLIB) display = ((Backends::BackendXLib *) Backends::Backend::GetNthBackend(i))->GetDisplay();
	}
}

S::GUI::WindowXLib::~WindowXLib()
{
}

S::Void *S::GUI::WindowXLib::GetSystemWindow()
{
	return (Void *) wnd;
}

S::Int S::GUI::WindowXLib::Open(String title, Point pos, Size size, Int flags)
{
	XSetWindowAttributes	 attributes;

	attributes.background_pixel	= BlackPixel(display, 0);
	attributes.border_pixel		= BlackPixel(display, 0);
	attributes.override_redirect	= True;

	wnd = XCreateWindow(display, RootWindow(display, 0), pos.x, pos.y, size.cx, size.cy, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWBorderPixel | CWOverrideRedirect, &attributes);

	if (wnd != NIL)
	{
		XMapWindow(display, wnd);

		XFlush(display);

		drawSurface = new Surface((Void *) wnd);
		drawSurface->SetSize(size);

		return Success;
	}

	return Failure;
}

S::Int S::GUI::WindowXLib::Close()
{
	delete drawSurface;

	drawSurface = nullSurface;

	XDestroyWindow(display, wnd);

	return Success;
}
