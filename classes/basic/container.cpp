 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CONTAINER_
#define __OBJSMOOTH_CONTAINER_

#include <smooth/container.h>
#include <smooth/stk.h>
#include <smooth/surface.h>
#include <smooth/object.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_CONTAINER = SMOOTH::RequestObjectID();

SMOOTHContainer::SMOOTHContainer()
{
	self			= NIL;
	nOfObjects		= 0;
	containerType.container	= this;
	containerType		= OBJ_CONTAINER;

	nullSurface = new SMOOTHSurface();
	drawSurface = nullSurface;
}

SMOOTHContainer::~SMOOTHContainer()
{
	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		if (UnregisterObject(assocObjects.GetFirstEntry()) == SMOOTH::Success) nOfObjects++;
	}

	assocObjects.DeleteAll();

	delete nullSurface;
}

SMOOTHInt SMOOTHContainer::RegisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHContainer::UnregisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (object->IsRegistered())
		{
			if (assocObjects.DeleteEntry(object->handle) == SMOOTH::True)
			{
				nOfObjects--;

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				return SMOOTH::Success;
			}
		}
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHContainer::GetNOfObjects()
{
	return nOfObjects;
}

SMOOTHObject *SMOOTHContainer::RequestObject(SMOOTHInt objectHandle)
{
	return assocObjects.GetEntry(objectHandle);
}

SMOOTHBool SMOOTHContainer::IsContainerCompatible(SMOOTHInt objType)
{
	if (objType == OBJ_CONTAINER)	return SMOOTH::True;
	else				return SMOOTH::False;
}

SMOOTHContainerType SMOOTHContainer::GetContainerType()
{
	return containerType;
}

SMOOTHObject *SMOOTHContainer::GetContainerObject()
{
	return self;
}

SMOOTHSurface *SMOOTHContainer::GetDrawSurface()
{
	return drawSurface;
}

#endif
