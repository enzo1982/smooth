 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TIMER_
#define _H_OBJSMOOTH_TIMER_

namespace smooth
{
	class Timer;

	namespace GUI
	{
		class Window;
	};
};

#include "object.h"

namespace smooth
{
	class SMOOTHAPI Timer : public Object
	{
		private:
			GUI::Window	*timerwnd;
			Int		 timerid;
		public:
					 Timer();
					~Timer();

			Int		 Start(Int);
			Int		 Stop();

			Int		 GetID();

			Int		 Process(Int, Int, Int);

			Int		 Show();
			Int		 Hide();
		signals:
			Signal0<>	 onInterval;
	};

	SMOOTHVAR Int OBJ_TIMER;
};

#endif
