 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/container.h>
#include <smooth/graphics/surface.h>
#include <smooth/object.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Container::classID = S::Object::RequestClassID();

S::GUI::Container::Container() : containerType(this)
{
	containerType	= classID;

	nullSurface	= new GUI::Surface();
	drawSurface	= nullSurface;
}

S::GUI::Container::~Container()
{
	Int	 nOfObjects = assocObjects.GetNOfEntries();

	for (Int i = 0; i < nOfObjects; i++)
	{
		UnregisterObject(assocObjects.GetFirstEntry());
	}

	assocObjects.RemoveAll();

	delete nullSurface;
}

S::Int S::GUI::Container::RegisterObject(Object *object)
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

S::Int S::GUI::Container::UnregisterObject(Object *object)
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

S::Int S::GUI::Container::GetNOfObjects()
{
	return assocObjects.GetNOfEntries();
}

S::Object *S::GUI::Container::GetNthObject(Int n)
{
	return assocObjects.GetNthEntry(n);
}

S::Object *S::GUI::Container::RequestObject(Int objectHandle)
{
	return assocObjects.GetEntry(objectHandle);
}

S::Bool S::GUI::Container::IsContainerCompatible(Int objType)
{
	if (objType == classID)	return True;
	else			return False;
}

S::GUI::ContainerType S::GUI::Container::GetContainerType()
{
	return containerType;
}

S::GUI::Window *S::GUI::Container::GetContainerWindow()
{
	Object	*object = this;

	while (object->GetObjectType() != GUI::Window::classID)
	{
		if (object->GetContainer() == NIL) return NIL;

		object = object->GetContainer();

		if (object == NIL) return NIL;
	}

	return (GUI::Window *) object;
}

S::GUI::Surface *S::GUI::Container::GetDrawSurface()
{
	return drawSurface;
}
