 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/threads/semaphore.h>
#include <smooth/gui/application/application.h>

#ifdef __WIN32__
#include <smooth/threads/win32/semaphorewin32.h>
#else
#include <smooth/threads/posix/semaphoreposix.h>
#endif

const S::Int	 S::Threads::Semaphore::classID = S::Object::RequestClassID();

S::Threads::Semaphore::Semaphore(Void *iSemaphore)
{
#ifdef __WIN32__
	backend = new SemaphoreWin32(iSemaphore);
#else
	backend = new SemaphorePOSIX(iSemaphore);
#endif

	type = classID;

	possibleContainers.AddEntry(GUI::Application::classID);
}

S::Threads::Semaphore::~Semaphore()
{
	delete backend;
}

S::Int S::Threads::Semaphore::GetSemaphoreType()
{
	return backend->GetSemaphoreType();
}

S::Void *S::Threads::Semaphore::GetSystemSemaphore()
{
	return backend->GetSystemSemaphore();
}

S::Int S::Threads::Semaphore::Wait()
{
	return backend->Wait();
}

S::Int S::Threads::Semaphore::Release()
{
	return backend->Release();
}
