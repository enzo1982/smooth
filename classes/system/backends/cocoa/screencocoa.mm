 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/cocoa/screencocoa.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>

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
	NSArray		*screens     = [NSScreen screens];
	NSScreen	*screen	     = nil;
	NSScreen	*firstScreen = [screens objectAtIndex: 0];

	if ([screens count] == 1) screen = firstScreen;
	else			  screen = ScreenFromPoint(Input::Pointer::GetPosition());

	NSRect		 frame	     = [screen frame];
	NSRect		 firstFrame  = [firstScreen frame];

	return GUI::Rect(GUI::Point(frame.origin.x, firstFrame.size.height - frame.origin.y - frame.size.height), GUI::Size(frame.size.width, frame.size.height));
}

S::GUI::Rect S::System::ScreenCocoa::GetActiveScreenWorkArea()
{
	NSArray		*screens     = [NSScreen screens];
	NSScreen	*screen	     = nil;
	NSScreen	*firstScreen = [screens objectAtIndex: 0];

	if ([screens count] == 1) screen = firstScreen;
	else			  screen = ScreenFromPoint(Input::Pointer::GetPosition());

	NSRect		 frame	     = [screen visibleFrame];
	NSRect		 firstFrame  = [firstScreen frame];

	return GUI::Rect(GUI::Point(frame.origin.x, firstFrame.size.height - frame.origin.y - frame.size.height), GUI::Size(frame.size.width, frame.size.height));
}

S::GUI::Rect S::System::ScreenCocoa::GetVirtualScreenMetrics()
{
	NSArray		*screens     = [NSScreen screens];
	NSScreen	*firstScreen = [screens objectAtIndex: 0];
	NSRect		 firstFrame  = [firstScreen frame];

	GUI::Rect	 metrics;

	for (UnsignedInt i = 0; i < [screens count]; i++)
	{
		NSScreen	*screen = [screens objectAtIndex: i];
		NSRect		 frame	= [screen frame];

		metrics.left   = Math::Min(metrics.left, frame.origin.x);
		metrics.top    = Math::Min(metrics.top, firstFrame.size.height - frame.origin.y - frame.size.height);
		metrics.right  = Math::Max(metrics.right, frame.origin.x + frame.size.width);
		metrics.bottom = Math::Max(metrics.bottom, firstFrame.size.height - frame.origin.y);
	}

	return metrics;
}

NSScreen *S::System::ScreenCocoa::ScreenFromPoint(const GUI::Point &point)
{
	NSArray		*screens     = [NSScreen screens];
	NSScreen	*firstScreen = [screens objectAtIndex: 0];
	NSRect		 firstFrame  = [firstScreen frame];

	for (UnsignedInt i = 0; i < [screens count]; i++)
	{
		NSScreen	*screen = [screens objectAtIndex: i];
		NSRect		 frame	= [screen frame];

		if (point.x >=				frame.origin.x			   &&
		    point.x <				frame.origin.x + frame.size.width  &&
		    point.y >= firstFrame.size.height - frame.origin.y - frame.size.height &&
		    point.y <  firstFrame.size.height - frame.origin.y			  ) return screen;
	}

	return nil;
}
