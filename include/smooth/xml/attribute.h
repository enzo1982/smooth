 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLATTRIBUTE_
#define _H_OBJSMOOTH_XMLATTRIBUTE_

namespace smooth
{
	namespace XML
	{
		class Attribute;
	};
};

#include "../definitions.h"
#include "node.h"

namespace smooth
{
	namespace XML
	{
		class SMOOTHAPI Attribute
		{
			private:
				Attribute	*nextAttribute;
				Attribute	*prevAttribute;

				Node		*parentNode;

				Int		 attributeID;
				String		 name;
				String		 content;
			public:
						 Attribute();
						~Attribute();

				Node		*GetParentNode();
				Int		 SetParentNode(Node *);

				Int		 GetAttributeID();
				Int		 SetAttributeID(Int);

				Attribute	*GetNextAttribute();
				Attribute	*GetPrevAttribute();

				Int		 SetNextAttribute(Attribute *);
				Int		 SetPrevAttribute(Attribute *);

				String		 GetName();
				Int		 SetName(String);

				String		 GetContent();
				Int		 SetContent(String);
		};
	};
};

#endif
