 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/cocoa/eventcocoa.h>

#include <smooth/gui/window/window.h>
#include <smooth/system/system.h>

S::System::EventBackend *CreateEventCocoa()
{
	return new S::System::EventCocoa();
}

S::Int	 eventCocoaTmp = S::System::EventBackend::SetBackend(&CreateEventCocoa);

S::Bool	 S::System::EventCocoa::quitRequested = False;

S::System::EventCocoa::EventCocoa()
{
	type = EVENT_COCOA;
}

S::System::EventCocoa::~EventCocoa()
{
}

S::Bool S::System::EventCocoa::ProcessNextEvent()
{
	/* Run loop in main thread only.
	 */
	if (![NSThread isMainThread])
	{
		S::System::System::Sleep(1);

		return True;
	}

	/* Try to find active modal windows.
	 */
	GUI::Window	*smoothWindow = NIL;
	NSWindow	*modalWindow  = nil;

	for (Int n = GUI::Window::GetNOfWindows() - 1; n >= 0; n--)
	{
		GUI::Window	*window = GUI::Window::GetNthWindow(n);

		if (window->IsVisible() && window->GetFlags() & GUI::WF_MODAL)
		{
			smoothWindow = window;
			modalWindow  = (NSWindow *) window->GetSystemWindow();

			break;
		}
	}

	/* The first loop invocation must not be modal
	 * to prevent problems on macOS 10.9 Mavericks.
	 */
	static Bool	 ranNonModal = False;

	if (!ranNonModal)
	{
		ranNonModal = True;

		[NSApp run];

		return True;
	}

	/* Run modal or regular loop.
	 */
	if (modalWindow != nil && [modalWindow isVisible])
	{
		smoothWindow->EnterProtectedRegion();

		[NSApp runModalForWindow: modalWindow];

		smoothWindow->LeaveProtectedRegion();
	}
	else
	{
		[NSApp run];
	}

	/* Ask windows to close if quit has been requested.
	 */
	if (quitRequested)
	{
		quitRequested = False;

		for (Int n = GUI::Window::GetNOfWindows() - 1; n >= 0; n--)
		{
			GUI::Window	*window	= GUI::Window::GetNthWindow(n);

			if (window->IsVisible()) window->Close();
		}
	}

	return True;
}

S::Void S::System::EventCocoa::RequestApplicationQuit()
{
	quitRequested = True;
}
