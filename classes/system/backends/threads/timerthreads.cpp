 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
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

using namespace smooth::Threads;

S::System::TimerBackend *CreateTimerThreads(S::System::Timer *timer)
{
	return new S::System::TimerThreads(timer);
}

S::Int	 timerThreadsTmp = S::System::TimerBackend::SetBackend(&CreateTimerThreads);

S::System::TimerThreads::TimerThreads(Timer *iTimer) : TimerBackend(iTimer)
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

S::Int S::System::TimerThreads::Start(Int nInterval)
{
	if (nInterval <= 0) nInterval = 1;

	/* Cancel previous cancellation if
	 * the thread is still running.
	 */
	if (cancel && thread->IsCurrentThread())
	{
		cancel	 = False;
		interval = nInterval;
		timeout	 = System::System::Clock() + interval;

		return Success();
	}

	if (thread != NIL) return Error();

	interval = nInterval;
	timeout	 = System::System::Clock() + interval;
	thread	 = NonBlocking1<Int>(&TimerThreads::TimerProc, this).Call(thisTimer->GetHandle());

	return Success();
}

S::Int S::System::TimerThreads::Stop()
{
	if (thread == NIL) return Error();

	/* Check if the thread is already gone.
	 */
	if (thread->GetStatus() != THREAD_RUNNING) return Success();

	/* Order the thread to cancel.
	 */
	if (!thread->IsCurrentThread())
	{
		cancel = True;

		while (cancel == True) System::System::Sleep(0);
	}
	else
	{
		cancel = True;
	}

	return Success();
}

S::Int S::System::TimerThreads::GetID() const
{
	if (thread == NIL) return Error();

	return (Int64) thread & 0xFFFFFFFF;
}

S::Int S::System::TimerThreads::TimerProc(Int handle)
{
	while (!cancel)
	{
		if (System::System::Clock() >= timeout)
		{
			Timer	*timer = (Timer *) Object::GetObject(handle, Timer::classID);

			if (timer != NIL) timer->onInterval.Emit();
			else		  cancel = True;

			while (timeout <= System::System::Clock()) timeout += interval;
		}

		System::System::Sleep(1);
	}

	/* Reset thread variable and cancellation state.
	 */
	cancel = False;
	thread = NIL;

	return Success();
}
