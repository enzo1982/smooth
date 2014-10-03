 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/graphics/backends/gdi/fontgdi.h>
#include <smooth/graphics/surface.h>

S::GUI::FontBackend *CreateFontGDI(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontGDI(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontGDITmp = S::GUI::FontBackend::SetBackend(&CreateFontGDI);

S::GUI::FontGDI::FontGDI(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_GDI;
}

S::GUI::FontGDI::~FontGDI()
{
}

S::GUI::Size S::GUI::FontGDI::GetTextSize(const String &text, Bool scaled) const
{
	if (text == NIL) return Size();

	/* Fall back to Tahoma when trying to measure Hebrew on pre Windows 8 using Segoe UI.
	 */
	String	 fontName = this->fontName;

	if (fontName == "Segoe UI" && !Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2))
	{
		for (Int i = 0; i < text.Length(); i++) if (text[i] >= 0x0590 && text[i] <= 0x05FF) { fontName = "Tahoma"; break; }
	}

	/* Set up Windows font and calculate text size.
	 */
	Float	 dpi	  = Surface().GetSurfaceDPI();

	HDC	 dc	  = CreateCompatibleDC(NIL);
	HFONT	 hFont	  = CreateFont(-Math::Round(fontSize * dpi / 72.0), 0, 0, 0, fontWeight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, fontName);

	HFONT	 hOldFont = (HFONT) SelectObject(dc, hFont);
	SIZE	 tSize;

	GetTextExtentPoint32(dc, text, text.Length(), &tSize);

	SelectObject(dc, hOldFont);
	::DeleteObject(hFont);

	DeleteDC(dc);

	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(tSize.cx, tSize.cy);
	else					   return Size(tSize.cx, tSize.cy) * 96.0 / dpi;
}
