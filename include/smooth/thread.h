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
	const Int	 THREAD_WAITFLAG_START	= 1;

	class SMOOTHAPI Thread : public Object
	{
		friend Void ThreadProcCaller(Thread *);
		private:
			Int		 killflag;
			Int		 waitflag;
			Int		 status;

			LiSAThread	*thread;

			ThreadProcMember;
			Void		 (*altproc)(Thread *);
		public:
			static Int	 counter;

					 Thread(ThreadProcParam, Void *);
					 Thread(Void (*)(Thread *));
					~Thread();

			Int		 GetStatus();

			Int		 Start();
			Int		 Stop();

			Void		 SetWaitFlag(Int);
			Int		 GetWaitFlag();
			Void		 SetKillFlag(Int);
			Int		 GetKillFlag();
	};

	Void ThreadProcCaller(Thread *);

	SMOOTHVAR Int OBJ_THREAD;
};

#endif
