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
};

#include "object.h"

namespace smooth
{
	Void WINAPI TimerProc(HWND, unsigned int, unsigned int, unsigned long);

	class SMOOTHAPI Timer : public Object
	{
		friend Void TimerProc(HWND, unsigned int, unsigned int, unsigned long);
		private:
			Int			 timerid;
		public:
			static const Int	 classID;

						 Timer();
						~Timer();

			Int			 Start(Int);
			Int			 Stop();

			Int			 GetID();
		signals:
			Signal0<Void>		 onInterval;
	};
};

#endif
