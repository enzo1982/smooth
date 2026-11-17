 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cairo/surfacecairobase.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <pango/pangocairo.h>

#include <string.h>

#if (CAIRO_VERSION_MAJOR == 0							     ) || \
    (CAIRO_VERSION_MAJOR == 1 && CAIRO_VERSION_MINOR <= 8			     ) || \
    (CAIRO_VERSION_MAJOR == 1 && CAIRO_VERSION_MINOR == 9 && CAIRO_VERSION_MICRO <= 1)
#	define CAIRO_OPERATOR_DIFFERENCE CAIRO_OPERATOR_XOR
#endif

S::GUI::SurfaceCairoBase::SurfaceCairoBase()
{
	paintSurface = NIL;
	paintContext = NIL;
}

S::GUI::SurfaceCairoBase::~SurfaceCairoBase()
{
	DestroyBackbuffer();
}

S::Void S::GUI::SurfaceCairoBase::CreateBackbuffer(const Size &nSize)
{
	cairo_surface_t	*nSurface = CreateBackbufferSurface(nSize);
	cairo_t		*nContext = cairo_create(nSurface);

	/* Fill with background color.
	 */
	cairo_set_source_rgb(nContext, Setup::BackgroundColor.GetRed() / 255.0, Setup::BackgroundColor.GetGreen() / 255.0, Setup::BackgroundColor.GetBlue() / 255.0);
	cairo_paint(nContext);

	/* Copy previous contents.
	 */
	if (paintSurface != NIL)
	{
		cairo_set_source_surface(nContext, paintSurface, 0, 0);
		cairo_paint(nContext);
	}

	DestroyBackbuffer();

	paintSurface = nSurface;
	paintContext = nContext;

	cairo_set_antialias(paintContext, CAIRO_ANTIALIAS_NONE);

	paintRects.RemoveAll();
	paintRects.Add(Rect(Point(0, 0), nSize));

	allocSize = nSize;
}

S::Void S::GUI::SurfaceCairoBase::DestroyBackbuffer()
{
	if (paintContext != NIL) cairo_destroy(paintContext);
	if (paintSurface != NIL) cairo_surface_destroy(paintSurface);

	paintContext = NIL;
	paintSurface = NIL;
}

