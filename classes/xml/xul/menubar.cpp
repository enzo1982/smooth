 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/menubar.h>
#include <smooth/xml/xul/popupmenu.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>

S::XML::XUL::Menubar::Menubar(Node *node) : Widget(node)
{
	menubar = NIL;

	if (node != NIL)
	{
		menubar = new GUI::Menubar();

		for (Int i = 0; i < node->GetNOfNodes(); i++)
		{
			Node	*mNode = node->GetNthNode(i);

			if (mNode->GetName() == "menu")
			{
				XUL::PopupMenu	*menu = new XUL::PopupMenu(mNode);

				menubar->AddEntry(menu->GetName(), NIL, (GUI::PopupMenu *) menu->GetWidget());

				entries.AddEntry(menu);
			}
			else if (mNode->GetName() == "toolbarbutton")
			{
				menubar->AddEntry(GetXMLAttributeValue(mNode, "label"));
			}
		}
	}
}

S::XML::XUL::Menubar::~Menubar()
{
	for (Int i = 0; i < entries.GetNOfEntries(); i++) delete entries.GetNthEntry(i);

	entries.RemoveAll();

	if (menubar != NIL) Object::DeleteObject(menubar);
}

S::GUI::Widget *S::XML::XUL::Menubar::GetWidget() const
{
	return menubar;
}
