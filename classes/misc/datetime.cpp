 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/datetime.h>

#include <time.h>

#ifdef __WIN32__
#	define time_t	 __time64_t

#	define time	 _time64
#	define mktime	 _mktime64
#	define localtime _localtime64
#endif

S::DateTime::DateTime(Void *iTime)
{
	value = new time_t;

	if (iTime != NIL) memcpy(value, iTime, sizeof(time_t));
}

S::DateTime::~DateTime()
{
	delete (time_t *) time;
}

S::DateTime S::DateTime::Current()
{
	time_t	 t = time(NIL);

	return DateTime(&t);
}

S::Bool S::DateTime::SetYMD(Int year, Int month, Int day)
{
	if (year  <  1900	       ) return False;
	if (month <=	0 || month > 12) return False;
	if (day	  <=	0 || day   > 31) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_year = year - 1900;
	lt->tm_mon  = month - 1;
	lt->tm_mday = day;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Bool S::DateTime::SetHMS(Int hour, Int min, Int sec)
{
	if (hour < 0 || hour >= 24) return False;
	if (min	 < 0 || min  >= 60) return False;
	if (sec	 < 0 || sec  >= 60) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_hour = hour;
	lt->tm_min  = min;
	lt->tm_sec  = sec;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Int S::DateTime::GetYear() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_year + 1900;
}

S::Int S::DateTime::GetMonth() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_mon + 1;
}

S::Int S::DateTime::GetDay() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_mday;
}

S::Bool S::DateTime::SetYear(Int year)
{
	if (year < 0) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_year = year - 1900;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Bool S::DateTime::SetMonth(Int month)
{
	if (month <= 0 || month > 12) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_mon  = month - 1;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Bool S::DateTime::SetDay(Int day)
{
	if (day <= 0 || day > 31) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_mday = day;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Int S::DateTime::GetHour() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_hour;
}

S::Int S::DateTime::GetMinute() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_min;
}

S::Int S::DateTime::GetSecond() const
{
	tm	*lt = localtime((time_t *) value);

	return lt->tm_sec;
}

S::Bool S::DateTime::SetHour(Int hour)
{
	if (hour < 0 || hour >= 24) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_hour = hour;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Bool S::DateTime::SetMinute(Int min)
{
	if (min < 0 || min >= 60) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_min  = min;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}

S::Bool S::DateTime::SetSecond(Int sec)
{
	if (sec < 0 || sec >= 60) return False;

	tm	*lt = localtime((time_t *) value);

	lt->tm_sec  = sec;

	time_t	 t  = mktime(lt);

	memcpy(value, &t, sizeof(time_t));

	return True;
}
