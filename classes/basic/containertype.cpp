 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CONTAINERTYPE_
#define __OBJSMOOTH_CONTAINERTYPE_

#include <smooth/containertype.h>
#include <smooth/stk.h>
#include <smooth/container.h>

SMOOTHContainerType::SMOOTHContainerType()
{
	container	= NIL;
	type		= OBJ_CONTAINER;
}

SMOOTHContainerType::SMOOTHContainerType(const SMOOTHContainerType &containerType)
{
	container	= containerType.container;
	type		= containerType.type;
}

SMOOTHContainerType::operator SMOOTHInt()
{
	return type;
}

SMOOTHInt SMOOTHContainerType::operator =(SMOOTHInt newType)
{
	type = newType;

	return type;
}

SMOOTHBool SMOOTHContainerType::operator ==(SMOOTHInt objType)
{
	if (type == objType)					return SMOOTH::True;
	else if (container->IsContainerCompatible(objType))	return SMOOTH::True;
	else							return SMOOTH::False;
}

SMOOTHBool SMOOTHContainerType::operator !=(SMOOTHInt objType)
{
	return !(*this == objType);
}

SMOOTHBool SMOOTHContainerType::operator ==(SMOOTHArray<SMOOTHInt> *objTypes)
{
	for (SMOOTHInt i = 0; i < objTypes->GetNOfEntries(); i++)
	{
		if (*this == objTypes->GetNthEntry(i)) return SMOOTH::True;
	}

	return SMOOTH::False;
}

SMOOTHBool SMOOTHContainerType::operator !=(SMOOTHArray<SMOOTHInt> *objTypes)
{
	return !(*this == objTypes);
}

#endif
