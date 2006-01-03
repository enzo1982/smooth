 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/object.h>
#include <smooth/i18n/i18n.h>

const S::Int		 S::Object::classID		= S::Object::RequestClassID();

S::Int			 S::Object::nextClassID		= 0;
S::Int			 S::Object::nextObjectHandle	= 0;

S::Array<S::Object *, S::Void *>	 S::Object::objects;

S::Object::Object() : type(this)
{
	type			= classID;

	handle			= RequestObjectHandle();
	name			= String("Object::").Append(String::FromInt(handle));

	deleteObject		= False;
	isObjectInUse		= 0;

	flags			= 0;

	objects.AddEntry(this, handle);
}

S::Object::~Object()
{
	objects.RemoveEntry(handle);
}

S::Int S::Object::GetNOfObjects()
{
	return objects.GetNOfEntries();
}

S::Object *S::Object::GetNthObject(Int n)
{
	return objects.GetNthEntry(n);
}

S::Object *S::Object::GetObject(Int objectHandle, Int objectType)
{
	Object	*object = objects.GetEntry(objectHandle);

	if (object == NIL) return NIL;

	if (object->GetObjectType() == objectType)	return object;
	else						return NIL;
}

S::Object *S::Object::GetObject(const String &objectName)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetName() == objectName) return GetNthObject(i);
	}

	return NIL;
}

S::Int S::Object::GetHandle()
{
	return handle;
}

S::Int S::Object::SetName(const String &nName)
{
	if (GetObject(nName) != NIL) return Error();

	name = nName;

	return Success();
}

const S::String &S::Object::GetName()
{
	return name;
}

S::Int S::Object::SetFlags(Int nFlags)
{
	flags = nFlags;

	return Success();
}

S::Int S::Object::GetFlags()
{
	return flags;
}

S::String S::Object::ToString()
{
	return "an Object";
}

S::Object::operator S::String()
{
	return ToString();
}

S::Bool S::Object::IsTypeCompatible(Int objType)
{
	if (objType == classID)	return True;
	else			return False;
}

const S::ObjectType &S::Object::GetObjectType()
{
	return type;
}

S::Int S::Object::RequestClassID()
{
	return nextClassID++;
}

S::Int S::Object::RequestObjectHandle()
{
	return 	nextObjectHandle++;
}

S::Int S::Object::EnterProtectedRegion()
{
	return ++isObjectInUse;
}

S::Int S::Object::LeaveProtectedRegion()
{
	return --isObjectInUse;
}

S::Bool S::Object::IsObjectInUse()
{
	return isObjectInUse > 0;
}

S::Bool S::Object::IsObjectDeletable()
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

			return Success();
		}

		object->deleteObject = True;

		return Success();
	}

	return Error();
}
