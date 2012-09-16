 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/fontbackend.h>
#include <smooth/misc/math.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/graphics/backends/gdi/fontgdi.h>
#endif

S::GUI::FontBackend *CreateFontBackend(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::GUI::FontBackend *(*S::GUI::FontBackend::backend_creator)(const String &, Short, Short, Short, const Color &) = &CreateFontBackend;

S::Int S::GUI::FontBackend::SetBackend(FontBackend *(*backend)(const String &, Short, Short, Short, const Color &))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::GUI::FontBackend *S::GUI::FontBackend::CreateBackendInstance(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor)
{
	return backend_creator(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::GUI::FontBackend::FontBackend(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) FontGDI();
#endif

	type = FONT_NONE;

	fontName   = iFontName;
	fontSize   = iFontSize;
	fontColor  = iFontColor;
	fontWeight = iFontWeight;
	fontStyle  = iFontStyle;
}

S::GUI::FontBackend::~FontBackend()
{
}

S::Short S::GUI::FontBackend::GetFontType() const
{
	return type;
}

S::Int S::GUI::FontBackend::GetTextSizeX(const String &text, Bool scaled) const
{
	if (text == NIL) return 0;

	Int	 nOfChars = text.Length();
	Int	 sizex	= 0;
	Int	 lines	= 1;
	Int	 offset	= 0;

	for (Int k = 0; k < nOfChars; k++) if (text[k] == 10) lines++;

	for (Int i = 0; i < lines; i++)
	{
		Int	 origOffset = offset;
		String	 line;

		for (Int j = 0; j <= nOfChars; j++)
		{
			if (j + origOffset == nOfChars)
			{
				line[j] = 0;
				break;
			}

			if (text[j + origOffset] == 10 || text[j + origOffset] == 0)
			{
				offset++;
				line[j] = 0;
				break;
			}
			else
			{
				offset++;
				line[j] = text[j + origOffset];
			}
		}

		sizex = (Int) Math::Max(sizex, GetTextSize(line, scaled).cx);
	}

	return sizex;
}

S::Int S::GUI::FontBackend::GetTextSizeY(const String &text, Bool scaled) const
{
	if (text == NIL) return 0;

	Int	 lines		= 1;
	Int	 txtSize	= text.Length();

	for (Int i = 0; i < txtSize; i++)
	{
		if (text[i] == 10) lines++;
	}

	/* Line height is always the same regardless of the actual text,
	 * so we use a default text here and multiply by the numer of lines.
	 */
	static String	 string	= "abcdefghijklmnopqrstuvwxyz";

	return (lines * (GetTextSize(string, scaled).cy - 1)) + (lines - 1) * 3;
}

S::GUI::Size S::GUI::FontBackend::GetTextSize(const String &text, Bool scaled) const
{
	return Size();
}
