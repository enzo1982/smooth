 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TIME_
#define _H_OBJSMOOTH_TIME_

#define STime SMOOTHTime

class SMOOTHTime;

#include "definitions.h"

class SMOOTHAPI SMOOTHTime
{
	private:
		SMOOTHInt	 mseconds;
	public:
				 SMOOTHTime();
				~SMOOTHTime();

		SMOOTHBool	 SetHMS(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 GetHour();
		SMOOTHInt	 GetMinute();
		SMOOTHInt	 GetSecond();
		SMOOTHInt	 GetMSecond();

		SMOOTHBool	 SetHour(SMOOTHInt);
		SMOOTHBool	 SetMinute(SMOOTHInt);
		SMOOTHBool	 SetSecond(SMOOTHInt);
		SMOOTHBool	 SetMSecond(SMOOTHInt);
};

#endif
