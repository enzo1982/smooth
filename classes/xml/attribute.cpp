 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_XMLATTRIBUTE_
#define __OBJSMOOTH_XMLATTRIBUTE_

#include <libxml/parser.h>

#include <smooth/object.h>
#include <smooth/xml/attribute.h>
#include <smooth/stk.h>

SMOOTHXMLAttribute::SMOOTHXMLAttribute()
{
	nextAttribute	= NIL;
	prevAttribute	= NIL;

	parentNode	= NIL;

	attributeID	= -1;
	name		= NIL;
	content		= NIL;
}

SMOOTHXMLAttribute::~SMOOTHXMLAttribute()
{
}

SMOOTHXMLNode *SMOOTHXMLAttribute::GetParentNode()
{
	return parentNode;
}

SMOOTHInt SMOOTHXMLAttribute::SetParentNode(SMOOTHXMLNode *newParentNode)
{
	parentNode = newParentNode;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLAttribute::GetAttributeID()
{
	return attributeID;
}

SMOOTHInt SMOOTHXMLAttribute::SetAttributeID(SMOOTHInt newID)
{
	attributeID = newID;

	return SMOOTH::Success;
}

SMOOTHXMLAttribute *SMOOTHXMLAttribute::GetNextAttribute()
{
	return nextAttribute;
}

SMOOTHXMLAttribute *SMOOTHXMLAttribute::GetPrevAttribute()
{
	return prevAttribute;
}

SMOOTHInt SMOOTHXMLAttribute::SetNextAttribute(SMOOTHXMLAttribute *newAttribute)
{
	nextAttribute = newAttribute;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHXMLAttribute::SetPrevAttribute(SMOOTHXMLAttribute *newAttribute)
{
	prevAttribute = newAttribute;

	return SMOOTH::Success;
}

SMOOTHString SMOOTHXMLAttribute::GetName()
{
	return name;
}

SMOOTHInt SMOOTHXMLAttribute::SetName(SMOOTHString newName)
{
	name = newName;

	return SMOOTH::Success;
}

SMOOTHString SMOOTHXMLAttribute::GetContent()
{
	return content;
}

SMOOTHInt SMOOTHXMLAttribute::SetContent(SMOOTHString newContent)
{
	content = newContent;

	return SMOOTH::Success;
}

#endif
