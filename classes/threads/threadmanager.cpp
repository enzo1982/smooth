 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_THREADMANAGER_
#define __OBJSMOOTH_THREADMANAGER_

#include <smooth/threadmanager.h>
#include <smooth/thread.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/definitions.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHThreadManager	*mainThreadManager;
SMOOTHBool		 SMOOTHThreadManager::threadManagerExists = SMOOTH::False;

SMOOTHThreadManager::SMOOTHThreadManager()
{
	if (threadManagerExists)
	{
		iAmTheOne = SMOOTH::False;

#ifdef __WIN32__
		SMOOTH::MessageBox(TXT_ERROR, TXT_ERROR_THREADMANAGEREXISTS, MB_OK, IDI_INFORMATION);
#endif

		delete this;
	}
	else
	{
		iAmTheOne = SMOOTH::True;
		nOfThreads = 0;

		threadManagerExists = SMOOTH::True;
	}
}

SMOOTHThreadManager::~SMOOTHThreadManager()
{
	for (SMOOTHInt i = 0; i < nOfThreads; i++)
	{
		delete assocThreads.GetNthEntry(i);
	}

	assocThreads.DeleteAll();

	if (iAmTheOne) threadManagerExists = SMOOTH::False;
}

SMOOTHInt SMOOTHThreadManager::RegisterThread(SMOOTHThread *thread)
{
	assocThreads.AddEntry(thread, thread->handle);
	nOfThreads++;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHThreadManager::UnregisterThread(SMOOTHThread *thread)
{
	if (assocThreads.DeleteEntry(thread->handle) == SMOOTH::True)
	{
		nOfThreads--;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHThread *SMOOTHThreadManager::RequestThread(SMOOTHInt threadhandle)
{
	return assocThreads.GetEntry(threadhandle);
}

SMOOTHInt SMOOTHThreadManager::GetNOfThreads()
{
	return nOfThreads;
}

#endif
