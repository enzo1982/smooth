 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/container.h>
#include <smooth/graphics/surface.h>
#include <smooth/object.h>
#include <smooth/graphics/window.h>

const S::Int	 S::Container::classID = S::Object::RequestClassID();

S::Container::Container() : containerType(this)
{
	self		= NIL;
	containerType	= classID;

	nullSurface = new GUI::Surface();
	drawSurface = nullSurface;
}

S::Container::~Container()
{
	Int	 nOfObjects = assocObjects.GetNOfEntries();

	for (Int i = 0; i < nOfObjects; i++)
	{
		UnregisterObject(assocObjects.GetFirstEntry());
	}

	assocObjects.RemoveAll();

	delete nullSurface;
}

S::Int S::Container::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);

			object->SetContainer(this);
			object->SetRegisteredFlag();

			return Success;
		}
	}

	return Error;
}

S::Int S::Container::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->handle) == True)
			{
				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				return Success;
			}
		}
	}

	return Error;
}

S::Int S::Container::GetNOfObjects()
{
	return assocObjects.GetNOfEntries();
}

S::Object *S::Container::GetNthObject(Int n)
{
	return assocObjects.GetNthEntry(n);
}

S::Object *S::Container::RequestObject(Int objectHandle)
{
	return assocObjects.GetEntry(objectHandle);
}

S::Bool S::Container::IsContainerCompatible(Int objType)
{
	if (objType == classID)	return True;
	else			return False;
}

S::ContainerType S::Container::GetContainerType()
{
	return containerType;
}

S::Object *S::Container::GetContainerObject()
{
	return self;
}

S::GUI::Window *S::Container::GetContainerWindow()
{
	if (self == NIL) return NIL;

	Object	*object = self;

	while (object->GetObjectType() != GUI::Window::classID)
	{
		if (object->GetContainer() == NIL) return NIL;

		object = object->GetContainer()->GetContainerObject();

		if (object == NIL) return NIL;
	}

	return (GUI::Window *) object;
}

S::GUI::Surface *S::Container::GetDrawSurface()
{
	return drawSurface;
}
