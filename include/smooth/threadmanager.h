 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_THREADMANAGER_
#define _H_OBJSMOOTH_THREADMANAGER_

#define SThreadManager SMOOTHThreadManager

class SMOOTHThreadManager;

#include "application.h"
#include "thread.h"

class SMOOTHAPI SMOOTHThreadManager
{
	protected:
		SMOOTHArray<SMOOTHThread *>	 assocThreads;
		SMOOTHInt			 nOfThreads;

		static SMOOTHBool		 threadManagerExists;
		SMOOTHBool			 iAmTheOne;
	public:
						 SMOOTHThreadManager();
						~SMOOTHThreadManager();

		SMOOTHInt			 RegisterThread(SMOOTHThread *);
		SMOOTHInt			 UnregisterThread(SMOOTHThread *);

		SMOOTHThread			*RequestThread(SMOOTHInt);
		SMOOTHInt			 GetNOfThreads();
};

SMOOTHVAR SMOOTHThreadManager	*mainThreadManager;

#endif
