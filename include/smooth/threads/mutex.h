 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_THREADS_MUTEX
#define H_OBJSMOOTH_THREADS_MUTEX

namespace smooth
{
	namespace Threads
	{
		class Mutex;
		class MutexBackend;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace Threads
	{
		class SMOOTHAPI Mutex
		{
			private:
				MutexBackend		*backend;
			public:
							 Mutex(Void * = NIL);
							 Mutex(const Mutex &);
							~Mutex();

				Mutex &operator		 =(const Mutex &);

				Int			 GetMutexType() const;

				Void			*GetSystemMutex() const;

				Bool			 Lock();
				Bool			 TryLock();

				Bool			 Release();
		};

		/* A simple scoped locker for mutexes.
		 */
		class SMOOTHAPI Lock
		{
			private:
				Mutex	&mutex;
			public:
					 Lock(Mutex &m) : mutex(m)	{ mutex.Lock(); }
					~Lock()				{ mutex.Release(); }
		};
	};
};

#endif
