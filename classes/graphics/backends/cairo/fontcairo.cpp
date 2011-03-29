 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
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
#	include <cairo/cairo-win32.h>
#	include <fribidi.h>
#else
	using namespace X11;

#	include <cairo/cairo-xlib.h>

#	ifdef __APPLE__
//#		include <cairo/cairo-ft.h>
//#		include <fontconfig/fontconfig.h>
#	else
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

#ifdef __APPLE__
	/* Make sure Fontconfig is initialized.
	 */
//	FcInit();
#endif
}

S::GUI::FontCairo::~FontCairo()
{
}

S::GUI::Size S::GUI::FontCairo::GetTextSize(const String &iText) const
{
	if (iText == NIL) return Size();

	String	 text	       = iText;
	Bool	 rtlCharacters = False;

	for (Int j = 0; j < text.Length(); j++)
	{
		if (text[j] >= 0x0590 && text[j] <= 0x07BF) { rtlCharacters = True; break; }
	}

#ifdef __WIN32__
	if (rtlCharacters && Setup::useIconv)
	{
		/* Reorder the string with fribidi.
		 */
		FriBidiChar	*visual = new FriBidiChar [text.Length() + 1];
		FriBidiParType	 type = FRIBIDI_PAR_RTL;

		fribidi_log2vis((FriBidiChar *) text.ConvertTo("UCS-4LE"), text.Length(), &type, visual, NIL, NIL, NIL);

		visual[text.Length()] = 0;

		text.ImportFrom("UCS-4LE", (char *) visual);

		delete [] visual;
	}
#endif

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

#if defined __WIN32__ || defined __APPLE__
	cairo_select_font_face(context, fontName,
			       (fontStyle == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
			       (fontWeight == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

	cairo_set_font_size(context, fontSize * 96.0 / 72.0);

	cairo_font_extents_t	 fontExtents;
	cairo_text_extents_t	 textExtents;

	cairo_font_extents(context, &fontExtents);
	cairo_text_extents(context, text.ConvertTo("UTF-8"), &textExtents);
/*#elif defined __APPLE__
	FcCharSet		*charset = FcCharSetCreate();

	for (Int i = 0; i < text.Length(); i++) FcCharSetAddChar(charset, text[i]);

	FcPattern		*pattern = FcPatternBuild(0, FC_FAMILY,	 FcTypeString,	(char *) fontName,
							     FC_WEIGHT,  FcTypeInteger,	(fontWeight == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL),
							     FC_SLANT,	 FcTypeInteger,	(fontStyle == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
							     FC_CHARSET, FcTypeCharSet,	charset, (char *) 0);

	cairo_font_face_t	*fontface = cairo_ft_font_face_create_for_pattern(pattern);

	cairo_set_font_face(context, fontface);
	cairo_set_font_size(context, fontSize * 96.0 / 72.0);

	cairo_font_extents_t	 fontExtents;
	cairo_text_extents_t	 textExtents;

	cairo_font_extents(context, &fontExtents);
	cairo_text_extents(context, text.ConvertTo("UTF-8"), &textExtents);

	cairo_font_face_destroy(fontface);

	FcPatternDestroy(pattern);
	FcCharSetDestroy(charset);*/
#else
	PangoLayout		*layout	= pango_cairo_create_layout(context);
	PangoFontDescription	*desc	= pango_font_description_from_string(String(fontName)
									    .Append(" ")
									    .Append(fontStyle == Font::Italic ? "Italic " : "")
									    .Append(fontWeight == Font::Bold ? "Bold " : "")
									    .Append(String::FromInt(fontSize)));

	if (text.Length() > 0) pango_layout_set_text(layout, text.ConvertTo("UTF-8"), -1);

	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	int	 x = 0;
	int	 y = 0;

	pango_layout_get_pixel_size(layout, &x, &y);

	g_object_unref(layout);
#endif

	cairo_destroy(context);
	cairo_surface_destroy(surface);

#ifdef __WIN32__
	DeleteDC(dc);
#endif

#if defined __WIN32__ || defined __APPLE__
	return Size(textExtents.x_advance, fontExtents.height);
#else
	return Size(x, y - 2);
#endif
}
