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
#include <smooth/mutex.h>
#include <smooth/stk.h>
#include <smooth/application.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_MUTEX = S::Object::RequestObjectID();

S::Mutex::Mutex()
{
	type = OBJ_MUTEX;

	mutex = LiSAMutexCreate();

	possibleContainers.AddEntry(OBJ_APPLICATION);
}

S::Mutex::~Mutex()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	LiSAMutexCloseHandle(mutex);
}

S::Int S::Mutex::Lock()
{
	if (!registered) return Error;

	LiSAMutexLock(mutex);

	return Success;
}

S::Int S::Mutex::Release()
{
	if (!registered) return Error;

	LiSAMutexRelease(mutex);

	return Success;
}
