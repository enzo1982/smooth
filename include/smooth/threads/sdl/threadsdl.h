 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREADSDL_
#define _H_OBJSMOOTH_THREADSDL_

namespace smooth
{
	namespace Threads
	{
		class ThreadSDL;
	};
};

#include "../threadbackend.h"

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

namespace smooth
{
	namespace Threads
	{
		const Int	 THREAD_SDL	= 3;

		class SMOOTH_STATIC_EXPORT ThreadSDL : public ThreadBackend
		{
			protected:
				SDL_Thread	*thread;
				Bool		 myThread;
			public:
						 ThreadSDL(Void * = NIL);
						~ThreadSDL();

				Void		*GetSystemThread();

				Int		 GetThreadID();

				Int		 Start(Void (*)(Void *), Void *);
				Int		 Stop();

				Void		 Exit();
				Void		*Self();
		};
	};
};

#endif
