 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SEMAPHORE_
#define __OBJSMOOTH_SEMAPHORE_

#include <smooth/object.h>
#include <smooth/semaphore.h>
#include <smooth/stk.h>
#include <smooth/application.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_SEMAPHORE = SMOOTH::RequestObjectID();

SMOOTHSemaphore::SMOOTHSemaphore()
{
	type = OBJ_SEMAPHORE;

	semaphore = LiSASemaphoreCreate();

	possibleContainers.AddEntry(OBJ_APPLICATION);
}

SMOOTHSemaphore::~SMOOTHSemaphore()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	LiSASemaphoreCloseHandle(semaphore);
}

SMOOTHInt SMOOTHSemaphore::Wait()
{
	if (!registered) return SMOOTH::Error;

	LiSASemaphoreWait(semaphore);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSemaphore::Release()
{
	if (!registered) return SMOOTH::Error;

	LiSASemaphoreRelease(semaphore);

	return SMOOTH::Success;
}

#endif
