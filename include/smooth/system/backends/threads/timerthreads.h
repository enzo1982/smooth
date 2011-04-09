 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TIMERTHREADS
#define H_OBJSMOOTH_TIMERTHREADS

namespace smooth
{
	namespace System
	{
		class TimerThreads;
	};
};

#include "../timerbackend.h"
#include "../../../threads/thread.h"

namespace smooth
{
	namespace System
	{
		/* A threads based timer backend for use on systems
		 * that provide only limited or no timer support.
		 */
		const Short	 TIMER_THREADS = 3;

		class TimerThreads : public TimerBackend
		{
			private:
				Threads::Thread	*thread;

				Int		 interval;
				UnsignedInt64	 timeout;

				volatile Bool	 cancel;

				Int		 TimerProc(Int);
			public:
						 TimerThreads(Timer *);
						~TimerThreads();

				Int		 Start(Int);
				Int		 Stop();

				Int		 GetID() const;
		};
	};
};

#endif