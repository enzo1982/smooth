 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/backends/xlib/eventxlib.h>
#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/backends/xlib/backendxlib.h>

S::System::EventBackend *CreateEventXLib()
{
	return new S::System::EventXLib();
}

S::Int	 eventXLibTmp = S::System::EventBackend::SetBackend(&CreateEventXLib);

S::System::EventXLib::EventXLib()
{
	type = EVENT_XLIB;

	display	= NIL;

	for (Int i = 0; i < Backends::Backend::GetNOfBackends(); i++)
	{
		if (Backends::Backend::GetNthBackend(i)->GetBackendType() == Backends::BACKEND_XLIB) display = ((Backends::BackendXLib *) Backends::Backend::GetNthBackend(i))->GetDisplay();
	}
}

S::System::EventXLib::~EventXLib()
{
}

S::Int S::System::EventXLib::ProcessNextEvent(Bool block)
{
	XEvent		 e;

	XNextEvent(display, &e);

	GUI::WindowXLib	*window = NIL;

	switch (e.type)
	{
		case CreateNotify:
			window = GUI::WindowXLib::GetWindowBackend(e.xcreatewindow.window);
			break;
		case DestroyNotify:
			window = GUI::WindowXLib::GetWindowBackend(e.xdestroywindow.window);
			break;
		case MapNotify:
			window = GUI::WindowXLib::GetWindowBackend(e.xmap.window);
			break;
		case UnmapNotify:
			window = GUI::WindowXLib::GetWindowBackend(e.xunmap.window);
			break;
	}

	if (window != NIL) window->ProcessSystemMessages(&e);

	return Success();
}
