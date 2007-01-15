 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/attribute.h>

S::Array<S::String> S::XML::Attribute::attributeNames;

S::XML::Attribute::Attribute(const String &iName, const String &iContent)
{
	attributeID	= -1;

	nameIndex	= iName.ComputeCRC32();
	content		= iContent;

	if (attributeNames.Get(nameIndex) == NIL) attributeNames.Add(iName, nameIndex);
}

S::XML::Attribute::~Attribute()
{
}

S::Int S::XML::Attribute::GetAttributeID() const
{
	return attributeID;
}

S::Int S::XML::Attribute::SetAttributeID(Int newID)
{
	attributeID = newID;

	return Success();
}

const S::String &S::XML::Attribute::GetName() const
{
	return attributeNames.Get(nameIndex);
}

S::Int S::XML::Attribute::SetName(const String &newName)
{
	nameIndex = newName.ComputeCRC32();

	if (attributeNames.Get(nameIndex) == NIL) attributeNames.Add(newName, nameIndex);

	return Success();
}

const S::String &S::XML::Attribute::GetContent() const
{
	return content;
}

S::Int S::XML::Attribute::SetContent(const String &newContent)
{
	content = newContent;

	return Success();
}
