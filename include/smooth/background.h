 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BACKGROUND_APPLICATION_
#define _H_OBJSMOOTH_BACKGROUND_APPLICATION_

#define SBackgroundApplication SMOOTHBackgroundApplication

class SMOOTHBackgroundApplication;
class SMOOTHWindow;
class SMOOTHTimer;

#include "application.h"

class SMOOTHBackgroundApplication : public SMOOTHApplication
{
	private:
		SMOOTHWindow	*backgroundWindow;
		SMOOTHTimer	*backgroundTimer;

		SMOOTHVoid	 TimerProc();
	public:
				 SMOOTHBackgroundApplication();
				~SMOOTHBackgroundApplication();
};

extern SMOOTHBackgroundApplication	*backgroundApplication;

#endif
