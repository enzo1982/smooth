 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listentry.h>

S::ListEntry::ListEntry(Int entryID)
{
	id		= entryID;

	checked		= False;
	clicked		= False;

	selected	= False;

	size		= -1;

	font.SetColor(Setup::ClientTextColor);

	onClick.SetParentObject(this);
	onMouseOver.SetParentObject(this);
	onMouseOut.SetParentObject(this);
}

S::ListEntry::~ListEntry()
{
}
