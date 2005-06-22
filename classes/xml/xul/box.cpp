 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/box.h>
#include <smooth/xml/xul/button.h>
#include <smooth/xml/xul/textbox.h>
#include <smooth/xml/xul/label.h>
#include <smooth/xml/xul/box.h>
#include <smooth/xml/xul/description.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/layer.h>

S::XML::XUL::Box::Box(Node *node) : Widget(node)
{
	layer = NIL;

	if (node != NIL)
	{
		layer = new GUI::Layer();

		if (node->GetName() == "hbox")		orient = HORIZONTAL;
		else if (node->GetName() == "vbox")	orient = VERTICAL;

		for (Int i = 0; i < node->GetNOfNodes(); i++)
		{
			Node	*nNode = node->GetNthNode(i);

			if (nNode->GetName() == "hbox" || nNode->GetName() == "vbox")
			{
				XUL::Box	*box = new XUL::Box(nNode);

				layer->RegisterObject(box->GetWidget());

				widgets.AddEntry(box);
			}
			else if (nNode->GetName() == "button")
			{
				XUL::Button	*button = new XUL::Button(nNode);

				layer->RegisterObject(button->GetWidget());

				widgets.AddEntry(button);
			}
			else if (nNode->GetName() == "textbox")
			{
				XUL::TextBox	*textBox = new XUL::TextBox(nNode);

				layer->RegisterObject(textBox->GetWidget());

				widgets.AddEntry(textBox);
			}
			else if (nNode->GetName() == "label")
			{
				XUL::Label	*label = new XUL::Label(nNode);

				layer->RegisterObject(label->GetWidget());

				widgets.AddEntry(label);
			}
			else if (nNode->GetName() == "description")
			{
				XUL::Description	*description = new XUL::Description(nNode);

				layer->RegisterObject(description->GetWidget());

				widgets.AddEntry(description);
			}
		}
	}
}

S::XML::XUL::Box::~Box()
{
	for (Int i = 0; i < widgets.GetNOfEntries(); i++) delete widgets.GetNthEntry(i);

	widgets.RemoveAll();

	if (layer != NIL) Object::DeleteObject(layer);
}

S::GUI::Widget *S::XML::XUL::Box::GetWidget()
{
	return layer;
}
