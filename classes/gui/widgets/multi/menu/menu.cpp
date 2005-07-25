 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menu.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/misc/i18n.h>

const S::Int	 S::GUI::Menu::classID = S::Object::RequestClassID();

S::GUI::Menu::Menu()
{
	type		= classID;
	containerType	= classID;
}

S::GUI::Menu::~Menu()
{
	Clear();
}

S::GUI::MenuEntry *S::GUI::Menu::AddEntry(String text, Bitmap bitmap, Menu *popupMenu, Bool *bVar, Int *iVar, Int iCode, Int orientation)
{
	MenuEntry	*newEntry = new MenuEntry(text, bitmap, popupMenu, bVar, iVar, iCode, orientation);

	if (RegisterObject(newEntry) == Success)
	{
		newEntry->Show();

		return newEntry;
	}
	else
	{
		Object::DeleteObject(newEntry);

		return NIL;
	}
}

S::Int S::GUI::Menu::RemoveEntry(MenuEntry *entry)
{
	if (entry == NIL) return Failure;

	if (UnregisterObject(entry) == Success)
	{
		Object::DeleteObject(entry);

		return Success;
	}

	return Failure;
}

S::Int S::GUI::Menu::Clear()
{
	Int	 nOfEntries = assocObjects.GetNOfEntries();

	for (Int i = 0; i < nOfEntries; i++)
	{
		RemoveEntry((MenuEntry *) assocObjects.GetFirstEntry());
	}

	return Success;
}
