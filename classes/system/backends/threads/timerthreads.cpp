 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timer.h>
#include <smooth/system/system.h>
#include <smooth/system/backends/threads/timerthreads.h>
#include <smooth/templates/nonblocking.h>

#ifdef __WIN32__
#	include <time.h>
#else
#	include <sys/time.h>
#endif

unsigned int mtime()
{
#ifdef __WIN32__
	return clock();
#else
	timeval	 tv;

	gettimeofday(&tv, NIL);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

using namespace smooth::Threads;

S::System::TimerBackend *CreateTimerThreads()
{
	return new S::System::TimerThreads();
}

S::Int	 timerThreadsTmp = S::System::TimerBackend::SetBackend(&CreateTimerThreads);

S::System::TimerThreads::TimerThreads()
{
	type	 = TIMER_THREADS;

	thread	 = NIL;
	interval = 0;
	timeout	 = 0;
	cancel	 = False;
}

S::System::TimerThreads::~TimerThreads()
{
	Stop();
}

S::Int S::System::TimerThreads::Start(UnsignedInt nInterval)
{
	/* Cancel previous cancellation if
	 * the thread is still running.
	 */
	if (cancel)
	{
		cancel	 = False;
		interval = nInterval;
		timeout	 = mtime() + interval;

		return Success();
	}

	if (thread != NIL) return Error();

	interval = nInterval;
	timeout	 = mtime() + interval;
	thread	 = NonBlocking0<>(&TimerThreads::TimerProc, this).Call();

	return Success();
}

S::Int S::System::TimerThreads::Stop()
{
	if (thread == NIL) return Error();

	/* Order the thread to cancel.
	 */
	cancel = True;

	return Success();
}

S::Int S::System::TimerThreads::GetID() const
{
	if (thread == NIL) return Error();

	return (Int) thread;
}

S::Int S::System::TimerThreads::TimerProc()
{
	Signal0<Void>	 *onInterval = NIL;

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object *object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Timer::classID)
			{
				if (((Timer *) object)->GetID() == (Int) thread)
				{
					onInterval = &(((Timer *) object)->onInterval);
				}
			}
		}
	}

	while (!cancel)
	{
		if (mtime() >= timeout)
		{
			timeout = mtime() + interval;

			onInterval->Emit();

			while (timeout <= mtime()) timeout += interval;
		}

		System::System::Sleep(1);
	}

	/* Reset thread variable and cancellation state.
	 */
	cancel = False;
	thread = NIL;

	return Success();
}
