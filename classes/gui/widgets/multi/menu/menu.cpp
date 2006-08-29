 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menu.h>

const S::Int	 S::GUI::Menu::classID = S::Object::RequestClassID();

S::GUI::Menu::Menu() : Widget(Point(), Size())
{
	type	= classID;
}

S::GUI::Menu::~Menu()
{
	RemoveAllEntries();
}

S::Int S::GUI::Menu::RemoveEntry(MenuEntry *entry)
{
	if (entry == NIL) return Error();

	if (UnregisterObject(entry) == Success())
	{
		Object::DeleteObject(entry);

		return Success();
	}

	return Error();
}

S::Int S::GUI::Menu::RemoveAllEntries()
{
	Int	 nonMenuEntry = 0;

	while (GetNOfObjects() - nonMenuEntry)
	{
		if (GetNthObject(nonMenuEntry)->GetObjectType() != MenuEntry::classID) { nonMenuEntry++; continue; }

		Widget	*widget = GetNthObject(nonMenuEntry);

		UnregisterObject(widget);
		DeleteObject(widget);
	}

	return Success();
}

S::Int S::GUI::Menu::GetNOfEntries() const
{
	Int	n = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != MenuEntry::classID) continue;

		n++;
	}

	return n;
}

S::GUI::MenuEntry *S::GUI::Menu::GetNthEntry(Int n) const
{
	Int	m = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != MenuEntry::classID) continue;

		if (m == n) return (MenuEntry *) GetNthObject(i);
		else	    m++;
	}

	return NIL;
}
