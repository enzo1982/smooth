 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OBJECTTYPE_
#define __OBJSMOOTH_OBJECTTYPE_

#include <smooth/objecttype.h>
#include <smooth/stk.h>
#include <smooth/object.h>

SMOOTHObjectType::SMOOTHObjectType()
{
	object	= NIL;
	type	= OBJ_OBJECT;
}

SMOOTHObjectType::SMOOTHObjectType(const SMOOTHObjectType &objectType)
{
	object	= objectType.object;
	type	= objectType.type;
}

SMOOTHObjectType::operator SMOOTHInt()
{
	return type;
}

SMOOTHInt SMOOTHObjectType::operator =(SMOOTHInt newType)
{
	type = newType;

	return type;
}

SMOOTHBool SMOOTHObjectType::operator ==(SMOOTHInt objType)
{
	if (type == objType)				return SMOOTH::True;
	else if (object->IsTypeCompatible(objType))	return SMOOTH::True;
	else						return SMOOTH::False;
}

SMOOTHBool SMOOTHObjectType::operator !=(SMOOTHInt objType)
{
	return !(*this == objType);
}

#endif
