 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "stringitem.h"

const S::Short	 StringItem::classID = S::Object::RequestClassID();

StringItem::StringItem(Int iID, const String &iOriginal, const String &iTranslation) : ListEntry(NIL)
{
	type		= classID;

	id		= iID;

	original	= iOriginal;
	translation	= iTranslation;

	UpdateText();
}

StringItem::~StringItem()
{
}

Int StringItem::UpdateText()
{
	SetText(String::FromInt(id).Append(ListEntry::tabDelimiter).Append(original).Append(ListEntry::tabDelimiter).Append(translation));

	return Success();
}

Bool StringItem::IsTypeCompatible(Short compType) const
{
	if (compType == ListEntry::classID) return True;
	else				    return ListEntry::IsTypeCompatible(compType);
}
