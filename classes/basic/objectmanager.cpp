 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/objectmanager.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::ObjectManager	*S::mainObjectManager;
S::Bool			 S::ObjectManager::objectManagerExists = S::False;

S::ObjectManager::ObjectManager()
{
	if (objectManagerExists)
	{
		iAmTheOne = False;

#ifdef __WIN32__
		SMOOTH::MessageBox("ObjectManager already exists! Creation refused!", "Error", MB_OK, IDI_INFORMATION);
#endif

		delete this;
	}
	else
	{
		iAmTheOne = True;

		objectManagerExists = True;
	}
}

S::ObjectManager::~ObjectManager()
{
	for (Int i = 0; i < nOfObjects; i++)
	{
		delete assocObjects.GetNthEntry(i);
	}

	nOfObjects = 0;

	if (iAmTheOne) objectManagerExists = False;
}

S::Int S::ObjectManager::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	assocObjects.AddEntry(object, object->handle);
	nOfObjects++;

	return Success;
}

S::Int S::ObjectManager::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (assocObjects.RemoveEntry(object->handle) == True)
	{
		nOfObjects--;

		return Success;
	}

	return Error;
}
