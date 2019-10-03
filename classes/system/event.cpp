 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/backends/eventbackend.h>
#include <smooth/gui/application/application.h>

S::Callback0<S::Int>	 S::System::EventProcessor::allowTimerInterrupts;
S::Callback0<S::Int>	 S::System::EventProcessor::denyTimerInterrupts;

S::System::EventProcessor::EventProcessor()
{
	backend = EventBackend::CreateBackendInstance();
}

S::System::EventProcessor::~EventProcessor()
{
	delete backend;
}

S::Bool S::System::EventProcessor::ProcessNextEvent()
{
	Int	 suspendCount = GUI::Application::Lock::SuspendLock();
	Bool	 result	      = backend->ProcessNextEvent();

	GUI::Application::Lock::ResumeLock(suspendCount);

	return result;
}
