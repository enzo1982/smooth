 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BEATCLOCK_TIME
#define H_BEATCLOCK_TIME

#include <smooth.h>

using namespace smooth;

class Time
{
	public:
		static Int	 BeatsToSeconds(Int);
		static Int	 SecondsToBeats(Int);

		static String	 SecondsToTimeString(Int);
		static Int	 TimeStringToSeconds(String);

		static Int	 GetDayOfWeek(Int, Int, Int);
		static Bool	 OutOfMonth(Int, Int, Int);
};

#endif
