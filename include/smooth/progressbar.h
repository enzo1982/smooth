 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_PROGRESSBAR_
#define _H_OBJSMOOTH_PROGRESSBAR_

#define SProgressbar SMOOTHProgressbar

class SMOOTHProgressbar;

#include "object.h"

const SMOOTHInt PB_NOTEXT	= 0;
const SMOOTHInt PB_VALUE	= 1;
const SMOOTHInt PB_PERCENT	= 2;

class SMOOTHAPI SMOOTHProgressbar : public SMOOTHObject
{
	protected:
		SMOOTHInt		 value;

		SMOOTHInt		 startValue;
		SMOOTHInt		 endValue;

		SMOOTHInt		 textFlag;
	public:
					 SMOOTHProgressbar(SMOOTHPoint, SMOOTHSize, SMOOTHInt, SMOOTHInt, SMOOTHInt, SMOOTHInt, SMOOTHInt);
					~SMOOTHProgressbar();

		virtual SMOOTHInt	 Paint(SMOOTHInt);

		SMOOTHInt		 SetValue(SMOOTHInt);
		SMOOTHInt		 GetValue();
};

SMOOTHVAR SMOOTHInt OBJ_PROGRESSBAR;

#endif
