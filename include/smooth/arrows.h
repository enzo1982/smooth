 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARROWS_
#define _H_OBJSMOOTH_ARROWS_

#define SArrows SMOOTHArrows

class SMOOTHArrows;
class SMOOTHTimer;

#include "object.h"

class SMOOTHAPI SMOOTHArrows : public SMOOTHObject
{
	private:
		SMOOTHTimer		*timer;
		SMOOTHBool		 timerActive;
		SMOOTHInt		 timerCount;

		SMOOTHVoid		 TimerProc();
	protected:
		SMOOTHBool		 arrow1Clicked;
		SMOOTHBool		 arrow2Clicked;
		SMOOTHBool		 arrow1Checked;
		SMOOTHBool		 arrow2Checked;

		SMOOTHInt		*variable;

		SMOOTHInt		 startValue;
		SMOOTHInt 		 endValue;
	public:
					 SMOOTHArrows(SMOOTHPoint, SMOOTHSize, SMOOTHInt, SMOOTHInt *, SMOOTHInt, SMOOTHInt, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHArrows();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetRange(SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetValue(SMOOTHInt);
		SMOOTHInt		 GetValue();
};

SMOOTHVAR SMOOTHInt OBJ_ARROWS;

#endif
