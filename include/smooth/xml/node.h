 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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
#include "../string.h"
#include "attribute.h"
#include "document.h"

namespace smooth
{
	namespace XML
	{
		class SMOOTHAPI Node
		{
			private:
				Node			*nextNode;
				Node			*prevNode;

				Document		*parentDoc;
				Node			*parentNode;
				Array<Attribute *>	 attributes;
				Array<Node *>		 subnodes;

				Int			 nodeID;
				String			 name;
				String			 content;
			public:
							 Node();
							~Node();

				Document		*GetParentDocument();
				Int			 SetParentDocument(Document *);

				Node			*GetParentNode();
				Int			 SetParentNode(Node *);

				Int			 GetNodeID();
				Int			 SetNodeID(Int);

				Node			*GetNextNode();
				Node			*GetPrevNode();

				Int			 SetNextNode(Node *);
				Int			 SetPrevNode(Node *);

				String			 GetName();
				Int			 SetName(String);

				String			 GetContent();
				Int			 SetContent(String);

				Int			 GetNOfAttributes();
				Attribute		*GetNthAttribute(Int);
				Attribute		*GetAttributeByName(String);

				Attribute		*SetAttribute(String, String);
				Int			 RemoveAttribute(Attribute *);
				Int			 RemoveAttributeByName(String);

				Int			 GetNOfNodes();
				Node			*GetNthNode(Int);
				Node			*GetNodeByName(String);

				Node			*AddNode(String, String = NIL);
				Node			*SetNode(String, String = NIL);
				Int			 RemoveNode(Node *);
				Int			 RemoveNodeByName(String);
		};
	};
};

#endif
