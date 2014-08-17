 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/cocoa/timercocoa.h>
#include <smooth/system/timer.h>

S::System::TimerBackend *CreateTimerCocoa(S::System::Timer *timer)
{
	return new S::System::TimerCocoa(timer);
}

S::Int	 timerCocoaTmp = S::System::TimerBackend::SetBackend(&CreateTimerCocoa);

@interface TimerCocoa : NSObject
	- (void) timerInvoked: (NSTimer *) theTimer;
@end

S::System::TimerCocoa::TimerCocoa(Timer *iTimer) : TimerBackend(iTimer)
{
	type = TIMER_COCOA;

	timer	 = nil;
	receiver = [[::TimerCocoa alloc] init];
}

S::System::TimerCocoa::~TimerCocoa()
{
	Stop();

	[(::TimerCocoa *) receiver release];
}

S::Int S::System::TimerCocoa::Start(Int interval)
{
	if (timer != nil) return Error();

	timer = [NSTimer timerWithTimeInterval: interval / 1000.0
					target: (id) receiver
				      selector: @selector(timerInvoked:)
				      userInfo: [NSNumber numberWithInt: thisTimer->GetHandle()]
				       repeats: YES];

	[[NSRunLoop mainRunLoop] addTimer: timer
				  forMode: NSDefaultRunLoopMode];

	return Success();
}

S::Int S::System::TimerCocoa::Stop()
{
	if (timer == nil) return Error();

	[timer invalidate];

	timer = nil;

	return Success();
}

S::Int S::System::TimerCocoa::GetID() const
{
	if (timer == nil) return Error();

	return (Int64) timer;
}

@implementation TimerCocoa
	- (void) timerInvoked: (NSTimer *) theTimer
	{
		S::System::Timer	*timer = (S::System::Timer *) S::Object::GetObject([[theTimer userInfo] intValue], S::System::Timer::classID);

		if (timer != NIL) timer->onInterval.Emit();
	}
@end
