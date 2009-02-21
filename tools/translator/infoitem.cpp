 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "infoitem.h"

const S::Int	 InfoItem::classID = S::Object::RequestClassID();

InfoItem::InfoItem(const String &iName, const String &iValue) : ListEntry(NIL)
{
	type	= classID;

	name	= iName;
	value	= iValue;

	UpdateText();
}

InfoItem::~InfoItem()
{
}

Int InfoItem::UpdateText()
{
	SetText(String(name).Append(": ").Append(value));

	return Success();
}

Bool InfoItem::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == ListEntry::classID)	return True;
	else													return False;
}
