 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLDOCUMENT_
#define _H_OBJSMOOTH_XMLDOCUMENT_

#define SXMLDocument SMOOTHXMLDocument

class SMOOTHXMLDocument;

#include <libxml/parser.h>

#include "../definitions.h"
#include "node.h"

class SMOOTHAPI SMOOTHXMLDocument
{
	private:
		SMOOTHXMLNode	*rootNode;
		SMOOTHInt	 LoadNode(xmlNodePtr, SMOOTHXMLNode *);
		SMOOTHInt	 SaveNode(SMOOTHXMLNode *, xmlNodePtr);
	public:
				 SMOOTHXMLDocument();
				~SMOOTHXMLDocument();

		SMOOTHXMLNode	*GetRootNode();
		SMOOTHInt	 SetRootNode(SMOOTHXMLNode *);

		SMOOTHInt	 LoadFile(SMOOTHString);
		SMOOTHInt	 SaveFile(SMOOTHString);
};

#endif
