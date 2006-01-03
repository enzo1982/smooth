 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLNODE_
#define _H_OBJSMOOTH_XMLNODE_

namespace smooth
{
	namespace XML
	{
		class Node;
	};
};

#include "../definitions.h"
#include "../misc/string.h"
#include "attribute.h"
#include "document.h"

namespace smooth
{
	namespace XML
	{
		class SMOOTHAPI Node
		{
			private:
				static Array<String>		 elementNames;

				Array<Attribute *, Void *>	*attributes;
				Array<Node *, Void *>		*subnodes;

				Int				 nodeID;

				Int				 nameIndex;
				String				 content;
			public:
								 Node(const String &, const String & = NIL);
								~Node();

				Int				 GetNodeID();
				Int				 SetNodeID(Int);

				const String			 GetName();
				Int				 SetName(const String &);

				const String			&GetContent();
				Int				 SetContent(const String &);

/* Functions for attribute access */
				Int				 GetNOfAttributes();
				Attribute			*GetNthAttribute(Int);
				Attribute			*GetAttributeByName(const String &);

				Attribute			*SetAttribute(const String &, const String &);
				Int				 RemoveAttribute(Attribute *);
				Int				 RemoveAttributeByName(const String &);

/* Functions for subnode access */
				Int				 GetNOfNodes();
				Node				*GetNthNode(Int);
				Node				*GetNodeByName(const String &);

				Node				*AddNode(const String &, const String & = NIL);
				Int				 RemoveNode(Node *);
				Int				 RemoveNodeByName(const String &);
		};
	};
};

#endif
