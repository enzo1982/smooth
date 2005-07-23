 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/renderer.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/widget.h>

const S::Int S::XML::XUL::Widget::HORIZONTAL	= 0;
const S::Int S::XML::XUL::Widget::VERTICAL	= 1;

S::XML::XUL::Widget::Widget(Node *node)
{
	if (node != NIL)
	{
		id = GetXMLAttributeValue(node, "id");

		orient = (GetXMLAttributeValue(node, "orient") == "vertical") ? VERTICAL : HORIZONTAL;
	}
}

S::XML::XUL::Widget::~Widget()
{
}

S::GUI::Widget *S::XML::XUL::Widget::GetWidget()
{
	return NIL;
}

S::String S::XML::XUL::Widget::GetXMLAttributeValue(Node *node, const String &attribute)
{
	if (node != NIL)
	{
		if (node->GetAttributeByName(attribute) != NIL)
		{
			return node->GetAttributeByName(attribute)->GetContent();
		}
	}

	return NIL;
}
