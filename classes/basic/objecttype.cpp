 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/objecttype.h>
#include <smooth/object.h>

S::ObjectType::ObjectType()
{
	object	= NIL;
	type	= OBJ_OBJECT;
}

S::ObjectType::ObjectType(const ObjectType &objectType)
{
	object	= objectType.object;
	type	= objectType.type;
}

S::ObjectType::operator S::Int()
{
	return type;
}

S::Int S::ObjectType::operator =(Int newType)
{
	type = newType;

	return type;
}

S::Bool S::ObjectType::operator ==(Int objType)
{
	if (type == objType)				return True;
	else if (object->IsTypeCompatible(objType))	return True;
	else						return False;
}

S::Bool S::ObjectType::operator !=(Int objType)
{
	return !(*this == objType);
}
