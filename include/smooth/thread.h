 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREAD_
#define _H_OBJSMOOTH_THREAD_

#define SThread SMOOTHThread

class SMOOTHThread;

#include "object.h"

const SMOOTHInt	 THREAD_CREATED		= 0;
const SMOOTHInt	 THREAD_RUNNING		= 1;
const SMOOTHInt	 THREAD_STOPPED		= 2;
const SMOOTHInt	 THREAD_PAUSED		= 3;
const SMOOTHInt	 THREAD_STARTME		= 4;
const SMOOTHInt	 THREAD_STOPPED_SELF	= 5;

const SMOOTHInt	 THREAD_KILLFLAG_KILL	= 0;
const SMOOTHInt	 THREAD_KILLFLAG_WAIT	= 1;

const SMOOTHInt	 THREAD_WAITFLAG_WAIT	= 0;
const SMOOTHInt	 THREAD_WAITFLAG_START	= 1;

class SMOOTHAPI SMOOTHThread : public SMOOTHObject
{
	friend SMOOTHVoid SMOOTHThreadProcCaller(SMOOTHThread *);
	private:
		SMOOTHInt		 killflag;
		SMOOTHInt		 waitflag;
		SMOOTHInt		 status;

		LiSAThread		*thread;

		SMOOTHThreadProcMember;
		SMOOTHVoid		 (*altproc)(SMOOTHThread *);
	public:
		static SMOOTHInt	 counter;

					 SMOOTHThread(SMOOTHThreadProcParam, SMOOTHVoid *);
					 SMOOTHThread(SMOOTHVoid (*)(SMOOTHThread *));
					~SMOOTHThread();

		SMOOTHInt		 GetStatus();

		SMOOTHInt		 Start();
		SMOOTHInt		 Stop();

		SMOOTHVoid		 SetWaitFlag(SMOOTHInt);
		SMOOTHInt		 GetWaitFlag();
		SMOOTHVoid		 SetKillFlag(SMOOTHInt);
		SMOOTHInt		 GetKillFlag();
};

SMOOTHVoid SMOOTHThreadProcCaller(SMOOTHThread *);

SMOOTHVAR SMOOTHInt OBJ_THREAD;

#endif
