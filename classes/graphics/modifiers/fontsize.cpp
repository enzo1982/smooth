 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/modifiers/fontsize.h>

S::GUI::FontSizeModifier::FontSizeModifier()
{
	fontSize = 96;
}

S::GUI::FontSizeModifier::~FontSizeModifier()
{
}

S::Int S::GUI::FontSizeModifier::TranslateX(Int x) const
{
	return x * fontSize / 96;
}

S::Int S::GUI::FontSizeModifier::TranslateY(Int y) const
{
	return y * fontSize / 96;
}

S::GUI::Point S::GUI::FontSizeModifier::TranslatePoint(const Point &p) const
{
	return p * fontSize / 96;
}

S::GUI::Rect S::GUI::FontSizeModifier::TranslateRect(const Rect &r) const
{
	return r * fontSize / 96;
}

S::Void S::GUI::FontSizeModifier::SetFontSize(Int nFontSize)
{
	fontSize = nFontSize;
}

S::Int S::GUI::FontSizeModifier::GetFontSize() const
{
	return fontSize;
}
