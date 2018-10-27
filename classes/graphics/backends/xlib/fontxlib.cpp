 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/xlib/fontxlib.h>
#include <smooth/graphics/surface.h>
#include <smooth/files/file.h>
#include <smooth/foreach.h>
#include <smooth/init.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <fribidi/fribidi.h>

using namespace X11;

#include <smooth/backends/xlib/backendxlib.h>

S::GUI::FontBackend *CreateFontXLib(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontXLib(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontXLibTmp = S::GUI::FontBackend::SetBackend(&CreateFontXLib);

S::Int	 addFontXLibInitTmp = S::AddInitFunction(&S::GUI::FontXLib::Initialize);

S::Int S::GUI::FontXLib::Initialize()
{
	Font::Default = "Helvetica";

	/* Search the path for gsettings.
	 */
	String			 path  = getenv("PATH");
	const Array<String>	&paths = path.Explode(":");

	foreach (const String &path, paths)
	{
		/* Check for gsettings in this path.
		 */
		if (File(String(path).Append("/").Append("gsettings")).Exists())
		{
			/* If gsettings exists, use it to get the default font.
			 */
			FILE	*pstdin = popen("gsettings get org.gnome.desktop.interface font-name", "r");

			if (pstdin != NIL)
			{
				char	 fontName[256];

				if (fgets(fontName, 256, pstdin) != NIL)
				{
					String	 font = fontName;

					Font::Default = font.SubString(1, font.FindLast(" ") - 1);
				}

				pclose(pstdin);
			}

			break;
		}
	}

	String::ExplodeFinish();

	return Success();
}

S::GUI::FontXLib::FontXLib(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_XLIB;
}

S::GUI::FontXLib::~FontXLib()
{
}

S::GUI::Size S::GUI::FontXLib::GetTextSize(const String &iText) const
{
	if (iText == NIL) return Size();

	Display	*display       = Backends::BackendXLib::GetDisplay();

	if (display == NIL) return Size();

	String	 text	       = iText;
	Int	 textLength    = text.Length();

	Float	 dpi	       = Surface().GetSurfaceDPI();

	Bool	 rtlCharacters = False;

	for (Int i = 0; i < textLength; i++) if (text[i] >= 0x0590 && text[i] <= 0x08FF) { rtlCharacters = True; break; }

	if (rtlCharacters && Setup::useIconv)
	{
		/* Reorder the string with fribidi.
		 */
		FriBidiChar	*visual = new FriBidiChar [textLength + 1];
		FriBidiParType	 type	= FRIBIDI_PAR_RTL;

		fribidi_log2vis((FriBidiChar *) text.ConvertTo("UCS-4LE"), textLength, &type, visual, NIL, NIL, NIL);

		visual[textLength] = 0;

		text.ImportFrom("UCS-4LE", (char *) visual);

		delete [] visual;
	}

	/* Get text extents.
	 */
	XftFont		*font	 = XftFontOpenName(display, XDefaultScreen(display), String(fontName).Append("-").Append(String::FromInt(Math::Round(fontSize * dpi / 96.0))).Append(":").Append(fontWeight >= Font::Bold ? "bold" : "medium").Append(fontStyle & Font::Italic ? ":italic" : ""));
	XGlyphInfo	 extents = { 0 };

	XftTextExtents16(display, font, (XftChar16 *) text.ConvertTo("UCS2"), textLength, &extents);

	XftFontClose(display, font);

	int		 lines	 = 1;

	for (Int i = 0; i < textLength; i++) if (text[i] == '\n') lines++;

	return Size(extents.width - 2, (font->ascent + 2) * lines);
}
