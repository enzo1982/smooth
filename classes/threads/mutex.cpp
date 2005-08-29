 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/mutex.h>
#include <smooth/threads/mutexbackend.h>

const S::Int	 S::Threads::Mutex::classID = S::Object::RequestClassID();

S::Threads::Mutex::Mutex(Void *iMutex)
{
	backend = MutexBackend::CreateBackendInstance(iMutex);

	type = classID;
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
