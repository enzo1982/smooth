 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <windows.h>
#include <gdiplus.h>

#include <smooth/graphics/backends/gdiplus/fontgdiplus.h>
#include <smooth/graphics/font.h>

S::GUI::FontBackend *CreateFontGDIPlus(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontGDIPlus(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontGDITmp = S::GUI::FontBackend::SetBackend(&CreateFontGDIPlus);

S::GUI::FontGDIPlus::FontGDIPlus(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_GDIPLUS;
}

S::GUI::FontGDIPlus::~FontGDIPlus()
{
}

S::GUI::Size S::GUI::FontGDIPlus::GetTextSize(const String &text) const
{
	if (text == NIL) return Size();

	Gdiplus::Graphics	 context((HWND) NIL);

	Gdiplus::Font		 gdip_font(fontName, fontSize, fontWeight >= Font::Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	Gdiplus::StringFormat	 gdip_format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
	Gdiplus::RectF		 rect;

	gdip_format.SetAlignment(Gdiplus::StringAlignmentNear);

	context.MeasureString(text, -1, &gdip_font, Gdiplus::PointF(0.0, 0.0), &gdip_format, &rect);

	return Size(rect.Width, rect.Height);
}
