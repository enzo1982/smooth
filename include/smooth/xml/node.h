 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLNODE_
#define _H_OBJSMOOTH_XMLNODE_

#define SXMLNode SMOOTHXMLNode

class SMOOTHXMLNode;

#include "../definitions.h"
#include "attribute.h"
#include "document.h"

class SMOOTHAPI SMOOTHXMLNode
{
	private:
		SMOOTHXMLNode				*nextNode;
		SMOOTHXMLNode				*prevNode;

		SMOOTHXMLDocument			*parentDoc;
		SMOOTHXMLNode				*parentNode;
		SMOOTHArray<SMOOTHXMLAttribute *>	 attributes;
		SMOOTHArray<SMOOTHXMLNode *>		 subnodes;

		SMOOTHInt				 nodeID;
		SMOOTHString				 name;
		SMOOTHString				 content;
	public:
							 SMOOTHXMLNode();
							~SMOOTHXMLNode();

		SMOOTHXMLDocument			*GetParentDocument();
		SMOOTHInt				 SetParentDocument(SMOOTHXMLDocument *);

		SMOOTHXMLNode				*GetParentNode();
		SMOOTHInt				 SetParentNode(SMOOTHXMLNode *);

		SMOOTHInt				 GetNodeID();
		SMOOTHInt				 SetNodeID(SMOOTHInt);

		SMOOTHXMLNode				*GetNextNode();
		SMOOTHXMLNode				*GetPrevNode();

		SMOOTHInt				 SetNextNode(SMOOTHXMLNode *);
		SMOOTHInt				 SetPrevNode(SMOOTHXMLNode *);

		SMOOTHString				 GetName();
		SMOOTHInt				 SetName(SMOOTHString);

		SMOOTHString				 GetContent();
		SMOOTHInt				 SetContent(SMOOTHString);

		SMOOTHInt				 GetNOfAttributes();
		SMOOTHXMLAttribute			*GetNthAttribute(SMOOTHInt);
		SMOOTHXMLAttribute			*GetAttributeByName(SMOOTHString);

		SMOOTHXMLAttribute			*SetAttribute(SMOOTHString, SMOOTHString);
		SMOOTHInt				 RemoveAttribute(SMOOTHXMLAttribute *);
		SMOOTHInt				 RemoveAttributeByName(SMOOTHString);

		SMOOTHInt				 GetNOfNodes();
		SMOOTHXMLNode				*GetNthNode(SMOOTHInt);
		SMOOTHXMLNode				*GetNodeByName(SMOOTHString);

		SMOOTHXMLNode				*AddNode(SMOOTHString, SMOOTHString = NIL);
		SMOOTHXMLNode				*SetNode(SMOOTHString, SMOOTHString = NIL);
		SMOOTHInt				 RemoveNode(SMOOTHXMLNode *);
		SMOOTHInt				 RemoveNodeByName(SMOOTHString);
};

#endif
