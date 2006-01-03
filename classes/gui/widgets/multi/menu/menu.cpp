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
	Clear();
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

S::Int S::GUI::Menu::Clear()
{
	while (GetNOfObjects()) RemoveEntry((MenuEntry *) GetNthObject(0));

	return Success();
}
