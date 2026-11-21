 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/direct2d/fontdirectwrite.h>
#include <smooth/backends/direct2d/backenddirect2d.h>
#include <smooth/backends/platform.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

S::GUI::FontBackend *CreateFontDirectWrite(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontDirectWrite(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

static S::Int Register_fontDirectWriteTmp() { return S::GUI::FontBackend::SetBackend(&CreateFontDirectWrite); }

S::Int	 fontDirectWriteTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_fontDirectWriteTmp);

S::GUI::FontDirectWrite::FontDirectWrite(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_DIRECTWRITE;
}

S::GUI::FontDirectWrite::~FontDirectWrite()
{
}

S::GUI::Size S::GUI::FontDirectWrite::GetTextSize(const String &text) const
{
	if (text == NIL) return Size();

	Int	 textLength = text.Length();

	/* Set up DirectWrite text layout and calculate text size.
	 */
	Float			 dpi	= Surface().GetSurfaceDPI();
	Int			 height = Math::Round(fontSize * dpi / 72.0);

	IDWriteTextFormat	*format = Backends::BackendDirect2D::CreateTextFormat(fontName, height, fontWeight >= Font::Bold, fontStyle & Font::Italic);
	IDWriteTextLayout	*layout = NIL;

	if (format == NIL) return Size();

	Backends::BackendDirect2D::GetDWriteFactory()->CreateGdiCompatibleTextLayout(text, textLength, format, 32767, 32767, 1.0, NIL, True, &layout);

	DWRITE_TEXT_METRICS	 metrics = { 0 };

	if (layout != NIL) layout->GetMetrics(&metrics);

	if (layout != NIL) layout->Release();

	format->Release();

	return Size(Math::Ceil(metrics.widthIncludingTrailingWhitespace), Math::Ceil(metrics.height));
}
