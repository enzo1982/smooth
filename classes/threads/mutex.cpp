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
#include <smooth/threads/mutex.h>
#include <smooth/gui/application/application.h>

#ifdef __WIN32__
#include <smooth/threads/win32/mutexwin32.h>
#else
#include <smooth/threads/posix/mutexposix.h>
#endif

const S::Int	 S::Threads::Mutex::classID = S::Object::RequestClassID();

S::Threads::Mutex::Mutex(Void *iMutex)
{
#ifdef __WIN32__
	backend = new MutexWin32(iMutex);
#else
	backend = new MutexPOSIX(iMutex);
#endif

	type = classID;

	possibleContainers.AddEntry(GUI::Application::classID);
}

S::Threads::Mutex::~Mutex()
{
	delete backend;
}

S::Int S::Threads::Mutex::GetMutexType()
{
	return backend->GetMutexType();
}

S::Void *S::Threads::Mutex::GetSystemMutex()
{
	return backend->GetSystemMutex();
}

S::Int S::Threads::Mutex::Lock()
{
	return backend->Lock();
}

S::Int S::Threads::Mutex::Release()
{
	return backend->Release();
}