S::Int S::GUI::SurfaceCairoBase::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (paintContext != NIL && !painting) CreateBackbuffer(Size(Math::Max(allocSize.cx, nSize.cx), Math::Max(allocSize.cy, nSize.cy)));

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceCairoBase::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceCairoBase::StartPaint(const Rect &iPRect)
{
	if (paintContext == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());

	cairo_save(paintContext);
	cairo_rectangle(paintContext, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight());
	cairo_clip(paintContext);

	paintRects.Add(pRect);

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::EndPaint()
{
	if (!painting) return Error();

	painting--;

	Rect	 pRect = paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	cairo_restore(paintContext);

	if (painting == 0) PaintRect(pRect);

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::SetPixel(const Point &iPoint, const Color &color)
{
	if (paintContext == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	cairo_set_source_rgb(paintContext, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_rectangle(paintContext, point.x, point.y, 1.0, 1.0);
	cairo_fill(paintContext);

	if (!painting) PaintRect(Rect(point, Size(1, 1)));

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (paintContext == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	cairo_set_source_rgb(paintContext, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);

	/* Draw non-straight lines pixel by pixel to
	 * get the same results as other backends.
	 */
	if (pos1.x != pos2.x && pos1.y != pos2.y)
	{
		Int	 steps	  = Math::Max(Math::Abs(pos2.x - pos1.x), Math::Abs(pos2.y - pos1.y));
		Float	 xAdvance = steps ? Float(pos2.x - pos1.x) / steps : 0;
		Float	 yAdvance = steps ? Float(pos2.y - pos1.y) / steps : 0;

		for (Int i = 0; i < steps; i++) cairo_rectangle(paintContext, pos1.x + i * xAdvance, pos1.y + i * yAdvance, 1.0, 1.0);

		cairo_fill(paintContext);
	}
	else
	{
		cairo_set_line_width(paintContext, 1.0);
		cairo_move_to(paintContext, pos1.x + 0.01, pos1.y + 0.01);
		cairo_line_to(paintContext, pos2.x + 0.01, pos2.y + 0.01);
		cairo_stroke(paintContext);
	}

	if (!painting) PaintRect(Rect(Point(Math::Min(pos1.x, pos2.x) - 1, Math::Min(pos1.y, pos2.y) - 1), Size(Math::Abs(pos2.x - pos1.x) + 3, Math::Abs(pos2.y - pos1.y) + 3)));

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (paintContext == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	cairo_set_source_rgb(paintContext, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			cairo_move_to(paintContext, rect.left + ellipse.cx, rect.top);
			cairo_line_to(paintContext, rect.right - ellipse.cx, rect.top);
			cairo_curve_to(paintContext, rect.right, rect.top, rect.right, rect.top, rect.right, rect.top + ellipse.cy);
			cairo_line_to(paintContext, rect.right, rect.bottom - ellipse.cy);
			cairo_curve_to(paintContext, rect.right, rect.bottom, rect.right, rect.bottom, rect.right - ellipse.cx, rect.bottom);
			cairo_line_to(paintContext, rect.left + ellipse.cx, rect.bottom);
			cairo_curve_to(paintContext, rect.left, rect.bottom, rect.left, rect.bottom, rect.left, rect.bottom - ellipse.cy);
			cairo_line_to(paintContext, rect.left, rect.top + ellipse.cy);
			cairo_curve_to(paintContext, rect.left, rect.top, rect.left, rect.top, rect.left + ellipse.cx, rect.top);

			cairo_fill(paintContext);
		}
		else
		{
			cairo_rectangle(paintContext, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
			cairo_fill(paintContext);
		}
	}
	else if (style == Rect::Outlined)
	{
		cairo_set_line_width(paintContext, 1.0);
		cairo_rectangle(paintContext, rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1);
		cairo_stroke(paintContext);
	}
	else if (style & Rect::Inverted)
	{
		if (cairo_version() >= CAIRO_VERSION_ENCODE(1,9,2))
		{
			cairo_set_operator(paintContext, CAIRO_OPERATOR_DIFFERENCE);
			cairo_set_source_rgb(paintContext, 1.0, 1.0, 1.0);
			cairo_rectangle(paintContext, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
			cairo_fill(paintContext);
			cairo_set_operator(paintContext, CAIRO_OPERATOR_OVER);
		}
		else
		{
			/* Invert colors manually if Cairo does
			 * not support the difference operator.
			 */
			Bitmap	 area(rect.GetSize());

			BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

			area.InvertColors();

			BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);

			return Success();
		}
	}
	else if (style & Rect::Dotted)
	{
		for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) cairo_rectangle(paintContext, x, rect.top,  1.0, 1.0);
		for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) cairo_rectangle(paintContext, rect.right - 1, y,  1.0, 1.0);
		for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) cairo_rectangle(paintContext, x, rect.bottom - 1,  1.0, 1.0);
		for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) cairo_rectangle(paintContext, rect.left, y,  1.0, 1.0);

		cairo_fill(paintContext);
	}

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::SetText(const String &string, const Rect &iRect, const Font &iFont, Bool shadow)
{
	if (paintContext == NIL) return Success();

	if (string == NIL) return Error();
	if (shadow)	   return SurfaceBackend::SetText(string, iRect, iFont, shadow);

	Font	 font	      = iFont;
	Rect	 rect	      = iRect;
	Int	 lineHeight   = 0;

	/* Set up Pango font.
	 */
	Rect	 tRect = rightToLeft.TranslateRect(rect);

	PangoFontDescription	*desc	= pango_font_description_from_string(String(font.GetName())
									    .Append(" ")
									    .Append(font.GetStyle() & Font::Italic ? "Italic " : "")
									    .Append(font.GetWeight() >= Font::Bold ? "Bold " : "")
									    .Append(String::FromInt(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 96.0))));

	cairo_save(paintContext);
	cairo_rectangle(paintContext, tRect.left, tRect.top, tRect.GetWidth(), tRect.GetHeight() + 1);
	cairo_clip(paintContext);

	cairo_set_source_rgb(paintContext, font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

	PangoLayout	*layout = pango_cairo_create_layout(paintContext);

	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);

	/* Draw text line by line.
	 */
	const Array<String>	&lines = string.Explode("\n");

	if (lines.Length() > 1) lineHeight = font.GetScaledTextSizeY() + 3;

	foreach (const String &line, lines)
	{
		Int	 lineLength = line.Length();

		if (lineLength == 0) { rect.top += lineHeight; continue; }

		Rect	 tRect = rightToLeft.TranslateRect(rect);

		tRect.left = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;

		Int	 utf8Length = (line != NIL ? strlen(line.ConvertTo("UTF-8")) : 0);

		PangoAttrList	*attributes    = pango_attr_list_new();
		PangoAttribute	*underline     = pango_attr_underline_new(font.GetStyle() & Font::Underline ? PANGO_UNDERLINE_SINGLE : PANGO_UNDERLINE_NONE);
		PangoAttribute	*strikethrough = pango_attr_strikethrough_new(font.GetStyle() & Font::StrikeOut ? True : False);

		underline->end_index	 = utf8Length;
		strikethrough->end_index = utf8Length;

		pango_attr_list_insert(attributes, underline);
		pango_attr_list_insert(attributes, strikethrough);

		pango_layout_set_attributes(layout, attributes);

		pango_layout_set_text(layout, String(line).Append(" ").ConvertTo("UTF-8"), -1);

		cairo_move_to(paintContext, tRect.left, tRect.top);
		pango_cairo_show_layout(paintContext, layout);

		pango_attr_list_unref(attributes);

		rect.top += lineHeight;
	}

	g_object_unref(layout);

	cairo_restore(paintContext);

	if (!painting) PaintRect(tRect + Size(0, 1));

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (paintContext == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	/* Setup colors.
	 */
	Color	 c1   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color2 : color1;
	Color	 c2   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color1 : color2;

	/* Setup Cairo objects and draw gradient.
	 */
	cairo_pattern_t	*pattern = cairo_pattern_create_linear(rect.left, rect.top, style == OR_HORZ ? rect.right  : rect.left,
									    style == OR_VERT ? rect.bottom : rect.top);

	cairo_pattern_add_color_stop_rgb(pattern, 0, c1.GetRed() / 255.0, c1.GetGreen() / 255.0, c1.GetBlue() / 255.0);
	cairo_pattern_add_color_stop_rgb(pattern, 1, c2.GetRed() / 255.0, c2.GetGreen() / 255.0, c2.GetBlue() / 255.0);

	cairo_set_source(paintContext, pattern);
	cairo_rectangle(paintContext, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
	cairo_fill(paintContext);

	cairo_pattern_destroy(pattern);

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (paintContext == NIL) return Success();
	if (bitmap == NIL)	 return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	Bitmap	 premultipliedBitmap = bitmap;

	PremultiplyAlpha(premultipliedBitmap);

	Size		 srcSize    = premultipliedBitmap.GetSize();
	cairo_format_t	 srcFormat  = premultipliedBitmap.GetDepth() == 32 ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24;
	cairo_surface_t	*srcSurface = cairo_image_surface_create_for_data(premultipliedBitmap.GetBytes(), srcFormat, srcSize.cx, srcSize.cy, srcSize.cx * 4);

	cairo_save(paintContext);

	cairo_rectangle(paintContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	cairo_clip(paintContext);
	cairo_translate(paintContext, destRect.left, destRect.top);
	cairo_scale(paintContext, (float) destRect.GetWidth() / srcRect.GetWidth(), (float) destRect.GetHeight() / srcRect.GetHeight());
	cairo_set_source_surface(paintContext, srcSurface, -srcRect.left, -srcRect.top);
	cairo_paint(paintContext);

	cairo_restore(paintContext);

	cairo_surface_destroy(srcSurface);

	if (!painting) PaintRect(destRect);

	return Success();
}

S::Int S::GUI::SurfaceCairoBase::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (paintContext == NIL) return Success();
	if (bitmap == NIL)	 return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image from the backbuffer.
	 */
	cairo_surface_flush(paintSurface);

	cairo_surface_t	*bitmapSurface = cairo_image_surface_create_for_data(bitmap.GetBytes(), CAIRO_FORMAT_RGB24, bitmap.GetSize().cx, bitmap.GetSize().cy, bitmap.GetSize().cx * 4);
	cairo_t		*bitmapContext = cairo_create(bitmapSurface);

	cairo_rectangle(bitmapContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	cairo_clip(bitmapContext);
	cairo_translate(bitmapContext, destRect.left, destRect.top);
	cairo_scale(bitmapContext, (float) destRect.GetWidth() / srcRect.GetWidth(), (float) destRect.GetHeight() / srcRect.GetHeight());
	cairo_set_source_surface(bitmapContext, paintSurface, -srcRect.left, -srcRect.top);
	cairo_paint(bitmapContext);

	cairo_destroy(bitmapContext);
	cairo_surface_destroy(bitmapSurface);

	return Success();
}
