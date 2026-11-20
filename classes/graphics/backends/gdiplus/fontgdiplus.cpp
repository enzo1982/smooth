 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/backends/platform.h>
#include <smooth/graphics/backends/gdiplus/fontgdiplus.h>
#include <smooth/graphics/surface.h>
#include <smooth/init.h>

#include <gdiplus.h>

S::GUI::FontBackend *CreateFontGDIPlus(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontGDIPlus(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

static S::Int Register_fontGDIPlusTmp() { return S::GUI::FontBackend::SetBackend(&CreateFontGDIPlus); }

S::Int	 fontGDIPlusTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_GDIPLUS, &Register_fontGDIPlusTmp);

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

	/* Set up GDI+ font with the same pixel height GDI would use and calculate text width.
	 */
	Float			 dpi	= Surface().GetSurfaceDPI();
	Int			 height = Math::Round(fontSize * dpi / 72.0);

	Gdiplus::Graphics	 gdip_context((HWND) NIL);
	Gdiplus::Font		 gdip_font(fontName, height, fontWeight >= Font::Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::StringFormat	 gdip_format(Gdiplus::StringFormat::GenericTypographic());
	Gdiplus::RectF		 gdip_rect;

	gdip_format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsMeasureTrailingSpaces);
	gdip_format.SetAlignment(Gdiplus::StringAlignmentNear);

	gdip_context.MeasureString(text, -1, &gdip_font, Gdiplus::PointF(0.0, 0.0), &gdip_format, &gdip_rect);

	/* Use GDI line metrics for the text height, so text
	 * is laid out the same way as with the GDI backend.
	 */
	HDC	 dc	  = CreateCompatibleDC(NIL);
	HFONT	 hFont	  = CreateFont(-height, 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);
	HFONT	 hOldFont = (HFONT) SelectObject(dc, hFont);
	RECT	 tRect	  = { 0, 0, 0, 0 };

	DrawTextEx(dc, text, -1, &tRect, DT_EXPANDTABS | DT_NOPREFIX | DT_LEFT | DT_CALCRECT, NIL);

	SelectObject(dc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(dc);

	return Size(Math::Ceil(gdip_rect.Width), tRect.bottom - tRect.top);
}
