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
	class Timer;

	namespace GUI
	{
		namespace Dialogs
		{
			class SplashScreenApp;
		};

		class Window;
	};
};

#include "dialog.h"
#include "../graphics/bitmap.h"

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			class SplashScreen : public Application
			{
				private:
					static Int	 nOfSplashScreens;

					Int		 time;
					Bitmap		 bitmap;
					Window		*splashscreen;
					Timer		*timer;
				slots:
					Void		 SplashPaintProc();
					Bool		 SplashKillProc();

					Void		 TimerProc();
				public:
							 SplashScreen(const Bitmap &, Int);
							~SplashScreen();

					Int		 ShowDialog();
			};
		};
	};
};

#endif
