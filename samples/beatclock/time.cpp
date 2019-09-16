 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "time.h"

Int Time::BeatsToSeconds(Int beats)
{
	return (Int) (beats * 86.4);
}

Int Time::SecondsToBeats(Int seconds)
{
	return (Int) (seconds / 86.4);
}

String Time::SecondsToTimeString(Int seconds)
{
	String	 rstring;

	if (((Int) (seconds / 3600)) < 10) rstring.Append("0");

	rstring.Append(String::FromInt(seconds / 3600));
	rstring.Append(":");

	if (((Int) ((seconds % 3600) / 60)) < 10) rstring.Append("0");

	rstring.Append(String::FromInt((seconds % 3600) / 60));

	return rstring;
}

Int Time::TimeStringToSeconds(String time)
{
	const Array<String>	&parts = time.Explode(":");

	return parts.GetNth(0).ToInt() * 3600 + parts.GetNth(1).ToInt() * 60;
}

Int Time::GetDayOfWeek(Int day, Int month, Int year)
{
	month += 10;
	year   = (month - 24) / 12 + year;
	day    = (((13 * (month - (month / 13) * 12) - 1) / 5 + day + 77 + 5 * (year - (year / 100) * 100) / 4 + year / 400 - (year / 100) * 2) % 7);

	return day;
}

Bool Time::OutOfMonth(Int day, Int month, Int year)
{
	if	(day > 31									  ) return True;
	else if (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)		  ) return True;
	else if (day > 29 && month == 2								  ) return True;
	else if (day > 28 && month == 2 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))) return True;

	return False;
}
