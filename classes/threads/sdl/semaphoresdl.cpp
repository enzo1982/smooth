 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/sdl/semaphoresdl.h>

S::Threads::SemaphoreSDL::SemaphoreSDL(Void *iSemaphore)
{
	type = SEMAPHORE_SDL;

	if (iSemaphore != NIL)
	{
		semaphore	= (SDL_sem *) iSemaphore;
		mySemaphore	= False;
	}
	else
	{
		semaphore	= SDL_CreateSemaphore(1);
		mySemaphore	= True;
	}
}

S::Threads::SemaphoreSDL::~SemaphoreSDL()
{
	if (mySemaphore) SDL_DestroySemaphore(semaphore);
}

S::Void *S::Threads::SemaphoreSDL::GetSystemSemaphore()
{
	return (Void *) semaphore;
}

S::Int S::Threads::SemaphoreSDL::Wait()
{
	SDL_SemWait(semaphore);

	return Success;
}

S::Int S::Threads::SemaphoreSDL::Release()
{
	SDL_SemPost(semaphore);

	return Success;
}
