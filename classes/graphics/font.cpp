 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/font.h>

S::GUI::Font::Font(String iFontName, Int iFontSize, Int iFontColor, Int iFontWeight, Bool iFontItalic, Bool iFontUnderline, Bool iFontStrikeOut)
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

S::Bool S::GUI::Font::operator ==(const Font &font)
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

S::Bool S::GUI::Font::operator !=(const Font &font)
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

S::Int S::GUI::Font::SetName(String newFontName)
{
	fontName = newFontName;

	return Success;
}

S::Int S::GUI::Font::SetSize(Int newFontSize)
{
	fontSize = newFontSize;

	return Success;
}

S::Int S::GUI::Font::SetColor(Int newFontColor)
{
	fontColor = newFontColor;

	return Success;
}

S::Int S::GUI::Font::SetWeight(Int newFontWeight)
{
	fontWeight = newFontWeight;

	return Success;
}

S::Int S::GUI::Font::SetItalic(Bool newFontItalic)
{
	fontItalic = newFontItalic;

	return Success;
}

S::Int S::GUI::Font::SetUnderline(Bool newFontUnderline)
{
	fontUnderline = newFontUnderline;

	return Success;
}

S::Int S::GUI::Font::SetStrikeOut(Bool newFontStrikeOut)
{
	fontStrikeOut = newFontStrikeOut;

	return Success;
}

S::String S::GUI::Font::GetName()
{
	return fontName;
}

S::Int S::GUI::Font::GetSize()
{
	return fontSize;
}

S::Int S::GUI::Font::GetColor()
{
	return fontColor;
}

S::Int S::GUI::Font::GetWeight()
{
	return fontWeight;
}

S::Bool S::GUI::Font::GetItalic()
{
	return fontItalic;
}

S::Bool S::GUI::Font::GetUnderline()
{
	return fontUnderline;
}

S::Bool S::GUI::Font::GetStrikeOut()
{
	return fontStrikeOut;
}
