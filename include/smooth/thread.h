 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREAD_
#define _H_OBJSMOOTH_THREAD_

namespace smooth
{
	class Thread;
};

#include "object.h"

namespace smooth
{
	const Int	 THREAD_CREATED		= 0;
	const Int	 THREAD_RUNNING		= 1;
	const Int	 THREAD_STOPPED		= 2;
	const Int	 THREAD_PAUSED		= 3;
	const Int	 THREAD_STARTME		= 4;
	const Int	 THREAD_STOPPED_SELF	= 5;

	const Int	 THREAD_KILLFLAG_KILL	= 0;
	const Int	 THREAD_KILLFLAG_WAIT	= 1;

	const Int	 THREAD_WAITFLAG_WAIT	= 0;
	const Int	 THREAD_WAITFLAG_START	= 2;

	class SMOOTHAPI Thread : public Object
	{
		private:
			Int			 status;

			LiSAThread		*thread;
			Int			 threadID;
		public:
			static Int		 counter;

						 Thread();
						~Thread();

			Int			 GetStatus();
			Int			 GetThreadID();

			Int			 Start();
			Int			 Stop();
		callbacks:
			Callback1<Int, Thread *> threadMain;
	};

	SMOOTHVAR Int OBJ_THREAD;
};

#endif
