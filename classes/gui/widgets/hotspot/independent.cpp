 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/hotspot/independent.h>

const S::Int	 S::GUI::IndependentHotspot::classID = S::Object::RequestClassID();

S::GUI::IndependentHotspot::IndependentHotspot(const Point &iPos, const Size &iSize) : Hotspot(iPos, iSize)
{
	type	= classID;
}

S::GUI::IndependentHotspot::~IndependentHotspot()
{
}

S::GUI::Rect S::GUI::IndependentHotspot::GetVisibleArea() const
{
	if (!IsRegistered()) return Rect();

	return Rect(GetRealPosition(), GetSize());
}
