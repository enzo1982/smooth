 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TIMER_
#define _H_OBJSMOOTH_TIMER_

#define STimer SMOOTHTimer

class SMOOTHTimer;
class SMOOTHWindow;

#include "object.h"

class SMOOTHAPI SMOOTHTimer : public SMOOTHObject
{
	private:
		SMOOTHWindow	*timerwnd;
		SMOOTHInt	 timerid;
	public:
				 SMOOTHTimer();
				~SMOOTHTimer();

		SMOOTHInt	 Start(SMOOTHInt);
		SMOOTHInt	 Stop();

		SMOOTHInt	 GetID();

		SMOOTHInt	 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 Show();
		SMOOTHInt	 Hide();
};

SMOOTHVAR SMOOTHInt OBJ_TIMER;

#endif
