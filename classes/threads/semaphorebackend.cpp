 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/semaphorebackend.h>

S::Threads::SemaphoreBackend::SemaphoreBackend(Void *iSemaphore)
{
	type = SEMAPHORE_NONE;
}

S::Threads::SemaphoreBackend::~SemaphoreBackend()
{
}

S::Int S::Threads::SemaphoreBackend::GetSemaphoreType()
{
	return type;
}

S::Void *S::Threads::SemaphoreBackend::GetSystemSemaphore()
{
	return NIL;
}

S::Int S::Threads::SemaphoreBackend::Wait()
{
	return Failure;
}

S::Int S::Threads::SemaphoreBackend::Release()
{
	return Failure;
}
