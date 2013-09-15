 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_THREADCOCOA
#define H_OBJSMOOTH_THREADCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace Threads
	{
		class ThreadCocoa;
	};
};

#include "../threadbackend.h"

#include <pthread.h>

namespace smooth
{
	namespace Threads
	{
		const Short	 THREAD_COCOA	= 3;

		struct ThreadInfo
		{
			Void	 (*threadProc)(Void *);
			Void	*threadParam;
		};

		class ThreadCocoa : public ThreadBackend
		{
			private:
				static Void	 Caller(ThreadInfo *);

				ThreadInfo	 info;
			protected:
				pthread_t	*thread;
				Bool		 myThread;
			public:
						 ThreadCocoa(Void * = NIL);
						~ThreadCocoa();

				Void		*GetSystemThread() const;
				Int		 GetThreadID() const;

				Bool		 IsCurrentThread() const;

				Int		 Start(Void (*)(Void *), Void *);
				Int		 Stop();

				Int		 Wait();

				Void		 Exit();
		};
	};
};

#endif
