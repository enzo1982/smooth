 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/system.h>
#include <smooth/system/backends/xlib/eventxlib.h>
#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/backends/xlib/backendxlib.h>

using namespace X11;

S::System::EventBackend *CreateEventXLib()
{
	return new S::System::EventXLib();
}

S::Int	 eventXLibTmp = S::System::EventBackend::SetBackend(&CreateEventXLib);

S::Int	 S::System::EventXLib::nested = 0;

S::System::EventXLib::EventXLib()
{
	type = EVENT_XLIB;

	display	= Backends::BackendXLib::GetDisplay();

	/* Deny timer interrupts outside of any event loops
	 * to prevent interruption of sensitive code.
	 */
	if (!nested++) EventProcessor::denyTimerInterrupts.Call();
}

S::System::EventXLib::~EventXLib()
{
	/* Allow timer interrupts again before leaving the
	 * outermost event processor.
	 */
	if (!--nested) EventProcessor::allowTimerInterrupts.Call();
}

/* Predicate function to find MotionNotify events.
 */
int FindMotionEvent(Display *d, XEvent *e, XPointer arg)
{
	return (e->type == MotionNotify && e->xany.window == (X11::Window) arg);
}

/* Predicate function to find ConfigureNotify events.
 */
int FindConfigureEvent(Display *d, XEvent *e, XPointer arg)
{
	return (e->type == ConfigureNotify && e->xany.window == (X11::Window) arg);
}

S::Int S::System::EventXLib::ProcessNextEvent()
{
	/* Emulate a timeout of ~100ms by trying to find a message
	 * 10 times while sleeping for 10ms between trying.
	 */
	for (Int i = 0; i < 10; i++)
	{
		/* XEventsQueued with QueuedAfterFlush sometimes hanged,
		 * so we flush manually and use QueuedAfterReading then.
		 */
		XFlush(display);

		if (XEventsQueued(display, QueuedAfterReading))
		{
			XEvent		 e;

			XNextEvent(display, &e);

			if (XFilterEvent(&e, None)) break;

			/* Process only the most recent MotionNotify and ConfigureNotify events.
			 */
			if (e.type == MotionNotify)    while (XCheckIfEvent(display, &e, FindMotionEvent,    (XPointer) e.xany.window)) { }
			if (e.type == ConfigureNotify) while (XCheckIfEvent(display, &e, FindConfigureEvent, (XPointer) e.xany.window)) { }

			/* Find window and dispatch event.
			 */
			GUI::WindowXLib	*window = GUI::WindowXLib::GetWindowBackend(e.xany.window);

			if (window != NIL) window->ProcessSystemMessages(&e);

			break;
		}

		/* Allow timeouts to be processed.
		 */
		EventProcessor::allowTimerInterrupts.Call();

		/* Now sleep for 10ms.
		 */
		System::System::Sleep(10);

		/* Prevent timeouts from being processed.
		 */
		EventProcessor::denyTimerInterrupts.Call();
	}

	return Success();
}
