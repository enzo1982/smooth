 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TOOLTIP_
#define _H_OBJSMOOTH_TOOLTIP_

#define STooltip SMOOTHTooltip

class SMOOTHTooltip;
class SMOOTHTimer;
class SMOOTHToolWindow;

#include "object.h"

class SMOOTHAPI SMOOTHTooltip : public SMOOTHObject
{
	private:
		SMOOTHToolWindow	*toolWindow;
		SMOOTHTimer		*timer;

		SMOOTHVoid		 TimerProc();
	protected:
		SMOOTHInt		 timeOut;
	public:
					 SMOOTHTooltip();
					~SMOOTHTooltip();

		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 Show();
		SMOOTHInt		 Hide();

		SMOOTHInt		 SetTimeout(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_TOOLTIP;

#endif
