 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/xlib/backendxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

using namespace X11;

S::Backends::Backend *CreateBackendXLib()
{
	return new S::Backends::BackendXLib();
}

S::Int	 backendXLibTmp = S::Backends::Backend::AddBackend(&CreateBackendXLib);

Display	*S::Backends::BackendXLib::display = NIL;
XIM	 S::Backends::BackendXLib::im	   = NIL;

S::Backends::BackendXLib::BackendXLib()
{
	type = BACKEND_XLIB;
}

S::Backends::BackendXLib::~BackendXLib()
{
}

S::Int S::Backends::BackendXLib::Init()
{
	XInitThreads();

	/* Set default font size.
	 */
	Setup::FontSize = 1.00;

	return Success();
}

S::Int S::Backends::BackendXLib::Deinit()
{
	if (im	    != NIL) XCloseIM(im);
	if (display != NIL) XCloseDisplay(display);

	return Success();
}

Display *S::Backends::BackendXLib::GetDisplay()
{
	if (display != NIL) return display;

	/* Open display.
	 */
	display	= XOpenDisplay(NIL);

	return display;
}

XIM S::Backends::BackendXLib::GetIM()
{
	if (im != NIL) return im;

	/* Check if display is open.
	 */
	if (display == NIL && GetDisplay() == NIL)
	{
		const char	*display = getenv("DISPLAY");

		fprintf(stderr, "Error: Unable to open display at %s.\n", (display != NIL && display[0] != 0) ? display : "<none>");

		exit(-1);
	}

	/* Set locale and open input method.
	 */
	setlocale(LC_ALL, "");

	XSetLocaleModifiers("");

	im = XOpenIM(display, NIL, NIL, NIL);

	return im;
}
