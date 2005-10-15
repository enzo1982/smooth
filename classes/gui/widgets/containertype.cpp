 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/containertype.h>
#include <smooth/gui/widgets/container.h>

S::GUI::ContainerType::ContainerType(Container *iContainer)
{
	container	= iContainer;
	type		= Container::classID;
}

S::GUI::ContainerType::ContainerType(const ContainerType &containerType)
{
	container	= containerType.container;
	type		= containerType.type;
}

S::GUI::ContainerType::operator S::Int()
{
	return type;
}

S::Int S::GUI::ContainerType::operator =(Int newType)
{
	type = newType;

	return type;
}

S::Bool S::GUI::ContainerType::operator ==(Int objType)
{
	if (type == objType)					return True;
	else if (container->IsContainerCompatible(objType))	return True;
	else							return False;
}

S::Bool S::GUI::ContainerType::operator !=(Int objType)
{
	return !(*this == objType);
}

S::Bool S::GUI::ContainerType::operator ==(Array<Int> *objTypes)
{
	for (Int i = 0; i < objTypes->GetNOfEntries(); i++)
	{
		if (*this == objTypes->GetNthEntry(i)) return True;
	}

	return False;
}

S::Bool S::GUI::ContainerType::operator !=(Array<Int> *objTypes)
{
	return !(*this == objTypes);
}
