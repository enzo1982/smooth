 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREAD_
#define _H_OBJSMOOTH_THREAD_

namespace smooth
{
	namespace Threads
	{
		class Thread;
		class ThreadBackend;
	};
};

#include "../object.h"
#include "../callbacks.h"

namespace smooth
{
	namespace Threads
	{
		const Int	 THREAD_CREATED		= 0;
		const Int	 THREAD_RUNNING		= 1;
		const Int	 THREAD_STOPPED		= 2;
		const Int	 THREAD_STARTME		= 3;
		const Int	 THREAD_STOPPED_SELF	= 4;

		const Int	 THREAD_KILLFLAG_KILL	= 0;
		const Int	 THREAD_KILLFLAG_WAIT	= 1;

		const Int	 THREAD_WAITFLAG_WAIT	= 0;
		const Int	 THREAD_WAITFLAG_START	= 2;

		class SMOOTHAPI Thread : public Object
		{
			private:
				Int			 status;

				ThreadBackend		*backend;
			public:
				static const Int	 classID;

							 Thread(Void * = NIL);
							~Thread();

				Int			 GetStatus();
				Int			 GetThreadID();

				Int			 Start();
				Int			 Stop();

				static Int		 GetNOfRunningThreads();
			callbacks:
				Callback1<Int, Thread *> threadMain;
		};
	};
};

#endif
