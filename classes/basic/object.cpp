 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/object.h>
#include <smooth/system/timer.h>
#include <smooth/init.h>

const S::Short	 S::Object::classID		= S::Object::RequestClassID();

S::Short	 S::Object::nextClassID		= 0;
S::Int		 S::Object::nextObjectHandle	= 0;

S::Array<S::Object *, S::Void *>	 S::Object::objects;
S::Array<S::Object *, S::Void *>	 S::Object::deletable;

S::System::Timer	*S::Object::cleanupTimer = NIL;

S::Object::Object() : type(this)
{
	/* Enable R/W locking for object lists.
	 */
	if (objects.Length() == 0)
	{
		objects.EnableLocking();
		deletable.EnableLocking();
	}

	type	       = classID;

	handle	       = RequestObjectHandle();
	name	       = NIL;

	lockingEnabled = False;

	isDeletable    = False;
	isObjectInUse  = 0;

	flags	       = 0;

	objects.Add(this, handle);

	/* Create periodical cleanup timer if we
	 * just created the first object.
	 */
	if (objects.Length() == 1)
	{
		cleanupTimer = new System::Timer();

		cleanupTimer->onInterval.Connect(&Object::ObjectCleanup);
		cleanupTimer->Start(1000);
	}
}

S::Object::~Object()
{
	/* Try to remove ourself from the object list
	 * as DeleteObject might not have been called.
	 */
	if (!isDeletable) objects.Remove(handle);

	/* Free periodical cleanup timer if the timer
	 * itself is the only remaining object.
	 */
	if (objects.Length() == 1)
	{
		cleanupTimer->Stop();

		DeleteObject(cleanupTimer);
	}

	/* Delete name string if it was created.
	 */
	if (name != NIL) delete name;
}

S::Int S::Object::EnableLocking(Bool enable)
{
	lockingEnabled = enable;

	return Success();
}

S::Int S::Object::GetNOfObjects()
{
	return objects.Length();
}

S::Object *S::Object::GetNthObject(Int n)
{
	return objects.GetNth(n);
}

S::Object *S::Object::GetObject(Int objectHandle, Short objectType)
{
	Object	*object = objects.Get(objectHandle);

	if (object == NIL) return NIL;

	if (object->GetObjectType() == objectType) return object;
	else					   return NIL;
}

S::Object *S::Object::GetObject(const String &objectName)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object	*object = GetNthObject(i);

		if (object->name != NIL && *(object->name) == objectName) return object;
	}

	return NIL;
}

S::Int S::Object::SetName(const String &nName)
{
	if (GetObject(nName) != NIL) return Error();

	if (name == NIL) name = new String();

	*name = nName;

	return Success();
}

const S::String &S::Object::GetName() const
{
	static String	 prefix = "Object::";

	if (name == NIL) name = new String(String(prefix).Append(String::FromInt(handle)));

	return *name;
}

S::Short S::Object::RequestClassID()
{
	return nextClassID++;
}

S::Int S::Object::RequestObjectHandle()
{
	if (initializing) return			    nextObjectHandle++;
	else		  return Threads::Access::Increment(nextObjectHandle) - 1;
}

S::Int S::Object::DeleteObject(Object *object)
{
	if (object == NIL || objects.Get(object->handle) == NIL) return Error();

	/* Notify object that it will be deleted soon.
	 */
	object->EnqueueForDeletion();
	object->isDeletable = True;

	if (!object->IsObjectInUse())
	{
		/* Delete object immediately if
		 * it is not currently in use.
		 */
		objects.Remove(object->handle);
		delete object;
	}
	else
	{
		/* Remove object from object list and add
		 * it to the list of objects to delete.
		 */
		objects.Remove(object->handle);
		deletable.Add(object, object->handle);
	}

	return Success();
}

S::Void S::Object::ObjectCleanup()
{
	/* Loop through all deletable objects...
	 */
	for (Int i = 0; i < deletable.Length(); i++)
	{
		Object	*object = deletable.GetNth(i);

		if (object->IsObjectInUse()) continue;

		deletable.RemoveNth(i--);
		delete object;
	}
}
