 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_XULWIDGET_
#define _H_OBJSMOOTH_XULWIDGET_

namespace smooth
{
	namespace XML
	{
		namespace XUL
		{
			class Widget;
		};

		class Node;
	};

	namespace GUI
	{
		class Widget;
	};
};

#include "../../definitions.h"
#include "../../misc/string.h"

namespace smooth
{
	namespace XML
	{
		namespace XUL
		{
			class SMOOTHAPI Widget
			{
				protected:
					String			 id;

					String			 GetXMLAttributeValue(Node *, const String &);
				public:
								 Widget();
					virtual			~Widget();

					virtual GUI::Widget	*GetWidget();
			};
		};
	};
};

#endif
