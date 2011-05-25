 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/xlib/fontxlib.h>
#include <smooth/graphics/font.h>

using namespace X11;

#include <smooth/backends/xlib/backendxlib.h>

S::GUI::FontBackend *CreateFontXLib(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontXLib(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontXLibTmp = S::GUI::FontBackend::SetBackend(&CreateFontXLib);

S::GUI::FontXLib::FontXLib(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_XLIB;
}

S::GUI::FontXLib::~FontXLib()
{
}

S::GUI::Size S::GUI::FontXLib::GetTextSize(const String &text) const
{
	if (text == NIL) return Size();

	Display		*display    = Backends::BackendXLib::GetDisplay();
	XFontStruct	*fontStruct = XLoadQueryFont(display, String("-*-").Append(fontName.ToLower()).Append("-").Append(fontWeight >= Font::Bold ? "bold" : "medium").Append("-").Append(fontStyle & Font::Italic ? "i" : "r").Append("-normal-*-").Append(String::FromInt(fontSize * 96.0 / 72.0)).Append("-*-*-*-*-*-*-*"));

	int		 direction  = 0;
	int		 ascent	    = 0;
	int		 descent    = 0;
	XCharStruct	 overall;

	if (Setup::enableUnicode) XTextExtents16(fontStruct, (XChar2b *) text.ConvertTo("UTF-16BE"), text.Length(), &direction, &ascent, &descent, &overall);
	else			  XTextExtents(fontStruct, text, text.Length(), &direction, &ascent, &descent, &overall);

	XFreeFont(display, fontStruct);

	int		 lines	    = 1;

	for (Int j = 0; j < text.Length(); j++) if (text[j] == '\n') lines++;

	return Size(overall.width, (overall.ascent + overall.descent + 3) * lines);
}
