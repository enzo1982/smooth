 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MUTEX_
#define __OBJSMOOTH_MUTEX_

#include <smooth/object.h>
#include <smooth/mutex.h>
#include <smooth/stk.h>
#include <smooth/application.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_MUTEX = SMOOTH::RequestObjectID();

SMOOTHMutex::SMOOTHMutex()
{
	type = OBJ_MUTEX;

	mutex = LiSAMutexCreate();

	possibleContainers.AddEntry(OBJ_APPLICATION);
}

SMOOTHMutex::~SMOOTHMutex()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	LiSAMutexCloseHandle(mutex);
}

SMOOTHInt SMOOTHMutex::Lock()
{
	if (!registered) return SMOOTH::Error;

	LiSAMutexLock(mutex);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMutex::Release()
{
	if (!registered) return SMOOTH::Error;

	LiSAMutexRelease(mutex);

	return SMOOTH::Success;
}

#endif
