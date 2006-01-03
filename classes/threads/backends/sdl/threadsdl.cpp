 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/sdl/threadsdl.h>

S::Threads::ThreadBackend *CreateThreadSDL(S::Void *iThread)
{
	return new S::Threads::ThreadSDL(iThread);
}

S::Int	 threadSDLTmp = S::Threads::ThreadBackend::AddBackend(&CreateThreadSDL);

S::Threads::ThreadSDL::ThreadSDL(Void *iThread)
{
	type = THREAD_SDL;

	if (iThread != NIL)
	{
		thread		= (SDL_Thread *) iThread;
		myThread	= False;
	}
	else
	{
		thread		= NIL;
		myThread	= True;
	}
}

S::Threads::ThreadSDL::~ThreadSDL()
{
	if (myThread) Stop();
}

S::Void *S::Threads::ThreadSDL::GetSystemThread()
{
	return (Void *) SDL_GetThreadID(thread);
}

S::Int S::Threads::ThreadSDL::GetThreadID()
{
	return (Int) SDL_GetThreadID(thread);
}

S::Int S::Threads::ThreadSDL::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	Stop();

	thread = SDL_CreateThread((int (*)(void *)) threadProc, threadParam);

	return Success();
}

S::Int S::Threads::ThreadSDL::Stop()
{
	if (thread == NIL) return Error();

	SDL_KillThread(thread);

	if (myThread) myThread = False;

	thread = NIL;
	
	return Success();
}

S::Void S::Threads::ThreadSDL::Exit()
{
	if (thread == NIL) return;

	SDL_Thread	*self = thread;

	thread = NIL;

	SDL_KillThread(self);
}

S::Void *S::Threads::ThreadSDL::Self()
{
	return (Void *) SDL_ThreadID();
}
