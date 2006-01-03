 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/description.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/basic/text.h>

S::XML::XUL::Description::Description(Node *node) : Widget(node)
{
	text = NIL;

	if (node != NIL)
	{
		text = new GUI::Text(node->GetContent(), GUI::Point(0, 0));
	}
}

S::XML::XUL::Description::~Description()
{
	if (text != NIL) Object::DeleteObject(text);
}

S::GUI::Widget *S::XML::XUL::Description::GetWidget()
{
	return text;
}
