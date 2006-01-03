 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

S::Int	 mutexSDLTmp = S::Threads::MutexBackend::AddBackend(&CreateMutexSDL);

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
		mutex	= SDL_CreateMutex();
		myMutex	= True;
	}
}

S::Threads::MutexSDL::~MutexSDL()
{
	if (myMutex) SDL_DestroyMutex(mutex);
}

S::Void *S::Threads::MutexSDL::GetSystemMutex()
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexSDL::Lock()
{
	SDL_mutexP(mutex);

	return Success();
}

S::Int S::Threads::MutexSDL::Release()
{
	SDL_mutexV(mutex);

	return Success();
}
