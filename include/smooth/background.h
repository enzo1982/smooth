 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BACKGROUND_APPLICATION_
#define _H_OBJSMOOTH_BACKGROUND_APPLICATION_

namespace smooth
{
	class BackgroundApplication;
	class Timer;

	namespace GUI
	{
		class Window;
	};
};

#include "application.h"

namespace smooth
{
	class BackgroundApplication : public Application
	{
		private:
			GUI::Window	*backgroundWindow;
			Timer		*backgroundTimer;

			Void		 TimerProc();
		public:
					 BackgroundApplication();
					~BackgroundApplication();
	};

	extern BackgroundApplication	*backgroundApplication;
};

#endif
