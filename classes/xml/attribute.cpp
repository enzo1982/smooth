 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/attribute.h>

S::XML::Attribute::Attribute()
{
	nextAttribute	= NIL;
	prevAttribute	= NIL;

	parentNode	= NIL;

	attributeID	= -1;
	name		= NIL;
	content		= NIL;
}

S::XML::Attribute::~Attribute()
{
}

S::XML::Node *S::XML::Attribute::GetParentNode()
{
	return parentNode;
}

S::Int S::XML::Attribute::SetParentNode(Node *newParentNode)
{
	parentNode = newParentNode;

	return Success;
}

S::Int S::XML::Attribute::GetAttributeID()
{
	return attributeID;
}

S::Int S::XML::Attribute::SetAttributeID(Int newID)
{
	attributeID = newID;

	return Success;
}

S::XML::Attribute *S::XML::Attribute::GetNextAttribute()
{
	return nextAttribute;
}

S::XML::Attribute *S::XML::Attribute::GetPrevAttribute()
{
	return prevAttribute;
}

S::Int S::XML::Attribute::SetNextAttribute(Attribute *newAttribute)
{
	nextAttribute = newAttribute;

	return Success;
}

S::Int S::XML::Attribute::SetPrevAttribute(Attribute *newAttribute)
{
	prevAttribute = newAttribute;

	return Success;
}

S::String S::XML::Attribute::GetName()
{
	return name;
}

S::Int S::XML::Attribute::SetName(String newName)
{
	name = newName;

	return Success;
}

S::String S::XML::Attribute::GetContent()
{
	return content;
}

S::Int S::XML::Attribute::SetContent(String newContent)
{
	content = newContent;

	return Success;
}
