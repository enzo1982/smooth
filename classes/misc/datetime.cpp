 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/datetime.h>

S::DateTime::DateTime()
{
	days		= 0;
	mseconds	= 0;
}

S::DateTime::~DateTime()
{
}

S::Bool S::DateTime::SetYMD(Int year, Int month, Int day)
{
	if (year < 0)			return False;
	if (month <= 0 || month > 12)	return False;
	if (day <= 0 || day > 31)	return False;

	SetYear(year);
	SetMonth(month);
	SetDay(day);

	return True;
}

S::Bool S::DateTime::SetHMS(Int hour, Int min, Int sec)
{
	if (hour < 0 || hour >= 24)	return False;
	if (min < 0 || min >= 60)	return False;
	if (sec < 0 || sec >= 60)	return False;

	SetHour(hour);
	SetMinute(min);
	SetSecond(sec);
	SetMSecond(0);

	return True;
}

S::Int S::DateTime::GetYear() const
{
	return (Int) ((Float) days / (12 * 31));
}

S::Int S::DateTime::GetMonth() const
{
	return (Int) ((Float) (days % (12 * 31)) / 31);
}

S::Int S::DateTime::GetDay() const
{
	return days % (12 * 31) % 31;
}

S::Bool S::DateTime::SetYear(Int year)
{
	if (year < 0) return False;

	days = days - 12 * 31 * GetYear() + 12 * 31 * year;

	return True;
}

S::Bool S::DateTime::SetMonth(Int month)
{
	if (month <= 0 || month > 12) return False;

	days = days - 31 * GetMonth() + 31 * month;

	return True;
}

S::Bool S::DateTime::SetDay(Int day)
{
	if (day <= 0 || day > 31) return False;

	days = days - GetDay() + day;

	return True;
}

S::Int S::DateTime::GetHour() const
{
	return (Int) ((Float) mseconds / (60 * 60 * 1000));
}

S::Int S::DateTime::GetMinute() const
{
	return (Int) ((Float) (mseconds % (60 * 60 * 1000)) / (60 * 1000));
}

S::Int S::DateTime::GetSecond() const
{
	return (Int) ((Float) (mseconds % (60 * 60 * 1000) % (60 * 1000)) / 1000);
}

S::Int S::DateTime::GetMSecond() const
{
	return mseconds % (60 * 60 * 1000) % (60 * 1000) % 1000;
}

S::Bool S::DateTime::SetHour(Int hour)
{
	if (hour < 0 || hour >= 24) return False;

	mseconds = mseconds - 60 * 60 * 1000 * GetHour() + 60 * 60 * 1000 * hour;

	return True;
}

S::Bool S::DateTime::SetMinute(Int min)
{
	if (min < 0 || min >= 60) return False;

	mseconds = mseconds - 60 * 1000 * GetMinute() + 60 * 1000 * min;

	return True;
}

S::Bool S::DateTime::SetSecond(Int sec)
{
	if (sec < 0 || sec >= 60) return False;

	mseconds = mseconds - 1000 * GetSecond() + 1000 * sec;

	return True;
}

S::Bool S::DateTime::SetMSecond(Int msec)
{
	if (msec < 0 || msec >= 1000) return False;

	mseconds = mseconds - GetMSecond() + msec;

	return True;
}
