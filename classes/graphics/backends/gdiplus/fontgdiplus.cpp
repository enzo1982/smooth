 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <windows.h>
#include <gdiplus.h>

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/graphics/backends/gdiplus/fontgdiplus.h>
#include <smooth/graphics/surface.h>

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

S::GUI::Size S::GUI::FontGDIPlus::GetTextSize(const String &text, Bool scaled) const
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

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(gdip_rect.Width, gdip_rect.Height - 2);
	else					   return Size(gdip_rect.Width, gdip_rect.Height - 2) * 96.0 / dpi;
}
