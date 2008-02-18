 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/object.h>
#include <smooth/system/timer.h>

const S::Int	 S::Object::classID		= S::Object::RequestClassID();

S::Int		 S::Object::nextClassID		= 0;
S::Int		 S::Object::nextObjectHandle	= 0;

S::Array<S::Object *, S::Void *>	 S::Object::objects;
S::Array<S::Object *, S::Void *>	 S::Object::deleteable;

S::System::Timer	*S::Object::cleanupTimer = NIL;

S::Object::Object() : type(this)
{
	type			= classID;

	handle			= RequestObjectHandle();
	name			= String("Object::").Append(String::FromInt(handle));

	isObjectInUse		= 0;

	flags			= 0;

	objects.Add(this, handle);

	/* Create periodical cleanup timer if we
	 * just created the first object.
	 */
	if (objects.Length() == 1)
	{
		cleanupTimer = new System::Timer();

		cleanupTimer->onInterval.Connect(&Object::ObjectCleanup);
		cleanupTimer->Start(10000);
	}
}

S::Object::~Object()
{
	/* Try to remove ourself from the object list
	 * as DeleteObject might not have been called.
	 */
	objects.Remove(handle);

	/* Free periodical cleanup timer if the timer
	 * itself is the only remaining object.
	 */
	if (objects.Length() == 1)
	{
		cleanupTimer->Stop();

		DeleteObject(cleanupTimer);

		ObjectCleanup();
	}
}

S::Int S::Object::GetNOfObjects()
{
	return objects.Length();
}

S::Object *S::Object::GetNthObject(Int n)
{
	return objects.GetNth(n);
}

S::Object *S::Object::GetObject(Int objectHandle, Int objectType)
{
	Object	*object = objects.Get(objectHandle);

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

S::Int S::Object::SetName(const String &nName)
{
	if (GetObject(nName) != NIL) return Error();

	name = nName;

	return Success();
}

S::Int S::Object::RequestClassID()
{
	return nextClassID++;
}

S::Int S::Object::RequestObjectHandle()
{
	return nextObjectHandle++;
}

S::Int S::Object::DeleteObject(Object *object)
{
	if (object != NIL)
	{
		/* Remove object from object list and add
		 * it to the list of objects to delete.
		 */
		objects.Remove(object->handle);
		deleteable.Add(object, object->handle);

		return Success();
	}

	return Error();
}

S::Void S::Object::ObjectCleanup()
{
	/* Loop through all deleteable objects...
	 */
	for (Int i = 0; i < deleteable.Length(); i++)
	{
		Object	*object = deleteable.GetNth(i);

		if (!object->IsObjectInUse())
		{
			deleteable.Remove(object->handle);
			delete object;

			i = -1;

			continue;
		}
	}
}
