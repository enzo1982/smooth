 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XMLDOCUMENT_
#define _H_OBJSMOOTH_XMLDOCUMENT_

namespace smooth
{
	namespace XML
	{
		class Document;
	};
};

#include "../definitions.h"
#include "../misc/string.h"
#include "node.h"

#include <libxml/parser.h>

namespace smooth
{
	namespace XML
	{
		class SMOOTHAPI Document
		{
			private:
				String	 encoding;
				Node	*rootNode;
				Bool	 ownRoot;

				Int	 LoadNode(xmlNodePtr, Node *);
				Int	 SaveNode(Node *, xmlNodePtr);
			public:
					 Document();
					~Document();

				Node	*GetRootNode();
				Int	 SetRootNode(Node *);

				Int	 SetEncoding(String);

				Int	 LoadFile(String);
				Int	 SaveFile(String);

				Int	 ParseMemory(Void *, Int);
		};
	};
};

#endif
