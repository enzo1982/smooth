 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/haiku/fonthaiku.h>
#include <smooth/graphics/surface.h>

#include <View.h>

S::GUI::FontBackend *CreateFontHaiku(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontHaiku(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontHaikuTmp = S::GUI::FontBackend::SetBackend(&CreateFontHaiku);

S::GUI::FontHaiku::FontHaiku(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_HAIKU;
}

S::GUI::FontHaiku::~FontHaiku()
{
}

S::GUI::Size S::GUI::FontHaiku::GetTextSize(const String &text, Bool scaled) const
{
	if (text == NIL) return Size();

	Float	 dpi  = Surface().GetSurfaceDPI();

	BView		 view(BRect(0, 0, 100, 100), NULL, B_FOLLOW_ALL_SIDES, 0);
	BFont		 font;
	font_height	 height;
	Size		 size;

	font.SetFamilyAndStyle(fontName, NULL);
	font.SetFace((fontStyle & Font::Italic ? B_ITALIC_FACE : 0) |
		     (fontWeight >= Font::Bold ? B_BOLD_FACE   : 0));
	font.SetSize(Math::Round(fontSize * dpi / 96.0));

	view.SetFont(&font);
	view.GetFontHeight(&height);

	size = Size(view.StringWidth(text.ConvertTo("UTF-8")), Math::Ceil((height.ascent + height.descent + height.leading) * 96.0 / 72.0));

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return size;
	else					   return size * 96.0 / dpi;
}
