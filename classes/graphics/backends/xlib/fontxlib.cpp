 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/xlib/fontxlib.h>
#include <smooth/graphics/surface.h>

#include <fribidi.h>

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

S::GUI::Size S::GUI::FontXLib::GetTextSize(const String &iText, Bool scaled) const
{
	if (iText == NIL) return Size();

	String	 text = iText;
	Float	 dpi  = Surface().GetSurfaceDPI();

	Bool	 rtlCharacters = False;

	for (Int i = 0; i < text.Length(); i++) if (text[i] >= 0x0590 && text[i] <= 0x08FF) { rtlCharacters = True; break; }

	if (rtlCharacters && Setup::useIconv)
	{
		/* Reorder the string with fribidi.
		 */
		FriBidiChar	*visual = new FriBidiChar [text.Length() + 1];
		FriBidiParType	 type = FRIBIDI_PAR_RTL;

		fribidi_log2vis((FriBidiChar *) text.ConvertTo("UCS-4LE"), text.Length(), &type, visual, NIL, NIL, NIL);

		visual[text.Length()] = 0;

		text.ImportFrom("UCS-4LE", (char *) visual);

		delete [] visual;
	}

	Display		*display = Backends::BackendXLib::GetDisplay();
	XftFont		*font	 = XftFontOpenName(display, XDefaultScreen(display), String(fontName).Append("-").Append(String::FromInt(Math::Round(fontSize * dpi / 96.0))).Append(":").Append(fontWeight >= Font::Bold ? "bold" : "medium").Append(fontStyle & Font::Italic ? ":italic" : ""));
	XGlyphInfo	 extents = { 0 };

	XftTextExtents16(display, font, (XftChar16 *) text.ConvertTo("UCS2"), text.Length(), &extents);

	XftFontClose(display, font);

	int		 lines	    = 1;

	for (Int j = 0; j < text.Length(); j++) if (text[j] == '\n') lines++;

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(extents.width - 2, (font->ascent + 2) * lines);
	else					   return Size(extents.width - 2, (font->ascent + 2) * lines) * 96.0 / dpi;
}
