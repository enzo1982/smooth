 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listentryseparator.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::ListEntrySeparator::classID = S::Object::RequestClassID();

S::GUI::ListEntrySeparator::ListEntrySeparator() : ListEntry(NIL)
{
	type		= classID;

	font.SetColor(Setup::GrayTextColor);

	SetHeight(3);

	Remove(hotspot);
	Remove(markHotspot);

	internalOnSelectEntry.Disconnect(&ListEntry::OnSelectOtherEntry, this);
}

S::GUI::ListEntrySeparator::~ListEntrySeparator()
{
	if (IsRegistered() && container != NIL) container->Remove(this);
}

S::Int S::GUI::ListEntrySeparator::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->Box(frame + Point(4, 1) - Size(8, 2), font.GetColor(), Rect::Filled);
			}
	}

	return Widget::Paint(message);
}

S::Bool S::GUI::ListEntrySeparator::IsTypeCompatible(Short compType) const
{
	if (compType == Object::classID	   ||
	    compType == Widget::classID	   ||
	    compType == ListEntry::classID) return True;
	else				    return False;
}
