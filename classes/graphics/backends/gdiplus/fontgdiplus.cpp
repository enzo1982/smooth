 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/graphics/backends/gdiplus/fontgdiplus.h>
#include <smooth/graphics/surface.h>
#include <smooth/init.h>

#include <gdiplus.h>

S::GUI::FontBackend *CreateFontGDIPlus(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontGDIPlus(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontGDIPlusTmp = S::GUI::FontBackend::SetBackend(&CreateFontGDIPlus);

S::Int	 addFontGDIPlusInitTmp = S::AddInitFunction(&S::GUI::FontGDIPlus::Initialize);

S::Int S::GUI::FontGDIPlus::Initialize()
{
	NONCLIENTMETRICS	 ncm;

	ncm.cbSize = sizeof(ncm);

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);

	Font::Default = ncm.lfMessageFont.lfFaceName;

	Setup::FontSize = Float(-ncm.lfMessageFont.lfHeight) / 96.0 * 72.0 / Font::DefaultSize;

	return Success();
}

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

	Int	 textLength = text.Length();

	/* Fall back to Tahoma when trying to measure Hebrew on pre Windows 8 using Segoe UI.
	 */
	String	 fontName = this->fontName;

	if (fontName == "Segoe UI" && !Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2))
	{
		for (Int i = 0; i < textLength; i++) if (text[i] >= 0x0590 && text[i] <= 0x05FF) { fontName = "Tahoma"; break; }
	}

	/* Set up GDI+ font and calculate text size.
	 */
	Float			 dpi = Surface().GetSurfaceDPI();

	Gdiplus::Graphics	 gdip_context((HWND) NIL);
	Gdiplus::Font		 gdip_font(fontName, fontSize * dpi / 96.0, fontWeight >= Font::Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	Gdiplus::StringFormat	 gdip_format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
	Gdiplus::RectF		 gdip_rect;

	gdip_format.SetAlignment(Gdiplus::StringAlignmentNear);

	gdip_context.MeasureString(text, -1, &gdip_font, Gdiplus::PointF(0.0, 0.0), &gdip_format, &gdip_rect);

	return Size(gdip_rect.Width, gdip_rect.Height - 2);
}
