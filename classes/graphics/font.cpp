 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/font.h>
#include <smooth/graphics/backends/fontbackend.h>
#include <smooth/misc/math.h>

S::String	 S::GUI::Font::Default		= "default";
S::Short	 S::GUI::Font::DefaultSize	= 8;

S::Short	 S::GUI::Font::Thin		= 100;
S::Short	 S::GUI::Font::ExtraLight	= 200;
S::Short	 S::GUI::Font::Light		= 300;
S::Short	 S::GUI::Font::Normal		= 400;
S::Short	 S::GUI::Font::Medium		= 500;
S::Short	 S::GUI::Font::SemiBold		= 600;
S::Short	 S::GUI::Font::Bold		= 700;
S::Short	 S::GUI::Font::ExtraBold	= 800;
S::Short	 S::GUI::Font::Black		= 900;

S::Short	 S::GUI::Font::Italic		= 2;
S::Short	 S::GUI::Font::Underline	= 4;
S::Short	 S::GUI::Font::StrikeOut	= 8;

S::GUI::Font::Font(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor)
{
	fontName   = iFontName;
	fontSize   = iFontSize;
	fontColor  = iFontColor;
	fontWeight = iFontWeight;
	fontStyle  = iFontStyle;

	backend = FontBackend::CreateBackendInstance(fontName, fontSize, fontWeight, fontStyle, fontColor);
}

S::GUI::Font::Font(const Font &iFont)
{
	fontName   = iFont.fontName;
	fontSize   = iFont.fontSize;
	fontColor  = iFont.fontColor;
	fontWeight = iFont.fontWeight;
	fontStyle  = iFont.fontStyle;

	backend = FontBackend::CreateBackendInstance(fontName, fontSize, fontWeight, fontStyle, fontColor);
}

S::GUI::Font::~Font()
{
	delete backend;
}

S::GUI::Font &S::GUI::Font::operator =(const Font &newFont)
{
	SetName(newFont.GetName());
	SetSize(newFont.GetSize());
	SetColor(newFont.GetColor());
	SetWeight(newFont.GetWeight());
	SetStyle(newFont.GetStyle());

	return *this;
}

S::Bool S::GUI::Font::operator ==(const Font &font) const
{
	if (fontName   == font.GetName()   &&
	    fontSize   == font.GetSize()   &&
	    fontColor  == font.GetColor()  &&
	    fontWeight == font.GetWeight() &&
	    fontStyle  == font.GetStyle()) return True;
	else				   return False;
}

S::Bool S::GUI::Font::operator !=(const Font &font) const
{
	return !(*this == font);
}

S::Int S::GUI::Font::SetName(const String &newFontName)
{
	backend->SetName(newFontName);

	fontName = newFontName;

	return Success();
}

S::Int S::GUI::Font::SetSize(Short newFontSize)
{
	backend->SetSize(newFontSize);

	fontSize = newFontSize;

	return Success();
}

S::Int S::GUI::Font::SetColor(const Color &newFontColor)
{
	backend->SetColor(newFontColor);

	fontColor = newFontColor;

	return Success();
}

S::Int S::GUI::Font::SetWeight(Short newFontWeight)
{
	backend->SetWeight(newFontWeight);

	fontWeight = newFontWeight;

	return Success();
}

S::Int S::GUI::Font::SetStyle(Short newFontStyle)
{
	backend->SetStyle(newFontStyle);

	fontStyle = newFontStyle;

	return Success();
}

const S::String &S::GUI::Font::GetName() const
{
	return fontName;
}

S::Short S::GUI::Font::GetSize() const
{
	return fontSize;
}

const S::GUI::Color &S::GUI::Font::GetColor() const
{
	return fontColor;
}

S::Short S::GUI::Font::GetWeight() const
{
	return fontWeight;
}

S::Short S::GUI::Font::GetStyle() const
{
	return fontStyle;
}

S::Int S::GUI::Font::GetUnscaledTextSizeX(const String &text) const
{
	return backend->GetTextSizeX(text, False);
}

S::Int S::GUI::Font::GetUnscaledTextSizeY(const String &text) const
{
	return backend->GetTextSizeY(text, False);
}

S::Int S::GUI::Font::GetScaledTextSizeX(const String &text) const
{
	return backend->GetTextSizeX(text, True);
}

S::Int S::GUI::Font::GetScaledTextSizeY(const String &text) const
{
	return backend->GetTextSizeY(text, True);
}
