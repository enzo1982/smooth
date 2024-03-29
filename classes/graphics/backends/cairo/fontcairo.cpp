 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <type_traits>

#include <smooth/graphics/backends/cairo/fontcairo.h>
#include <smooth/graphics/surface.h>
#include <smooth/files/file.h>
#include <smooth/foreach.h>
#include <smooth/init.h>

using namespace X11;

#include <cairo/cairo-xlib.h>
#include <pango/pangocairo.h>

#include <smooth/backends/xlib/backendxlib.h>

S::GUI::FontBackend *CreateFontCairo(const S::String &iFontName, S::Short iFontSize, S::Short iFontWeight, S::Short iFontStyle, const S::GUI::Color &iFontColor)
{
	return new S::GUI::FontCairo(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor);
}

S::Int	 fontCairoTmp = S::GUI::FontBackend::SetBackend(&CreateFontCairo);

S::Int	 addFontCairoInitTmp = S::AddInitFunction(&S::GUI::FontCairo::Initialize);

S::Int S::GUI::FontCairo::Initialize()
{
	Font::Default	= "Helvetica";
	Setup::FontSize = 1.0;

	String	 font	     = Backends::BackendXLib::QueryGSettings("org.gnome.desktop.interface", "font-name");
	Float	 scaleFactor = Backends::BackendXLib::QueryGSettings("org.gnome.desktop.interface", "text-scaling-factor").ToFloat();

	if (font != NIL)
	{
		Font::Default	= font.SubString(1, font.FindLast(" ") - 1);
		Setup::FontSize = font.SubString(font.FindLast(" ") + 1, font.Length() - font.FindLast(" ") - 2).ToFloat() / Font::DefaultSize;
	}

	if (scaleFactor != 0) Setup::FontSize *= scaleFactor;

	return Success();
}

S::GUI::FontCairo::FontCairo(const String &iFontName, Short iFontSize, Short iFontWeight, Short iFontStyle, const Color &iFontColor) : FontBackend(iFontName, iFontSize, iFontWeight, iFontStyle, iFontColor)
{
	type = FONT_CAIRO;
}

S::GUI::FontCairo::~FontCairo()
{
}

S::GUI::Size S::GUI::FontCairo::GetTextSize(const String &iText) const
{
	if (iText == NIL) return Size();

	String	 text	    = iText;
	Int	 textLength = text.Length();

	/* Set up Cairo font and calculate text size.
	 */
	Float		 dpi	 = Surface().GetSurfaceDPI();

	cairo_surface_t	*surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
	cairo_t		*context = cairo_create(surface);

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

	cairo_destroy(context);
	cairo_surface_destroy(surface);

	return Size(x, y - 2);
}
