 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREADMANAGER_
#define _H_OBJSMOOTH_THREADMANAGER_

namespace smooth
{
	class ThreadManager;
};

#include "application.h"
#include "thread.h"

namespace smooth
{
	class SMOOTHAPI ThreadManager
	{
		protected:
			Array<Thread *>	 assocThreads;
			Int		 nOfThreads;

			static Bool	 threadManagerExists;
			Bool		 iAmTheOne;
		public:
					 ThreadManager();
					~ThreadManager();

			Int		 RegisterThread(Thread *);
			Int		 UnregisterThread(Thread *);

			Thread		*RequestThread(Int);
			Int		 GetNOfThreads();
	};

	SMOOTHVAR ThreadManager	*mainThreadManager;
};

#endif
