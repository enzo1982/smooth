 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/misc/math.h>
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
			Node		*nNode	= node->GetNthNode(i);
			XUL::Widget	*widget	= NIL;

			if (nNode->GetName() == "hbox" || nNode->GetName() == "vbox")
			{
				widget = new XUL::Box(nNode);

				layer->RegisterObject(widget->GetWidget());

				widgets.AddEntry(widget);
			}
			else if (nNode->GetName() == "button")
			{
				widget = new XUL::Button(nNode);

				layer->RegisterObject(widget->GetWidget());

				widgets.AddEntry(widget);
			}
			else if (nNode->GetName() == "textbox")
			{
				widget = new XUL::TextBox(nNode);

				layer->RegisterObject(widget->GetWidget());

				widgets.AddEntry(widget);
			}
			else if (nNode->GetName() == "label")
			{
				widget = new XUL::Label(nNode);

				layer->RegisterObject(widget->GetWidget());

				widgets.AddEntry(widget);
			}
			else if (nNode->GetName() == "description")
			{
				widget = new XUL::Description(nNode);

				layer->RegisterObject(widget->GetWidget());

				widgets.AddEntry(widget);
			}
		}

		CalculateChildMetrics();
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

S::Void	 S::XML::XUL::Box::CalculateChildMetrics()
{
	if (widgets.GetNOfEntries() == 0) return;

	Int	 xOffset = 0;
	Int	 yOffset = 0;

	Int	 maxWidth = 0;
	Int	 maxHeight = 0;

	Int	 elementWidth = width / widgets.GetNOfEntries();
	Int	 elementHeight = height / widgets.GetNOfEntries();

	for (Int i = 0; i < widgets.GetNOfEntries(); i++)
	{
		XUL::Widget	*widget = widgets.GetNthEntry(i);

		maxWidth = Math::Max(maxWidth, widget->GetWidth());
		maxHeight = Math::Max(maxHeight, widget->GetHeight());

		Int	 usedElementWidth = widget->GetWidth();
		Int	 usedElementHeight = widget->GetHeight();

		if (usedElementWidth == 0) usedElementWidth = (orient == HORIZONTAL) ? elementWidth : widget->GetDefaultWidth();
		if (usedElementHeight == 0) usedElementHeight = (orient == VERTICAL) ? elementHeight : widget->GetDefaultHeight();

		widget->SetMetrics(GUI::Point(xOffset, yOffset), GUI::Size(usedElementWidth, usedElementHeight));

		if (orient == HORIZONTAL)	xOffset += usedElementWidth;
		else if (orient == VERTICAL)	yOffset += usedElementHeight;
	}
}
