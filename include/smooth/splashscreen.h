 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SPLASHSCREEN_
#define _H_OBJSMOOTH_SPLASHSCREEN_

#define SSplashScreeApp SMOOTHsplashScreenApp

class SMOOTHSplashScreenApp;
class SMOOTHWindow;
class SMOOTHTimer;

#include "definitions.h"
#include "application.h"

class SMOOTHSplashScreenApp : public SMOOTHApplication
{
	private:
		static int	 nOfSplashScreens;
		int		 time;
		HBITMAP		 bitmap;
		SMOOTHWindow	*splashscreen;
		SMOOTHTimer	*timer;
	public:
				 SMOOTHSplashScreenApp(HBITMAP, int);
				~SMOOTHSplashScreenApp();
		void		 SplashPaintProc();
		bool		 SplashKillProc();
		int		 ShowSplashScreen();
		void		 TimerProc();
};

#endif
