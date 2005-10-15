 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/hotspot.h>

const S::Int	 S::GUI::Hotspot::classID = S::Object::RequestClassID();

S::GUI::Hotspot::Hotspot(const Point &iPos, const Size &iSize)
{
	type		= classID;

	possibleContainers.AddEntry(Widget::classID);

	pos		= iPos;
	size		= iSize;
}

S::GUI::Hotspot::~Hotspot()
{
}
