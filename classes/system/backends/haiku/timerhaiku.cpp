 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timer.h>
#include <smooth/system/event.h>
#include <smooth/system/backends/haiku/timerhaiku.h>
#include <smooth/system/backends/haiku/eventhaiku.h>
#include <smooth/init.h>

S::Int	 addTimerHaikuInitTmp = S::AddInitFunction(&S::System::TimerHaiku::Initialize);
S::Int	 addTimerHaikuFreeTmp = S::AddFreeFunction(&S::System::TimerHaiku::Free);

S::System::TimerBackend *CreateTimerHaiku(S::System::Timer *timer)
{
	return new S::System::TimerHaiku(timer);
}

S::Int	 timerHaikuTmp = S::System::TimerBackend::SetBackend(&CreateTimerHaiku);

S::System::TimerHaiku::TimerHaiku(Timer *iTimer) : TimerBackend(iTimer)
{
	type = TIMER_HAIKU;

	timer = NIL;
}

S::System::TimerHaiku::~TimerHaiku()
{
	Stop();
}

S::Int S::System::TimerHaiku::Initialize()
{
	EventProcessor::allowTimerInterrupts.Connect(&AllowTimerInterrupts);
	EventProcessor::denyTimerInterrupts.Connect(&DenyTimerInterrupts);

	return Success();
}

S::Int S::System::TimerHaiku::Free()
{
	EventProcessor::allowTimerInterrupts.Disconnect(&AllowTimerInterrupts);
	EventProcessor::denyTimerInterrupts.Disconnect(&DenyTimerInterrupts);

	return Success();
}

S::Int S::System::TimerHaiku::AllowTimerInterrupts()
{
	/* Unblock SIGALRM so timeouts can be processed.
	 */
	sigset_t	 ss;

	sigemptyset(&ss);
	sigaddset(&ss, SIGALRM);

	pthread_sigmask(SIG_UNBLOCK, &ss, NIL);

	return Success();
}

S::Int S::System::TimerHaiku::DenyTimerInterrupts()
{
	/* Block SIGALRM again.
	 */
	sigset_t	 ss;

	sigemptyset(&ss);
	sigaddset(&ss, SIGALRM);

	pthread_sigmask(SIG_BLOCK, &ss, NIL);

	return Success();
}

S::Int S::System::TimerHaiku::Start(Int nInterval)
{
	if (timer != NIL) return Error();

	if (nInterval <= 0) nInterval = 1;

	timer = new timer_t;

	/* Set handler function.
	 */
	struct sigaction	 sa;

	memset(&sa, 0, sizeof(sa));

	sa.sa_sigaction = TimerProc;
	sa.sa_flags	= SA_SIGINFO;

	sigaction(SIGALRM, &sa, NIL);

	/* Create timer.
	 */
	struct sigevent		 se;

	memset(&se, 0, sizeof(se));

	se.sigev_notify		 = SIGEV_SIGNAL;
	se.sigev_signo		 = SIGALRM;
	se.sigev_value.sival_int = thisTimer->GetHandle();

	timer_create(CLOCK_REALTIME, &se, timer);

	/* Start timer.
	 */
	struct itimerspec	 ts;

	ts.it_value.tv_sec     = nInterval / 1000;
	ts.it_value.tv_nsec    = nInterval % 1000 * 1000000;
	ts.it_interval.tv_sec  = nInterval / 1000;
	ts.it_interval.tv_nsec = nInterval % 1000 * 1000000;

	timer_settime(*timer, 0, &ts, NIL);

	return Success();
}

S::Int S::System::TimerHaiku::Stop()
{
	if (timer == NIL) return Error();

	timer_delete(*timer);

	delete timer;

	timer = NIL;

	return Success();
}

S::Int S::System::TimerHaiku::GetID() const
{
	if (timer == NIL) return Error();

	return (Int64) timer;
}

void S::System::TimerHaiku::TimerProc(int sig, siginfo_t *info, void *text)
{
	System::EventHaiku::EnqueueMessage(0, BMessage(B_TIMER), B_TIMER, info->si_value.sival_int, 0);
}
