 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
#include "graphics/bitmap.h"

namespace smooth
{
	class SplashScreenApp : public Application
	{
		private:
			static Int	 nOfSplashScreens;
			Int		 time;
			GUI::Bitmap	 bitmap;
			GUI::Window	*splashscreen;
			Timer		*timer;
		public:
					 SplashScreenApp(const GUI::Bitmap &, Int);
					~SplashScreenApp();
			Void		 SplashPaintProc();
			Bool		 SplashKillProc();
			Int		 ShowSplashScreen();
			Void		 TimerProc();
	};

	SMOOTHAPI Int	 SplashScreen(const GUI::Bitmap &, Int);
};

#endif
