 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/cocoa/screencocoa.h>

S::System::ScreenBackend *CreateScreenCocoa()
{
	return new S::System::ScreenCocoa();
}

S::Int	 screenCocoaTmp = S::System::ScreenBackend::SetBackend(&CreateScreenCocoa);

S::System::ScreenCocoa::ScreenCocoa()
{
	type = SCREEN_COCOA;
}

S::System::ScreenCocoa::~ScreenCocoa()
{
}

S::GUI::Rect S::System::ScreenCocoa::GetActiveScreenMetrics()
{
	NSScreen	*screen = [NSScreen mainScreen];
	NSRect		 frame	= [screen frame];

	return GUI::Rect(GUI::Point(frame.origin.x, frame.origin.y), GUI::Size(frame.size.width, frame.size.height));
}

S::GUI::Rect S::System::ScreenCocoa::GetActiveScreenWorkArea()
{
	NSScreen	*screen = [NSScreen mainScreen];
	NSRect		 frame	= [screen frame];

	return GUI::Rect(GUI::Point(frame.origin.x, frame.origin.y), GUI::Size(frame.size.width, frame.size.height));
}

S::GUI::Rect S::System::ScreenCocoa::GetVirtualScreenMetrics()
{
	NSScreen	*screen = [NSScreen mainScreen];
	NSRect		 frame	= [screen frame];

	return GUI::Rect(GUI::Point(frame.origin.x, frame.origin.y), GUI::Size(frame.size.width, frame.size.height));
}
