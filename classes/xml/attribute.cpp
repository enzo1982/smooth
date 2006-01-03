 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

	if (attributeNames.GetEntry(nameIndex) == NIL) attributeNames.AddEntry(iName, nameIndex);
}

S::XML::Attribute::~Attribute()
{
}

S::Int S::XML::Attribute::GetAttributeID()
{
	return attributeID;
}

S::Int S::XML::Attribute::SetAttributeID(Int newID)
{
	attributeID = newID;

	return Success();
}

S::String S::XML::Attribute::GetName()
{
	return attributeNames.GetEntry(nameIndex);
}

S::Int S::XML::Attribute::SetName(const String &newName)
{
	nameIndex = newName.ComputeCRC32();

	if (attributeNames.GetEntry(nameIndex) == NIL) attributeNames.AddEntry(newName, nameIndex);

	return Success();
}

const S::String &S::XML::Attribute::GetContent()
{
	return content;
}

S::Int S::XML::Attribute::SetContent(const String &newContent)
{
	content = newContent;

	return Success();
}
