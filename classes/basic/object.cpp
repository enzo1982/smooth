 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/objectproperties.h>
#include <smooth/misc/i18n.h>

S::Array<S::Object *>	 S::Object::objects;
const S::Int		 S::Object::classID	= S::Object::RequestClassID();
S::Int			 S::Object::objectCount	= 0;
S::Int			 S::Object::nextID	= 0;

S::Object::Object() : type(this)
{
	type			= classID;

	objectProperties	= new ObjectProperties();

	handle			= RequestObjectHandle();

	deleteObject		= False;
	inUse			= 0;

	flags			= 0;

	registered		= False;

	myContainer		= NIL;

	objects.AddEntry(this, handle);
}

S::Object::~Object()
{
	objects.RemoveEntry(handle);

	delete objectProperties;
}

S::Int S::Object::GetNOfObjects()
{
	return objects.GetNOfEntries();
}

S::Object *S::Object::GetNthObject(Int n)
{
	return objects.GetNthEntry(n);
}

S::Object *S::Object::RequestObject(Int objectHandle)
{
	return objects.GetEntry(objectHandle);
}

S::Int S::Object::SetFlags(Int nFlags)
{
	flags = nFlags;

	return Success;
}

S::Int S::Object::GetFlags()
{
	return flags;
}

S::Bool S::Object::IsRegistered()
{
	return registered;
}

S::Bool S::Object::IsTypeCompatible(Int objType)
{
	if (objType == classID)	return True;
	else			return False;
}

S::ObjectProperties *S::Object::GetObjectProperties()
{
	return objectProperties;
}

S::ObjectType S::Object::GetObjectType()
{
	return type;
}

S::Int S::Object::SetContainer(GUI::Container *newContainer)
{
	myContainer = newContainer;

	return Success;
}

S::GUI::Container *S::Object::GetContainer()
{
	return myContainer;
}

S::Void S::Object::SetRegisteredFlag()
{
	registered = True;
}

S::Void S::Object::UnsetRegisteredFlag()
{
	registered = False;
}

S::Object *S::Object::GetObject(Int objectHandle, Int objectType)
{
	Object	*object = RequestObject(objectHandle);

	if (object == NIL) return NIL;

	if (object->GetObjectType() == objectType)	return object;
	else						return NIL;
}

S::Int S::Object::RequestClassID()
{
	return nextID++;
}

S::Int S::Object::RequestObjectHandle()
{
	return 	objectCount++;
}

S::Int S::Object::EnterProtectedRegion()
{
	return ++inUse;
}

S::Int S::Object::LeaveProtectedRegion()
{
	return --inUse;
}

S::Int S::Object::IsObjectInUse()
{
	return inUse;
}

S::Bool S::Object::IsObjectDeleteable()
{
	return deleteObject;
}

S::Int S::Object::DeleteObject(Object *object)
{
	if (object != NIL)
	{
		if (!object->IsObjectInUse())
		{
			delete object;

			return Success;
		}

		object->deleteObject = True;

		return Success;
	}

	return Error;
}
