 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BACKGROUND_APPLICATION_
#define _H_OBJSMOOTH_BACKGROUND_APPLICATION_

namespace smooth
{
	namespace System
	{
		class Timer;
	};

	namespace GUI
	{
		class BackgroundApplication;
		class Window;
	};
};

#include "application.h"

namespace smooth
{
	namespace GUI
	{
		class BackgroundApplication : public Application
		{
			private:
				Window		*backgroundWindow;
				System::Timer	*backgroundTimer;

				Void		 TimerProc();
			public:
						 BackgroundApplication();
						~BackgroundApplication();
		};

		extern BackgroundApplication	*backgroundApplication;
	};
};

#endif
