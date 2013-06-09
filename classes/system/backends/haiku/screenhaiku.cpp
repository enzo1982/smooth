 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/haiku/screenhaiku.h>

S::System::ScreenBackend *CreateScreenHaiku()
{
	return new S::System::ScreenHaiku();
}

S::Int	 screenHaikuTmp = S::System::ScreenBackend::SetBackend(&CreateScreenHaiku);

S::System::ScreenHaiku::ScreenHaiku()
{
	type = SCREEN_HAIKU;
}

S::System::ScreenHaiku::~ScreenHaiku()
{
}

S::GUI::Rect S::System::ScreenHaiku::GetActiveScreenMetrics()
{
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
}

S::GUI::Rect S::System::ScreenHaiku::GetActiveScreenWorkArea()
{
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
}

S::GUI::Rect S::System::ScreenHaiku::GetVirtualScreenMetrics()
{
	BScreen	 screen;
	BRect	 frame = screen.Frame();

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(frame.right - frame.left + 1, frame.bottom - frame.top - 1));
}
