 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/menu/menu.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/color.h>
#include <smooth/system.h>
#include <smooth/metrics.h>
#include <smooth/i18n.h>
#include <smooth/objectproperties.h>

const S::Int	 S::GUI::Menu::classID = S::Object::RequestClassID();

S::GUI::Menu::Menu()
{
	self = this;

	type		= classID;
	containerType	= classID;

	sizeset = False;
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
		sizeset = False;

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
	if (entry == NIL) return Error;

	if (UnregisterObject(entry) == Success)
	{
		Object::DeleteObject(entry);

		return Success;
	}

	return Error;
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

S::Void S::GUI::Menu::GetSize()
{
	if (!sizeset)
	{
		popupsize.cx = GetSizeX();
		popupsize.cy = GetSizeY();

		sizeset = True;
	}
}

S::Int S::GUI::Menu::GetSizeX()
{
	Int	 mSize = 50;
	Int	 greatest = 0;

	if (assocObjects.GetNOfEntries() == 0) return mSize;

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (entry->GetObjectProperties()->textSize.cx > greatest)
		{
			mSize		= 50 + entry->GetObjectProperties()->textSize.cx;
			greatest	= entry->GetObjectProperties()->textSize.cx;
		}
	}

	return mSize;
}

S::Int S::GUI::Menu::GetSizeY()
{
	Int	 mSize = 4;

	if (assocObjects.GetNOfEntries() == 0) return mSize;

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (entry->type == SM_SEPARATOR)	mSize = mSize + 5;
		else					mSize = mSize + METRIC_POPUPENTRYSIZE;
	}

	return mSize;
}

S::Int S::GUI::Menu::GetNOfEntries()
{
	return assocObjects.GetNOfEntries();
}
