 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SLIDER_
#define _H_OBJSMOOTH_SLIDER_

#define SSlider SMOOTHSlider

class SMOOTHSlider;

#include "object.h"

class SMOOTHAPI SMOOTHSlider : public SMOOTHObject
{
	private:
		SMOOTHInt		 mouseBias;
		SMOOTHInt		 prevValue;
	protected:
		SMOOTHInt		*variable;

		SMOOTHInt		 startValue;
		SMOOTHInt		 endValue;
	public:
					 SMOOTHSlider(SMOOTHPoint, SMOOTHSize, SMOOTHInt, SMOOTHInt *, SMOOTHInt, SMOOTHInt, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHSlider();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetRange(SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetValue(SMOOTHInt);
		SMOOTHInt		 GetValue();
};

SMOOTHVAR SMOOTHInt OBJ_SLIDER;

#endif
