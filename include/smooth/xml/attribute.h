 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLATTRIBUTE_
#define _H_OBJSMOOTH_XMLATTRIBUTE_

#define SXMLAttribute SMOOTHXMLAttribute

class SMOOTHXMLAttribute;

#include "../definitions.h"
#include "node.h"

class SMOOTHAPI SMOOTHXMLAttribute
{
	private:
		SMOOTHXMLAttribute	*nextAttribute;
		SMOOTHXMLAttribute	*prevAttribute;

		SMOOTHXMLNode		*parentNode;

		SMOOTHInt		 attributeID;
		SMOOTHString		 name;
		SMOOTHString		 content;
	public:
					 SMOOTHXMLAttribute();
					~SMOOTHXMLAttribute();

		SMOOTHXMLNode		*GetParentNode();
		SMOOTHInt		 SetParentNode(SMOOTHXMLNode *);

		SMOOTHInt		 GetAttributeID();
		SMOOTHInt		 SetAttributeID(SMOOTHInt);

		SMOOTHXMLAttribute	*GetNextAttribute();
		SMOOTHXMLAttribute	*GetPrevAttribute();

		SMOOTHInt		 SetNextAttribute(SMOOTHXMLAttribute *);
		SMOOTHInt		 SetPrevAttribute(SMOOTHXMLAttribute *);

		SMOOTHString		 GetName();
		SMOOTHInt		 SetName(SMOOTHString);

		SMOOTHString		 GetContent();
		SMOOTHInt		 SetContent(SMOOTHString);
};

#endif
