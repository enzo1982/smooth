 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DATE_
#define __OBJSMOOTH_DATE_

#include <smooth/date.h>
#include <smooth/stk.h>

SMOOTHDate::SMOOTHDate()
{
	days = 0;
}

SMOOTHDate::~SMOOTHDate()
{
}

SMOOTHBool SMOOTHDate::SetYMD(SMOOTHInt year, SMOOTHInt month, SMOOTHInt day)
{
	if (year < 0)			return SMOOTH::False;
	if (month <= 0 || month > 12)	return SMOOTH::False;
	if (day <= 0 || day > 31)	return SMOOTH::False;

	SetYear(year);
	SetMonth(month);
	SetDay(day);

	return SMOOTH::True;
}

SMOOTHInt SMOOTHDate::GetYear()
{
	return (SMOOTHInt) ((SMOOTHFloat) days / (12 * 31));
}

SMOOTHInt SMOOTHDate::GetMonth()
{
	return (SMOOTHInt) ((SMOOTHFloat) (days % (12 * 31)) / 31);
}

SMOOTHInt SMOOTHDate::GetDay()
{
	return days % (12 * 31) % 31;
}

SMOOTHBool SMOOTHDate::SetYear(SMOOTHInt year)
{
	if (year < 0) return SMOOTH::False;

	days = days - 12 * 31 * GetYear() + 12 * 31 * year;

	return SMOOTH::True;
}

SMOOTHBool SMOOTHDate::SetMonth(SMOOTHInt month)
{
	if (month <= 0 || month > 12) return SMOOTH::False;

	days = days - 31 * GetMonth() + 31 * month;

	return SMOOTH::True;
}

SMOOTHBool SMOOTHDate::SetDay(SMOOTHInt day)
{
	if (day <= 0 || day > 31) return SMOOTH::False;

	days = days - GetDay() + day;

	return SMOOTH::True;
}

#endif
