 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/mutexbackend.h>

S::Threads::MutexBackend::MutexBackend(Void *iMutex)
{
	type = MUTEX_NONE;
}

S::Threads::MutexBackend::~MutexBackend()
{
}

S::Int S::Threads::MutexBackend::GetMutexType()
{
	return type;
}

S::Void *S::Threads::MutexBackend::GetSystemMutex()
{
	return NIL;
}

S::Int S::Threads::MutexBackend::Lock()
{
	return Failure;
}

S::Int S::Threads::MutexBackend::Release()
{
	return Failure;
}
