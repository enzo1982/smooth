 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DATE_
#define _H_OBJSMOOTH_DATE_

#define SDate SMOOTHDate

class SMOOTHDate;

#include "definitions.h"

class SMOOTHAPI SMOOTHDate
{
	private:
		SMOOTHInt	 days;
	public:
				 SMOOTHDate();
				~SMOOTHDate();

		SMOOTHBool	 SetYMD(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 GetYear();
		SMOOTHInt	 GetMonth();
		SMOOTHInt	 GetDay();

		SMOOTHBool	 SetYear(SMOOTHInt);
		SMOOTHBool	 SetMonth(SMOOTHInt);
		SMOOTHBool	 SetDay(SMOOTHInt);
};

#endif
