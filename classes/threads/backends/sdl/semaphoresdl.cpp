 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/sdl/semaphoresdl.h>

S::Threads::SemaphoreBackend *CreateSemaphoreSDL(Int iValue, S::Void *iSemaphore)
{
	return new S::Threads::SemaphoreSDL(iValue, iSemaphore);
}

S::Int	 semaphoreSDLTmp = S::Threads::SemaphoreBackend::SetBackend(&CreateSemaphoreSDL);

S::Threads::SemaphoreSDL::SemaphoreSDL(Int iValue, Void *iSemaphore)
{
	type = SEMAPHORE_SDL;

	if (iSemaphore != NIL)
	{
		semaphore	= (SDL_sem *) iSemaphore;
		mySemaphore	= False;
	}
	else
	{
		/* The semaphore will be created once we need it
		 */
		semaphore	= NIL;
		initialValue	= iValue;
		mySemaphore	= True;
	}
}

S::Threads::SemaphoreSDL::~SemaphoreSDL()
{
	if (mySemaphore && semaphore != NIL) SDL_DestroySemaphore(semaphore);
}

S::Void *S::Threads::SemaphoreSDL::GetSystemSemaphore() const
{
	return (Void *) semaphore;
}

S::Int S::Threads::SemaphoreSDL::Wait()
{
	/* Lazy initialization of the semaphore happens here
	 */
	if (semaphore == NIL) semaphore	= SDL_CreateSemaphore(initialValue);

	SDL_SemWait(semaphore);

	return Success();
}

S::Int S::Threads::SemaphoreSDL::Release()
{
	if (semaphore != NIL) SDL_SemPost(semaphore);

	return Success();
}
