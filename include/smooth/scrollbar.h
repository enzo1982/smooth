 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SCROLLBAR_
#define _H_OBJSMOOTH_SCROLLBAR_

#define SScrollbar SMOOTHScrollbar

class SMOOTHScrollbar;
class SMOOTHTimer;

#include "object.h"

class SMOOTHAPI SMOOTHScrollbar : public SMOOTHObject
{
	private:
		SMOOTHTimer		*timer;
		SMOOTHBool		 timerActive;
		SMOOTHInt		 timerCount;

		SMOOTHVoid		 TimerProc();
	protected:
		SMOOTHBool		 button1Clicked;
		SMOOTHBool		 button2Clicked;
		SMOOTHBool		 button3Clicked;
		SMOOTHBool		 button1Checked;
		SMOOTHBool		 button2Checked;
		SMOOTHBool		 button3Checked;

		SMOOTHInt		*variable;

		SMOOTHInt		 startValue;
		SMOOTHInt 		 endValue;

		SMOOTHInt		 mouseBias;
	public:
					 SMOOTHScrollbar(SMOOTHPoint, SMOOTHSize, SMOOTHInt, SMOOTHInt *, SMOOTHInt, SMOOTHInt, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHScrollbar();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetRange(SMOOTHInt, SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_SCROLLBAR;

#endif
