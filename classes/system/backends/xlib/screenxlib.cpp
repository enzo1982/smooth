 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/xlib/screenxlib.h>
#include <smooth/backends/xlib/backendxlib.h>

using namespace X11;

S::System::ScreenBackend *CreateScreenXLib()
{
	return new S::System::ScreenXLib();
}

S::Int	 screenXLibTmp = S::System::ScreenBackend::SetBackend(&CreateScreenXLib);

S::System::ScreenXLib::ScreenXLib()
{
	type = SCREEN_XLIB;

	display	= Backends::BackendXLib::GetDisplay();
}

S::System::ScreenXLib::~ScreenXLib()
{
}

S::GUI::Rect S::System::ScreenXLib::GetActiveScreenMetrics()
{
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}

S::GUI::Rect S::System::ScreenXLib::GetActiveScreenWorkArea()
{
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}

S::GUI::Rect S::System::ScreenXLib::GetVirtualScreenMetrics()
{
	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}
