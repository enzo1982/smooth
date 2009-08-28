 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/textbox.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/gui/widgets/basic/multiedit.h>

S::XML::XUL::TextBox::TextBox(Node *node) : Widget(node)
{
	editBox = NIL;

	if (node != NIL)
	{
		if (GetXMLAttributeValue(node, "multiline") == "true")	editBox = new GUI::MultiEdit(GetXMLAttributeValue(node, "value"), GUI::Point(0, 0), GUI::Size(0, 0), GetXMLAttributeValue(node, "maxlength").ToInt());
		else							editBox = new GUI::EditBox(GetXMLAttributeValue(node, "value"), GUI::Point(0, 0), GUI::Size(0, 0), GetXMLAttributeValue(node, "maxlength").ToInt());

		if (GetXMLAttributeValue(node, "disabled") == "true")	editBox->Deactivate();
		if (GetXMLAttributeValue(node, "type") == "password")	editBox->SetFlags(editBox->GetFlags() | GUI::EDB_ASTERISK);
	}
}

S::XML::XUL::TextBox::~TextBox()
{
	if (editBox != NIL) Object::DeleteObject(editBox);
}

S::GUI::Widget *S::XML::XUL::TextBox::GetWidget() const
{
	return editBox;
}
