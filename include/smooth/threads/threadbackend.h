 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREADBACKEND_
#define _H_OBJSMOOTH_THREADBACKEND_

namespace smooth
{
	namespace Threads
	{
		class ThreadBackend;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace Threads
	{
		const Int	 THREAD_NONE = 0;

		class ThreadBackend
		{
			protected:
				Int		 type;
			public:
						 ThreadBackend(Void * = NIL);
				virtual		~ThreadBackend();

				Int		 GetThreadType();

				virtual Void	*GetSystemThread();

				virtual Int	 GetThreadID();

				virtual Int	 Start(Void (*)(Void *), Void *);
				virtual Int	 Stop();

				virtual Void	 Exit();
				virtual Void	*Self();
		};
	};
};

#endif
