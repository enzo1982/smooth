 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/sdl/mutexsdl.h>

S::Threads::MutexBackend *CreateMutexSDL(S::Void *iMutex)
{
	return new S::Threads::MutexSDL(iMutex);
}

S::Int	 mutexSDLTmp = S::Threads::MutexBackend::SetBackend(&CreateMutexSDL);

S::Threads::MutexSDL::MutexSDL(Void *iMutex)
{
	type = MUTEX_SDL;

	if (iMutex != NIL)
	{
		mutex	= (SDL_mutex *) iMutex;
		myMutex	= False;
	}
	else
	{
		/* The mutex will be created once we need it
		 */
		mutex	= NIL;
		myMutex	= True;
	}
}

S::Threads::MutexSDL::~MutexSDL()
{
	if (myMutex && mutex != NIL) SDL_DestroyMutex(mutex);
}

S::Void *S::Threads::MutexSDL::GetSystemMutex() const
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexSDL::Lock()
{
	/* Lazy initialization of the mutex happens here
	 */
	if (mutex == NIL) mutex	= SDL_CreateMutex();

	SDL_mutexP(mutex);

	return Success();
}

S::Int S::Threads::MutexSDL::Release()
{
	if (mutex != NIL) SDL_mutexV(mutex);

	return Success();
}
