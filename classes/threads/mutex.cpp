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

const S::Int	 S::Mutex::classID = S::Object::RequestClassID();

S::Mutex::Mutex()
{
	type = classID;

	mutex = LiSAMutexCreate();

	possibleContainers.AddEntry(Application::classID);
}

S::Mutex::~Mutex()
{
	LiSAMutexCloseHandle(mutex);
}

S::Int S::Mutex::Lock()
{
	LiSAMutexLock(mutex);

	return Success;
}

S::Int S::Mutex::Release()
{
	LiSAMutexRelease(mutex);

	return Success;
}
