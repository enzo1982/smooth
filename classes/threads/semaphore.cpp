 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/semaphore.h>
#include <smooth/stk.h>
#include <smooth/application.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_SEMAPHORE = S::Object::RequestObjectID();

S::Semaphore::Semaphore()
{
	type = OBJ_SEMAPHORE;

	semaphore = LiSASemaphoreCreate();

	possibleContainers.AddEntry(OBJ_APPLICATION);
}

S::Semaphore::~Semaphore()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	LiSASemaphoreCloseHandle(semaphore);
}

S::Int S::Semaphore::Wait()
{
	if (!registered) return Error;

	LiSASemaphoreWait(semaphore);

	return Success;
}

S::Int S::Semaphore::Release()
{
	if (!registered) return Error;

	LiSASemaphoreRelease(semaphore);

	return Success;
}
