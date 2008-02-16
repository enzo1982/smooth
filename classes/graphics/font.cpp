 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/font.h>
#include <smooth/misc/math.h>

S::String S::GUI::Font::Default		= "default";
S::Int	  S::GUI::Font::DefaultSize	= 8;

S::Int	  S::GUI::Font::Thin		= 100;
S::Int	  S::GUI::Font::ExtraLight	= 200;
S::Int	  S::GUI::Font::Light		= 300;
S::Int	  S::GUI::Font::Normal		= 400;
S::Int	  S::GUI::Font::Medium		= 500;
S::Int	  S::GUI::Font::SemiBold	= 600;
S::Int	  S::GUI::Font::Bold		= 700;
S::Int	  S::GUI::Font::ExtraBold	= 800;
S::Int	  S::GUI::Font::Black		= 900;

S::Int	  S::GUI::Font::Italic		= 2;
S::Int	  S::GUI::Font::Underline	= 4;
S::Int	  S::GUI::Font::StrikeOut	= 8;

S::Int	  S::GUI::Font::systemFontSize	= -1;

S::GUI::Font::Font(const String &iFontName, Int iFontSize, Int iFontWeight, Int iFontStyle, const Color &iFontColor)
{
	fontName   = iFontName;
	fontSize   = iFontSize;
	fontColor  = iFontColor;
	fontWeight = iFontWeight;
	fontStyle  = iFontStyle;
}

S::GUI::Font::Font(const Font &iFont)
{
	fontName   = iFont.fontName;
	fontSize   = iFont.fontSize;
	fontColor  = iFont.fontColor;
	fontWeight = iFont.fontWeight;
	fontStyle  = iFont.fontStyle;
}

S::GUI::Font::~Font()
{
}

S::GUI::Font &S::GUI::Font::operator =(const Font &newFont)
{
	fontName   = newFont.fontName;
	fontSize   = newFont.fontSize;
	fontColor  = newFont.fontColor;
	fontWeight = newFont.fontWeight;
	fontStyle  = newFont.fontStyle;

	return *this;
}

S::Bool S::GUI::Font::operator ==(const Font &font) const
{
	if (fontName   == font.fontName	  &&
	    fontSize   == font.fontSize	  &&
	    fontColor  == font.fontColor  &&
	    fontWeight == font.fontWeight &&
	    fontStyle  == font.fontStyle) return True;
	else				  return False;
}

S::Bool S::GUI::Font::operator !=(const Font &font) const
{
	return !(*this == font);
}

S::Int S::GUI::Font::SetName(const String &newFontName)
{
	fontName = newFontName;

	return Success();
}

S::Int S::GUI::Font::SetSize(Int newFontSize)
{
	fontSize = newFontSize;

	return Success();
}

S::Int S::GUI::Font::SetColor(const Color &newFontColor)
{
	fontColor = newFontColor;

	return Success();
}

S::Int S::GUI::Font::SetWeight(Int newFontWeight)
{
	fontWeight = newFontWeight;

	return Success();
}

S::Int S::GUI::Font::SetStyle(Int newFontStyle)
{
	fontStyle = newFontStyle;

	return Success();
}

const S::String &S::GUI::Font::GetName() const
{
	return fontName;
}

S::Int S::GUI::Font::GetSize() const
{
	return fontSize;
}

const S::GUI::Color &S::GUI::Font::GetColor() const
{
	return fontColor;
}

S::Int S::GUI::Font::GetWeight() const
{
	return fontWeight;
}

S::Int S::GUI::Font::GetStyle() const
{
	return fontStyle;
}

S::Int S::GUI::Font::GetSystemFontSize()
{
	if (systemFontSize != -1) return systemFontSize;

#ifdef __WIN32__
	HDC	 dc = GetWindowDC(0);
	Int	 dpi = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(0, dc);

	systemFontSize = dpi;

	return dpi;
#else
	return 96;
#endif
}

S::Int S::GUI::Font::GetTextSizeX(const String &text) const
{
	if (text == NIL) return 0;

	return GetTextSizeX(text, text.Length());
}

S::Int S::GUI::Font::GetTextSizeX(const String &text, Int nOfChars) const
{
	if (text == NIL)	return 0;
	if (nOfChars == 0)	return 0;

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

		sizex = (Int) Math::Max(sizex, GetLineSizeX(line, line.Length()));
	}

	return sizex;
}

S::Int S::GUI::Font::GetLineSizeX(const String &text, Int nOfChars) const
{
	if (text == NIL)	return 0;
	if (nOfChars == 0)	return 0;

#ifdef __WIN32__
	HDC	 dc	= CreateCompatibleDC(NIL);
	HFONT	 hFont;
	HFONT	 hOldFont;

	if (Setup::enableUnicode)	hFont = CreateFontW(-Math::Round(fontSize * 96.0 / 72.0), 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);
	else				hFont = CreateFontA(-Math::Round(fontSize * 96.0 / 72.0), 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);

	hOldFont = (HFONT) SelectObject(dc, hFont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(dc, text, nOfChars, &tSize);
	else				GetTextExtentPoint32A(dc, text, nOfChars, &tSize);

	SelectObject(dc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(dc);

	return tSize.cx;
#else
	return 0;
#endif
}

S::Int S::GUI::Font::GetTextSizeY(const String &text) const
{
	if (text == NIL) return 0;

	Int	 lines		= 1;
	Int	 txtSize	= text.Length();

	for (Int i = 0; i < txtSize; i++)
	{
		if (text[i] == 10) lines++;
	}

	return (lines * GetLineSizeY(text)) + (lines - 1) * 3;
}

S::Int S::GUI::Font::GetLineSizeY(const String &text) const
{
	if (text == NIL) return 0;

#ifdef __WIN32__
	HDC	 cdc	= CreateCompatibleDC(NIL);
	HFONT	 hFont;
	HFONT	 hOldFont;

	if (Setup::enableUnicode)	hFont = CreateFontW(-Math::Round(fontSize * 96.0 / 72.0), 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);
	else				hFont = CreateFontA(-Math::Round(fontSize * 96.0 / 72.0), 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);

	hOldFont = (HFONT) SelectObject(cdc, hFont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, text.Length(), &tSize);
	else				GetTextExtentPoint32A(cdc, text, text.Length(), &tSize);

	SelectObject(cdc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(cdc);

	return tSize.cy - 1;
#else
	return 0;
#endif
}
