 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "section.h"
#include "stringitem.h"

Signal1<Void, ListEntry *>	 Section::onSelectItem;

Section::Section()
{
	list = NIL;
}

Section::~Section()
{
	foreach (StringItem *item, items) delete item;
	foreach (Section *section, sections) delete section;
}

Bool Section::Parse(XML::Node *section, List *plist, Array<ListEntry *> &entries)
{
	if (section == NIL) return False;

	list = plist;

	if (section->GetAttributeByName("name") != NIL) name = section->GetAttributeByName("name")->GetContent();

	for (Int i = 0; i < section->GetNOfNodes(); i++)
	{
		XML::Node	*entry = section->GetNthNode(i);

		if (entry->GetName() == "entry")
		{
			GUI::Font	 blackFont;
			GUI::Font	 redFont;
			GUI::Font	 orangeFont;

			redFont.SetColor(Color(255, 0, 0));
			orangeFont.SetColor(Color(255, 127, 36));

			if (entries.Get(entry->GetAttributeByName("id")->GetContent().ToInt()) != NIL)
			{
				StringItem	*item = (StringItem *) entries.Get(entry->GetAttributeByName("id")->GetContent().ToInt());

				item->SetTranslation(entry->GetContent());

				if	(item->GetTranslation() == NIL)						   item->SetFont(redFont);
				else if (item->GetOriginal() != entry->GetAttributeByName("string")->GetContent()) item->SetFont(orangeFont);
				else										   item->SetFont(blackFont);
			}
			else
			{
				StringItem	*item = new StringItem(entry->GetAttributeByName("id")->GetContent().ToInt(),
								       entry->GetAttributeByName("string")->GetContent(),
								       entry->GetContent());

				if (item->GetTranslation() == NIL) item->SetFont(redFont);
				else				   item->SetFont(orangeFont);

				list->Add(item);

				entries.Add(item, item->GetID());

				items.Add(item, item->GetID());
			}
		}
		else if (entry->GetName() == "section")
		{
			if (sections.Get(entry->GetAttributeByName("name")->GetContent().ComputeCRC32()) == NIL)
			{
				Section	*section = new Section();
				Tree	*tree	 = new Tree(entry->GetAttributeByName("name")->GetContent());

				section->Parse(entry, tree->GetList(), entries);

				tree->Open();
				list->Add(tree);

				sections.Add(section, section->GetName().ComputeCRC32());
			}
			else
			{
				Section	*section = sections.Get(entry->GetAttributeByName("name")->GetContent().ComputeCRC32());

				section->Parse(entry, section->list, entries);
			}
		}
	}

	list->onSelectEntry.Connect(&Section::OnSelectEntry, this);

	return True;
}

Bool Section::Save(XML::Node *xmlSection, const String &templateName)
{
	for (Int i = 0; i < items.Length(); i++)
	{
		StringItem	*item = items.GetNth(i);

		/* If we have a template, save only
		 * entries with a translation.
		 */
		if (templateName == NIL || item->GetTranslation() != NIL)
		{
			XML::Node	*node = xmlSection->AddNode("entry", item->GetTranslation());

			node->SetAttribute("id", String::FromInt(item->GetID()));
			node->SetAttribute("string", item->GetOriginal());
		}
	}

	foreach (Section *section, sections)
	{
		XML::Node	*node = xmlSection->AddNode("section");

		node->SetAttribute("name", section->GetName());

		section->Save(node, templateName);
	}

	return True;
}

Void Section::OnSelectEntry(ListEntry *entry)
{
	onSelectItem.Emit(entry);
}