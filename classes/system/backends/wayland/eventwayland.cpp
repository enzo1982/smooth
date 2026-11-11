 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/backends/platform.h>
#include <smooth/system/system.h>
#include <smooth/threads/thread.h>
#include <smooth/system/backends/wayland/eventwayland.h>
#include <smooth/gui/window/backends/wayland/windowwayland.h>
#include <smooth/backends/wayland/backendwayland.h>

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

S::System::EventBackend *CreateEventWayland()
{
	return new S::System::EventWayland();
}

static S::Int Register_eventWaylandTmp() { return S::System::EventBackend::SetBackend(&CreateEventWayland); }

S::Int	 eventWaylandTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_WAYLAND, &Register_eventWaylandTmp);

S::Int	 S::System::EventWayland::nested = 0;

S::System::EventWayland::EventWayland()
{
	type = EVENT_WAYLAND;

	/* Deny timer interrupts outside of any event loops
	 * to prevent interruption of sensitive code.
	 */
	if (!nested++) EventProcessor::denyTimerInterrupts.Call();
}

S::System::EventWayland::~EventWayland()
{
	/* Allow timer interrupts again before leaving the
	 * outermost event processor.
	 */
	if (!--nested) EventProcessor::allowTimerInterrupts.Call();
}

S::Bool S::System::EventWayland::ProcessNextEvent()
{
	wl_display	*display = Backends::BackendWayland::GetDisplay();

	if (display == NIL) return False;

	/* Process events on main thread only.
	 */
	if (Threads::Thread::GetCurrentThreadID() != Threads::MainThreadID)
	{
		S::System::System::Sleep(1);

		return True;
	}

	/* Emulate a timeout of ~100ms by trying to find a message
	 * 10 times while waiting up to 10ms between trying.
	 */
	for (Int i = 0; i < 10; i++)
	{
		/* Dispatch pending events.
		 */
		Int	 dispatched = Backends::BackendWayland::Dispatch();

		if (dispatched < 0)
		{
			fprintf(stderr, "Error: Lost connection to Wayland display.\n");

			exit(EXIT_FAILURE);
		}

		Bool	 processed = GUI::WindowWayland::ProcessPendingEvents();

		if (dispatched > 0 || processed) break;

		/* Allow timeouts to be processed while waiting for events.
		 */
		EventProcessor::allowTimerInterrupts.Call();

		pollfd	 pfd = { wl_display_get_fd(display), POLLIN, 0 };

		poll(&pfd, 1, 10);

		/* Prevent timeouts from being processed.
		 */
		EventProcessor::denyTimerInterrupts.Call();
	}

	return True;
}
