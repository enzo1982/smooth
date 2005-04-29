 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/window.h>
#include <smooth/xml/xul/menubar.h>
#include <smooth/xml/xul/box.h>
#include <smooth/xml/node.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/titlebar.h>

S::XML::XUL::Window::Window(Node *node) : Widget(node)
{
	titlebar	= NIL;
	window		= NIL;

	if (node != NIL)
	{
		window = new GUI::Window(GetXMLAttributeValue(node, "title"));
		window->SetMetrics(GUI::Point(GetXMLAttributeValue(node, "screenX").ToInt(), GetXMLAttributeValue(node, "screenY").ToInt()), GUI::Size(GetXMLAttributeValue(node, "width").ToInt(), GetXMLAttributeValue(node, "height").ToInt()));

		if (GetXMLAttributeValue(node, "hidechrome") != "true" && GetXMLAttributeValue(node, "hidechrome") != "1")
		{
			titlebar = new GUI::Titlebar();

			window->RegisterObject(titlebar);
		}

		if (GetXMLAttributeValue(node, "sizemode") == "maximized") window->Maximize();
		if (GetXMLAttributeValue(node, "sizemode") == "minimized") window->Minimize();

		for (Int i = 0; i < node->GetNOfNodes(); i++)
		{
			Node	*nNode = node->GetNthNode(i);

			if (nNode->GetName() == "toolbox")
			{
				for (Int j = 0; j < nNode->GetNOfNodes(); j++)
				{
					Node	*tNode = nNode->GetNthNode(j);

					if (tNode->GetName() == "menubar" || tNode->GetName() == "toolbar")
					{
						XUL::Menubar	*menu = new XUL::Menubar(tNode);

						window->RegisterObject(menu->GetWidget());

						widgets.AddEntry(menu);
					}
				}
			}
		}

		XUL::Box	*box = new XUL::Box(node, GetXMLAttributeValue(node, "orient") == "horizontal");

		window->RegisterObject(box->GetWidget());

		widgets.AddEntry(box);
	}
}

S::XML::XUL::Window::~Window()
{
	for (Int i = 0; i < widgets.GetNOfEntries(); i++) delete widgets.GetNthEntry(i);

	widgets.RemoveAll();

	if (titlebar != NIL)	Object::DeleteObject(titlebar);
	if (window != NIL)	Object::DeleteObject(window);
}

S::GUI::Widget *S::XML::XUL::Window::GetWidget()
{
	return window;
}
