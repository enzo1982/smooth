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

const S::Int	 S::Semaphore::classID = S::Object::RequestClassID();

S::Semaphore::Semaphore()
{
	type = classID;

	semaphore = LiSASemaphoreCreate();

	possibleContainers.AddEntry(Application::classID);
}

S::Semaphore::~Semaphore()
{
	LiSASemaphoreCloseHandle(semaphore);
}

S::Int S::Semaphore::Wait()
{
	LiSASemaphoreWait(semaphore);

	return Success;
}

S::Int S::Semaphore::Release()
{
	LiSASemaphoreRelease(semaphore);

	return Success;
}
