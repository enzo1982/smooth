 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OBJECTMANAGER_
#define __OBJSMOOTH_OBJECTMANAGER_

#include <smooth/objectmanager.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHObjectManager	*mainObjectManager;
SMOOTHBool		 SMOOTHObjectManager::objectManagerExists = SMOOTH::False;

SMOOTHObjectManager::SMOOTHObjectManager()
{
	if (objectManagerExists)
	{
		iAmTheOne = SMOOTH::False;

#ifdef __WIN32__
		SMOOTH::MessageBox(TXT_ERROR, TXT_ERROR_OBJECTMANAGEREXISTS, MB_OK, IDI_INFORMATION);
#endif

		delete this;
	}
	else
	{
		iAmTheOne = SMOOTH::True;

		objectManagerExists = SMOOTH::True;
	}
}

SMOOTHObjectManager::~SMOOTHObjectManager()
{
	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		delete assocObjects.GetNthEntry(i);
	}

	nOfObjects = 0;

	if (iAmTheOne) objectManagerExists = SMOOTH::False;
}

SMOOTHInt SMOOTHObjectManager::RegisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	assocObjects.AddEntry(object, object->handle);
	nOfObjects++;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHObjectManager::UnregisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (assocObjects.DeleteEntry(object->handle) == SMOOTH::True)
	{
		nOfObjects--;

		return SMOOTH::Success;
	}

	return SMOOTH::Error;
}

#endif
