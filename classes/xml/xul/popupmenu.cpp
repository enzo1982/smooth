 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/popupmenu.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/multi/menu/menu.h>

S::XML::XUL::PopupMenu::PopupMenu(Node *node) : Widget(node)
{
	menu = NIL;

	if (node != NIL)
	{
		name	= GetXMLAttributeValue(node, "label");

		if (node->GetNthNode(0)->GetName() == "menupopup")
		{
			node = node->GetNthNode(0);

			menu = new GUI::Menu();

			for (Int i = 0; i < node->GetNOfNodes(); i++)
			{
				Node	*entry = node->GetNthNode(i);

				if (entry->GetName() == "menuitem")
				{
					menu->AddEntry(GetXMLAttributeValue(entry, "label"));
				}
				else if (entry->GetName() == "menuseparator")
				{
					menu->AddEntry();
				}
				else if (entry->GetName() == "menu")
				{
					XUL::PopupMenu	*popup = new XUL::PopupMenu(entry);

					menu->AddEntry(popup->GetName(), NIL, (GUI::Menu *) popup->GetWidget());

					entries.AddEntry(popup);
				}
			}
		}
	}
}

S::XML::XUL::PopupMenu::~PopupMenu()
{
	for (Int i = 0; i < entries.GetNOfEntries(); i++) delete entries.GetNthEntry(i);

	entries.RemoveAll();

	if (menu != NIL) delete menu;
}

S::String S::XML::XUL::PopupMenu::GetName()
{
	return name;
}

S::GUI::Widget *S::XML::XUL::PopupMenu::GetWidget()
{
	return menu;
}
