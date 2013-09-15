 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

S::System::EventCocoa::EventCocoa()
{
	type = EVENT_COCOA;
}

S::System::EventCocoa::~EventCocoa()
{
}

S::Int S::System::EventCocoa::ProcessNextEvent()
{
	/* Run loop in main thread only.
	 */
	if ([NSThread isMainThread])
	{
		/* Try to find active modal windows.
		 */
		NSWindow	*modalWindow = nil;

		for (Int n = GUI::Window::GetNOfWindows() - 1; n >= 0; n--)
		{
			GUI::Window	*window = GUI::Window::GetNthWindow(n);

			if (window->IsVisible() && window->GetFlags() & GUI::WF_MODAL)
			{
				modalWindow = (NSWindow *) window->GetSystemWindow();

				break;
			}
		}

		/* Run modal or regular loop.
		 */
		if (modalWindow != nil && [modalWindow isVisible]) [NSApp runModalForWindow: modalWindow];
		else						   [NSApp run];
	}
	else
	{
		/* Just sleep when called from non-main thread.
		 */
		S::System::System::Sleep(10);
	}


	return Success();
}
