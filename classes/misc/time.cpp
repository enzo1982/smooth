 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TIME_
#define __OBJSMOOTH_TIME_

#include <smooth/time.h>
#include <smooth/stk.h>

SMOOTHTime::SMOOTHTime()
{
	mseconds = 0;
}

SMOOTHTime::~SMOOTHTime()
{
}

SMOOTHBool SMOOTHTime::SetHMS(SMOOTHInt hour, SMOOTHInt min, SMOOTHInt sec)
{
	if (hour < 0 || hour >= 24)	return SMOOTH::False;
	if (min < 0 || min >= 60)	return SMOOTH::False;
	if (sec < 0 || sec >= 60)	return SMOOTH::False;

	SetHour(hour);
	SetMinute(min);
	SetSecond(sec);
	SetMSecond(0);

	return SMOOTH::True;
}

SMOOTHInt SMOOTHTime::GetHour()
{
	return (SMOOTHInt) ((SMOOTHFloat) mseconds / (60 * 60 * 1000));
}

SMOOTHInt SMOOTHTime::GetMinute()
{
	return (SMOOTHInt) ((SMOOTHFloat) (mseconds % (60 * 60 * 1000)) / (60 * 1000));
}

SMOOTHInt SMOOTHTime::GetSecond()
{
	return (SMOOTHInt) ((SMOOTHFloat) (mseconds % (60 * 60 * 1000) % (60 * 1000)) / 1000);
}

SMOOTHInt SMOOTHTime::GetMSecond()
{
	return mseconds % (60 * 60 * 1000) % (60 * 1000) % 1000;
}

SMOOTHBool SMOOTHTime::SetHour(SMOOTHInt hour)
{
	if (hour < 0 || hour >= 24) return SMOOTH::False;

	mseconds = mseconds - 60 * 60 * 1000 * GetHour() + 60 * 60 * 1000 * hour;

	return SMOOTH::True;
}

SMOOTHBool SMOOTHTime::SetMinute(SMOOTHInt min)
{
	if (min < 0 || min >= 60) return SMOOTH::False;

	mseconds = mseconds - 60 * 1000 * GetMinute() + 60 * 1000 * min;

	return SMOOTH::True;
}

SMOOTHBool SMOOTHTime::SetSecond(SMOOTHInt sec)
{
	if (sec < 0 || sec >= 60) return SMOOTH::False;

	mseconds = mseconds - 1000 * GetSecond() + 1000 * sec;

	return SMOOTH::True;
}

SMOOTHBool SMOOTHTime::SetMSecond(SMOOTHInt msec)
{
	if (msec < 0 || msec >= 1000) return SMOOTH::False;

	mseconds = mseconds - GetMSecond() + msec;

	return SMOOTH::True;
}

#endif
