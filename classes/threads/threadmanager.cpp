 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threadmanager.h>
#include <smooth/thread.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/messagebox.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::ThreadManager	*S::mainThreadManager;
S::Bool			 S::ThreadManager::threadManagerExists = S::False;

S::ThreadManager::ThreadManager()
{
	if (threadManagerExists)
	{
		iAmTheOne = False;

#ifdef __WIN32__
		QuickMessage("ThreadManager already exists! Creation refused!", "Error", MB_OK, IDI_INFORMATION);
#endif

		delete this;
	}
	else
	{
		iAmTheOne = True;
		nOfThreads = 0;

		threadManagerExists = True;
	}
}

S::ThreadManager::~ThreadManager()
{
	for (Int i = 0; i < nOfThreads; i++)
	{
		delete assocThreads.GetNthEntry(i);
	}

	assocThreads.RemoveAll();

	if (iAmTheOne) threadManagerExists = False;
}

S::Int S::ThreadManager::RegisterThread(Thread *thread)
{
	assocThreads.AddEntry(thread, thread->handle);
	nOfThreads++;

	return Success;
}

S::Int S::ThreadManager::UnregisterThread(Thread *thread)
{
	if (assocThreads.RemoveEntry(thread->handle) == True)
	{
		nOfThreads--;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Thread *S::ThreadManager::RequestThread(Int threadhandle)
{
	return assocThreads.GetEntry(threadhandle);
}

S::Int S::ThreadManager::GetNOfThreads()
{
	return nOfThreads;
}

S::Thread *S::ThreadManager::GetNthThread(Int n)
{
	return assocThreads.GetNthEntry(n);
}
