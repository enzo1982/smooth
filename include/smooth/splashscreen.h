 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SPLASHSCREEN_
#define _H_OBJSMOOTH_SPLASHSCREEN_

namespace smooth
{
	class SplashScreenApp;
	class Timer;

	namespace GUI
	{
		class Window;
	};
};

#include "definitions.h"
#include "application.h"

namespace smooth
{
	class SplashScreenApp : public Application
	{
		private:
			static Int	 nOfSplashScreens;
			Int		 time;
			HBITMAP		 bitmap;
			GUI::Window	*splashscreen;
			Timer		*timer;
		public:
					 SplashScreenApp(HBITMAP, Int);
					~SplashScreenApp();
			Void		 SplashPaintProc();
			Bool		 SplashKillProc();
			Int		 ShowSplashScreen();
			Void		 TimerProc();
	};
};

#endif
