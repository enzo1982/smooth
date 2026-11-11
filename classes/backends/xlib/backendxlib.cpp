 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/backends/freedesktop/desktopsettings.h>
#include <smooth/backends/platform.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

using namespace X11;

S::Backends::Backend *CreateBackendXLib()
{
	return new S::Backends::BackendXLib();
}

static S::Bool ProbeXLib()
{
	return getenv("DISPLAY") != NIL;
}

S::Int	 backendXLibTmp = S::Backends::Backend::AddBackend(&CreateBackendXLib);
S::Int	 backendXLibProbeTmp = S::Backends::Platform::AddProbe(S::Backends::PLATFORM_X11, 1, &ProbeXLib);

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
	if (!Platform::IsSelected(PLATFORM_X11)) return Success();

	XInitThreads();

	/* Open display.
	 */
	const char	*displayId = getenv("DISPLAY");

	if (displayId != NIL)
	{
		display	= XOpenDisplay(NIL);

		if (display == NIL)
		{
			fprintf(stderr, "Error: Unable to open display at %s.\n", displayId);

			return Error();
		}

		/* Init the desktop settings helper.
		 */
		DesktopSettings::InitDependencies("x11");

		/* Get default colors.
		 */
		DesktopSettings::UpdateColors();

		/* Set locale and open input method.
		 */
		setlocale(LC_ALL, "");

		XSetLocaleModifiers("");

		im = XOpenIM(display, NIL, NIL, NIL);
	}

	return Success();
}

S::Int S::Backends::BackendXLib::Deinit()
{
	if (!Platform::IsSelected(PLATFORM_X11)) return Success();

	if (im	    != NIL) XCloseIM(im);
	if (display != NIL) XCloseDisplay(display);

	DesktopSettings::BlockTimerInterrupts(False);

	return Success();
}

Display *S::Backends::BackendXLib::GetDisplay()
{
	return display;
}

XIM S::Backends::BackendXLib::GetIM()
{
	return im;
}
