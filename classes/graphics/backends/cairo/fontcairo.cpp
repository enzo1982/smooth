 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cairo/fontcairo.h>
#include <smooth/graphics/surface.h>

#ifdef __WIN32__
#	include <smooth/backends/win32/backendwin32.h>

#	include <cairo/cairo-win32.h>
#	include <fribidi/fribidi.h>
#else
	using namespace X11;

#	include <cairo/cairo-xlib.h>

#	ifndef __APPLE__
#		include <pango/pangocairo.h>
#	endif

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

S::GUI::Size S::GUI::FontCairo::GetTextSize(const String &iText, Bool scaled) const
{
	if (iText == NIL) return Size();

	String	 text	    = iText;
	Int	 textLength = text.Length();

#ifdef __WIN32__
	/* Fall back to Tahoma when trying to measure Hebrew on pre Windows 8 using Segoe UI.
	 */
	String	 fontName = this->fontName;

	if (fontName == "Segoe UI" && !Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2))
	{
		for (Int i = 0; i < textLength; i++) if (text[i] >= 0x0590 && text[i] <= 0x05FF) { fontName = "Tahoma"; break; }
	}

	/* Reorder the string.
	 */
	Bool	 rtlCharacters = False;

	for (Int i = 0; i < textLength; i++) if (text[i] >= 0x0590 && text[i] <= 0x08FF) { rtlCharacters = True; break; }

	if (rtlCharacters && Setup::useIconv)
	{
		/* Reorder the string with fribidi.
		 */
		FriBidiChar	*visual = new FriBidiChar [textLength + 1];
		FriBidiParType	 type = FRIBIDI_PAR_RTL;

		fribidi_log2vis((FriBidiChar *) text.ConvertTo("UCS-4LE"), textLength, &type, visual, NIL, NIL, NIL);

		visual[textLength] = 0;

		text.ImportFrom("UCS-4LE", (char *) visual);

		delete [] visual;
	}
#endif

	/* Set up Cairo font and calculate text size.
	 */
	Float		 dpi	 = Surface().GetSurfaceDPI();

	cairo_surface_t	*surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
	cairo_t		*context = cairo_create(surface);

#if defined __WIN32__ || defined __APPLE__
	cairo_select_font_face(context, fontName, (fontStyle & Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
						  (fontWeight >= Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

	cairo_set_font_size(context, fontSize * dpi / 72.0);

	cairo_font_extents_t	 fontExtents;
	cairo_text_extents_t	 textExtents;

	cairo_font_extents(context, &fontExtents);
	cairo_text_extents(context, text.ConvertTo("UTF-8"), &textExtents);
#else
	PangoLayout		*layout	= pango_cairo_create_layout(context);
	PangoFontDescription	*desc	= pango_font_description_from_string(String(fontName)
									    .Append(" ")
									    .Append(fontStyle & Font::Italic ? "Italic " : "")
									    .Append(fontWeight >= Font::Bold ? "Bold " : "")
									    .Append(String::FromInt(Math::Round(fontSize * dpi / 96.0))));

	if (textLength > 0) pango_layout_set_text(layout, text.ConvertTo("UTF-8"), -1);

	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	int	 x = 0;
	int	 y = 0;

	pango_layout_get_pixel_size(layout, &x, &y);

	g_object_unref(layout);
#endif

	cairo_destroy(context);
	cairo_surface_destroy(surface);

#if defined __WIN32__ || defined __APPLE__
	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(textExtents.x_advance, fontExtents.height);
	else					   return Size(textExtents.x_advance, fontExtents.height) * 96.0 / dpi;
#else
	if (scaled || Math::Abs(dpi - 96.0) < 0.1) return Size(x, y - 2);
	else					   return Size(x, y - 2) * 96.0 / dpi;
#endif
}
