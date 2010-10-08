 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cairo/fontcairo.h>
#include <smooth/graphics/font.h>

#ifdef __WIN32__
#	include <cairowin/cairo-win32.h>
#else
#	include <cairo/cairo-xlib.h>
#	include <smooth/backends/xlib/backendxlib.h>
#endif

S::GUI::FontBackend *CreateFontCairo(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontCairo(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontCairoTmp = S::GUI::FontBackend::SetBackend(&CreateFontCairo);

S::GUI::FontCairo::FontCairo(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_CAIRO;
}

S::GUI::FontCairo::~FontCairo()
{
}

S::GUI::Size S::GUI::FontCairo::GetTextSize(const String &text) const
{
	if (text == NIL) return Size();

#ifdef __WIN32__
	HDC		 dc	 = CreateCompatibleDC(NIL);

	cairo_surface_t	*surface = cairo_win32_surface_create(dc);
#else
	Display		*display = Backends::BackendXLib::GetDisplay();
	Visual		*visual	 = XDefaultVisual(display, XDefaultScreen(display));
	Window		 window	 = XRootWindow(display, XDefaultScreen(display));

	cairo_surface_t	*surface = cairo_xlib_surface_create(display, window, visual, 0, 0);
#endif

	cairo_t		*context = cairo_create(surface);

	cairo_select_font_face(context, fontName,
			       (fontStyle == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
			       (fontWeight == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

	cairo_set_font_size(context, fontSize * 96.0 / 72.0);

	cairo_text_extents_t	 extents;

	cairo_text_extents(context, text.ConvertTo("UTF-8"), &extents);

	cairo_destroy(context);
	cairo_surface_destroy(surface);

#ifdef __WIN32__
	DeleteDC(dc);
#endif

	return Size(Int(extents.width) + 1, Int(extents.height) + 2);
}
