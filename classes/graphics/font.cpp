 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/font.h>
#include <smooth/misc/math.h>

S::GUI::Font::Font(const String &iFontName, Int iFontSize, Int iFontColor, Int iFontWeight, Bool iFontItalic, Bool iFontUnderline, Bool iFontStrikeOut)
{
	fontName	= iFontName;
	fontSize	= iFontSize;
	fontColor	= iFontColor;
	fontWeight	= iFontWeight;
	fontItalic	= iFontItalic;
	fontUnderline	= iFontUnderline;
	fontStrikeOut	= iFontStrikeOut;
}

S::GUI::Font::Font(const Font &iFont)
{
	fontName	= iFont.fontName;
	fontSize	= iFont.fontSize;
	fontColor	= iFont.fontColor;
	fontWeight	= iFont.fontWeight;
	fontItalic	= iFont.fontItalic;
	fontUnderline	= iFont.fontUnderline;
	fontStrikeOut	= iFont.fontStrikeOut;
}

S::GUI::Font::~Font()
{
}

S::GUI::Font &S::GUI::Font::operator =(const Font &newFont)
{
	fontName	= newFont.fontName;
	fontSize	= newFont.fontSize;
	fontColor	= newFont.fontColor;
	fontWeight	= newFont.fontWeight;
	fontItalic	= newFont.fontItalic;
	fontUnderline	= newFont.fontUnderline;
	fontStrikeOut	= newFont.fontStrikeOut;

	return *this;
}

S::Bool S::GUI::Font::operator ==(const Font &font) const
{
	if (fontName == font.fontName		&&
	    fontSize == font.fontSize		&&
	    fontColor == font.fontColor		&&
	    fontWeight == font.fontWeight	&&
	    fontItalic == font.fontItalic	&&
	    fontUnderline == font.fontUnderline	&&
	    fontStrikeOut == font.fontStrikeOut)	return True;
	else						return False;
}

S::Bool S::GUI::Font::operator !=(const Font &font) const
{
	if (fontName != font.fontName		||
	    fontSize != font.fontSize		||
	    fontColor != font.fontColor		||
	    fontWeight != font.fontWeight	||
	    fontItalic != font.fontItalic	||
	    fontUnderline != font.fontUnderline	||
	    fontStrikeOut != font.fontStrikeOut)	return True;
	else						return False;
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

S::Int S::GUI::Font::SetColor(Int newFontColor)
{
	fontColor = newFontColor;

	return Success();
}

S::Int S::GUI::Font::SetWeight(Int newFontWeight)
{
	fontWeight = newFontWeight;

	return Success();
}

S::Int S::GUI::Font::SetItalic(Bool newFontItalic)
{
	fontItalic = newFontItalic;

	return Success();
}

S::Int S::GUI::Font::SetUnderline(Bool newFontUnderline)
{
	fontUnderline = newFontUnderline;

	return Success();
}

S::Int S::GUI::Font::SetStrikeOut(Bool newFontStrikeOut)
{
	fontStrikeOut = newFontStrikeOut;

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

S::Int S::GUI::Font::GetColor() const
{
	return fontColor;
}

S::Int S::GUI::Font::GetWeight() const
{
	return fontWeight;
}

S::Bool S::GUI::Font::GetItalic() const
{
	return fontItalic;
}

S::Bool S::GUI::Font::GetUnderline() const
{
	return fontUnderline;
}

S::Bool S::GUI::Font::GetStrikeOut() const
{
	return fontStrikeOut;
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
	HDC	 ddc	= GetWindowDC(0);
	HDC	 cdc	= CreateCompatibleDC(ddc);
	Int	 size	= -Math::Round(fontSize * 128.0 / GetDeviceCaps(cdc, LOGPIXELSY));
	HFONT	 hFont;
	HFONT	 hOldFont;

	if (Setup::enableUnicode)	hFont = CreateFontW(size, 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);
	else				hFont = CreateFontA(size, 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);

	hOldFont = (HFONT) SelectObject(cdc, hFont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, nOfChars, &tSize);
	else				GetTextExtentPoint32A(cdc, text, nOfChars, &tSize);

	SelectObject(cdc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(cdc);
	ReleaseDC(0, ddc);

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
	HDC	 ddc	= GetWindowDC(0);
	HDC	 cdc	= CreateCompatibleDC(ddc);
	Int	 size	= -Math::Round(fontSize * 128.0 / GetDeviceCaps(cdc, LOGPIXELSY));
	HFONT	 hFont;
	HFONT	 hOldFont;

	if (Setup::enableUnicode)	hFont = CreateFontW(size, 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);
	else				hFont = CreateFontA(size, 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);

	hOldFont = (HFONT) SelectObject(cdc, hFont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, text.Length(), &tSize);
	else				GetTextExtentPoint32A(cdc, text, text.Length(), &tSize);

	SelectObject(cdc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(cdc);
	ReleaseDC(0, ddc);

	return tSize.cy - 1;
#else
	return 0;
#endif
}
