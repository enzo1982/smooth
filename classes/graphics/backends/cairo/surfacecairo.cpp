 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gdk/gdk.h>

#include <smooth/graphics/backends/cairo/surfacecairo.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/application/application.h>
#include <smooth/system/dynamicloader.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

using namespace X11;

#include <cairo/cairo-xlib.h>
#include <pango/pangocairo.h>

#include <smooth/backends/xlib/backendxlib.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if (CAIRO_VERSION_MAJOR == 0							     ) || \
    (CAIRO_VERSION_MAJOR == 1 && CAIRO_VERSION_MINOR <= 8			     ) || \
    (CAIRO_VERSION_MAJOR == 1 && CAIRO_VERSION_MINOR == 9 && CAIRO_VERSION_MICRO <= 1)
#	define CAIRO_OPERATOR_DIFFERENCE CAIRO_OPERATOR_XOR
#endif

S::GUI::SurfaceBackend *CreateSurfaceCairo(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCairo(iSurface, maxSize);
}

S::Int		 surfaceCairoTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceCairo);

S::Short	 S::GUI::SurfaceCairo::surfaceDPI = -1;

S::GUI::SurfaceCairo::SurfaceCairo(Void *iWindow, const Size &maxSize)
{
	type	     = SURFACE_CAIRO;

	window	     = (X11::Window) iWindow;

	paintBitmap  = NIL;

	display	     = Backends::BackendXLib::GetDisplay();

	if (display != NIL) visual = XDefaultVisual(display, XDefaultScreen(display));
	else		    visual = NIL;

	context	     = NIL;
	surface	     = NIL;

	paintSurfaceCairo = NIL;
	paintContextCairo = NIL;

	if (window != NIL)
	{
		size = maxSize;

		if (maxSize == Size())
		{
			size.cx = XDisplayWidth(display, XDefaultScreen(display)) + 2;
			size.cy = XDisplayHeight(display, XDefaultScreen(display)) + 2;
		}

		fontSize.SetFontSize(GetSurfaceDPI());
		rightToLeft.SetSurfaceSize(size);

		XWindowAttributes	 windowAttributes;

		XGetWindowAttributes(display, window, &windowAttributes);

		paintBitmap	  = XCreatePixmap(display, DefaultRootWindow(display), size.cx, size.cy, windowAttributes.depth);
		paintSurfaceCairo = cairo_xlib_surface_create(display, paintBitmap, visual, size.cx, size.cy);
		paintContextCairo = cairo_create(paintSurfaceCairo);

		paintRects.Add(Rect(Point(0, 0), size));

		cairo_set_antialias(paintContextCairo, CAIRO_ANTIALIAS_NONE);

		allocSize = size;
	}
}

S::GUI::SurfaceCairo::~SurfaceCairo()
{
	if (window != NIL)
	{
		cairo_destroy(paintContextCairo);
		cairo_surface_destroy(paintSurfaceCairo);

		XFreePixmap(display, paintBitmap);
	}
}

S::Int S::GUI::SurfaceCairo::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		cairo_destroy(paintContextCairo);
		cairo_surface_destroy(paintSurfaceCairo);

		XFreePixmap(display, paintBitmap);

		paintRects.RemoveAll();

		XWindowAttributes	 windowAttributes;

		XGetWindowAttributes(display, window, &windowAttributes);

		paintBitmap	  = XCreatePixmap(display, DefaultRootWindow(display), size.cx, size.cy, windowAttributes.depth);
		paintSurfaceCairo = cairo_xlib_surface_create(display, paintBitmap, visual, size.cx, size.cy);
		paintContextCairo = cairo_create(paintSurfaceCairo);

		paintRects.Add(Rect(Point(0, 0), size));

		cairo_set_antialias(paintContextCairo, CAIRO_ANTIALIAS_NONE);
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceCairo::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceCairo::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		GC	 gc = XCreateGC(display, window, 0, NIL);

		XCopyArea(display, paintBitmap, window, gc, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight(), pRect.left, pRect.top);

		XFreeGC(display, gc);
	}

	return Success();
}

S::Int S::GUI::SurfaceCairo::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());

	cairo_save(paintContextCairo);
	cairo_rectangle(paintContextCairo, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight());
	cairo_clip(paintContextCairo);

	paintRects.Add(pRect);

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceCairo::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0) PaintRect(paintRects.GetLast());

	paintRects.RemoveNth(paintRects.Length() - 1);

	cairo_restore(paintContextCairo);

	return Success();
}

S::Void *S::GUI::SurfaceCairo::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceCairo::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	Float	 dpi = 96.0;

	if (display != NIL)
	{
		/* Load gdk_screen_get_monitor_scale_factor dynamically.
		 */
		System::DynamicLoader	 gdk("gdk-3");
		gint			 (*ex_gdk_screen_get_monitor_scale_factor)(GdkScreen *, gint) = (gint (*)(GdkScreen *, gint)) gdk.GetFunctionAddress("gdk_screen_get_monitor_scale_factor");

		/* Get scale factor.
		 */
		Int	 scale = 1.0;

		if (ex_gdk_screen_get_monitor_scale_factor != NIL) scale = ex_gdk_screen_get_monitor_scale_factor(gdk_screen_get_default(), 0);
		else						   scale = (Int64) Number::FromIntString(getenv("GDK_SCALE"));

		if (scale > 0) dpi *= scale;
	}

	surfaceDPI = Math::Round(dpi * Setup::FontSize);

	return surfaceDPI;
}

S::Void S::GUI::SurfaceCairo::CreateCairoContext()
{
	if (context != NIL || surface != NIL) return;

	surface = cairo_xlib_surface_create(display, window, visual, size.cx, size.cy);
	context = cairo_create(surface);

	cairo_set_antialias(context, CAIRO_ANTIALIAS_NONE);
}

S::Void S::GUI::SurfaceCairo::DestroyCairoContext()
{
	if (context == NIL || surface == NIL) return;

	cairo_destroy(context);
	cairo_surface_destroy(surface);

	context = NIL;
	surface = NIL;
}

S::Int S::GUI::SurfaceCairo::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
		cairo_rectangle(context, point.x, point.y, 1.0, 1.0);
		cairo_fill(context);

		DestroyCairoContext();
	}

	cairo_set_source_rgb(paintContextCairo, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_rectangle(paintContextCairo, point.x, point.y, 1.0, 1.0);
	cairo_fill(paintContextCairo);

	return Success();
}

S::Int S::GUI::SurfaceCairo::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if (pos1.x > pos2.x) { pos1.x++; pos2.x++; }
		if (pos1.y > pos2.y) { pos1.y++; pos2.y++; }
	}

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
		cairo_set_line_width(context, 1.0);
		cairo_move_to(context, pos1.x + 0.01, pos1.y + 0.01);
		cairo_line_to(context, pos2.x + 0.01, pos2.y + 0.01);
		cairo_stroke(context);

		DestroyCairoContext();
	}

	cairo_set_source_rgb(paintContextCairo, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_set_line_width(paintContextCairo, 1.0);
	cairo_move_to(paintContextCairo, pos1.x + 0.01, pos1.y + 0.01);
	cairo_line_to(paintContextCairo, pos2.x + 0.01, pos2.y + 0.01);
	cairo_stroke(paintContextCairo);

	return Success();
}

S::Int S::GUI::SurfaceCairo::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	}

	cairo_set_source_rgb(paintContextCairo, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			if (!painting)
			{
				cairo_move_to(context, rect.left + ellipse.cx, rect.top);
				cairo_line_to(context, rect.right - ellipse.cx, rect.top);
				cairo_curve_to(context, rect.right, rect.top, rect.right, rect.top, rect.right, rect.top + ellipse.cy);
				cairo_line_to(context, rect.right, rect.bottom - ellipse.cy);
				cairo_curve_to(context, rect.right, rect.bottom, rect.right, rect.bottom, rect.right - ellipse.cx, rect.bottom);
				cairo_line_to(context, rect.left + ellipse.cx, rect.bottom);
				cairo_curve_to(context, rect.left, rect.bottom, rect.left, rect.bottom, rect.left, rect.bottom - ellipse.cy);
				cairo_line_to(context, rect.left, rect.top + ellipse.cy);
				cairo_curve_to(context, rect.left, rect.top, rect.left, rect.top, rect.left + ellipse.cx, rect.top);

				cairo_fill(context);
			}

			cairo_move_to(paintContextCairo, rect.left + ellipse.cx, rect.top);
			cairo_line_to(paintContextCairo, rect.right - ellipse.cx, rect.top);
			cairo_curve_to(paintContextCairo, rect.right, rect.top, rect.right, rect.top, rect.right, rect.top + ellipse.cy);
			cairo_line_to(paintContextCairo, rect.right, rect.bottom - ellipse.cy);
			cairo_curve_to(paintContextCairo, rect.right, rect.bottom, rect.right, rect.bottom, rect.right - ellipse.cx, rect.bottom);
			cairo_line_to(paintContextCairo, rect.left + ellipse.cx, rect.bottom);
			cairo_curve_to(paintContextCairo, rect.left, rect.bottom, rect.left, rect.bottom, rect.left, rect.bottom - ellipse.cy);
			cairo_line_to(paintContextCairo, rect.left, rect.top + ellipse.cy);
			cairo_curve_to(paintContextCairo, rect.left, rect.top, rect.left, rect.top, rect.left + ellipse.cx, rect.top);

			cairo_fill(paintContextCairo);
		}
		else
		{
			if (!painting)
			{
				cairo_rectangle(context, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
				cairo_fill(context);
			}

			cairo_rectangle(paintContextCairo, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
			cairo_fill(paintContextCairo);
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting)
		{
			cairo_set_line_width(context, 1.0);
			cairo_rectangle(context, rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1);
			cairo_stroke(context);
		}

		cairo_set_line_width(paintContextCairo, 1.0);
		cairo_rectangle(paintContextCairo, rect.left + 0.5, rect.top + 0.5, rect.GetWidth() - 1, rect.GetHeight() - 1);
		cairo_stroke(paintContextCairo);
	}
	else if (style & Rect::Inverted)
	{
		if (cairo_version() >= CAIRO_VERSION_ENCODE(1,9,2))
		{
			if (!painting)
			{
				cairo_set_operator(context, CAIRO_OPERATOR_DIFFERENCE);
				cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
				cairo_rectangle(context, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
				cairo_fill(context);
				cairo_set_operator(context, CAIRO_OPERATOR_OVER);
			}

			cairo_set_operator(paintContextCairo, CAIRO_OPERATOR_DIFFERENCE);
			cairo_set_source_rgb(paintContextCairo, 1.0, 1.0, 1.0);
			cairo_rectangle(paintContextCairo, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
			cairo_fill(paintContextCairo);
			cairo_set_operator(paintContextCairo, CAIRO_OPERATOR_OVER);
		}
		else
		{
			Bitmap	 area(rect.GetSize());

			BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

			area.InvertColors();

			BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
		}
	}
	else if (style & Rect::Dotted)
	{
		if (!painting)
		{
			for (Int x = rect.left						    + 1; x <  rect.right;  x += 2) cairo_rectangle(context, x, rect.top, 1.0, 1.0);
			for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2; y <  rect.bottom; y += 2) cairo_rectangle(context, rect.right - 1, y, 1.0, 1.0);
			for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2; x >= rect.left;   x -= 2) cairo_rectangle(context, x, rect.bottom - 1, 1.0, 1.0);
			for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1; y >= rect.top;	   y -= 2) cairo_rectangle(context, rect.left, y, 1.0, 1.0);

			cairo_fill(context);
		}

		for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) cairo_rectangle(paintContextCairo, x, rect.top,  1.0, 1.0);
		for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) cairo_rectangle(paintContextCairo, rect.right - 1, y,  1.0, 1.0);
		for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) cairo_rectangle(paintContextCairo, x, rect.bottom - 1,  1.0, 1.0);
		for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) cairo_rectangle(paintContextCairo, rect.left, y,  1.0, 1.0);

		cairo_fill(paintContextCairo);
	}

	if (!painting) DestroyCairoContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::SetText(const String &string, const Rect &iRect, const Font &iFont, Bool shadow)
{
	if (window == NIL) return Success();

	if (string == NIL) return Error();
	if (shadow)	   return SurfaceBackend::SetText(string, iRect, iFont, shadow);

	Font	 font	      = iFont;
	Rect	 rect	      = iRect;
	Int	 lineHeight   = 0;

	/* Set up Cairo font.
	 */
	Rect	 tRect = rightToLeft.TranslateRect(rect);

	PangoLayout		*layout = NIL;
	PangoFontDescription	*desc	= pango_font_description_from_string(String(font.GetName())
									    .Append(" ")
									    .Append(font.GetStyle() & Font::Italic ? "Italic " : "")
									    .Append(font.GetWeight() >= Font::Bold ? "Bold " : "")
									    .Append(String::FromInt(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 96.0))));

	if (!painting)
	{
		CreateCairoContext();

		cairo_save(context);
		cairo_rectangle(context, tRect.left, tRect.top, tRect.GetWidth(), tRect.GetHeight() + 1);
		cairo_clip(context);

		cairo_set_source_rgb(context, font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

		layout = pango_cairo_create_layout(context);

		pango_layout_set_font_description(layout, desc);
	}

	cairo_save(paintContextCairo);
	cairo_rectangle(paintContextCairo, tRect.left, tRect.top, tRect.GetWidth(), tRect.GetHeight() + 1);
	cairo_clip(paintContextCairo);

	cairo_set_source_rgb(paintContextCairo, font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

	PangoLayout	*paintLayout = pango_cairo_create_layout(paintContextCairo);

	pango_layout_set_font_description(paintLayout, desc);
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

		if (!painting)
		{
			pango_layout_set_attributes(layout, attributes);

			pango_layout_set_text(layout, String(line).Append(" ").ConvertTo("UTF-8"), -1);

			cairo_move_to(context, tRect.left, tRect.top);
			pango_cairo_show_layout(context, layout);
		}

		pango_layout_set_attributes(paintLayout, attributes);

		pango_layout_set_text(paintLayout, String(line).Append(" ").ConvertTo("UTF-8"), -1);

		cairo_move_to(paintContextCairo, tRect.left, tRect.top);
		pango_cairo_show_layout(paintContextCairo, paintLayout);

		pango_attr_list_unref(attributes);

		rect.top += lineHeight;
	}

	if (!painting)
	{
		g_object_unref(layout);

		cairo_restore(context);

		DestroyCairoContext();
	}

	g_object_unref(paintLayout);

	cairo_restore(paintContextCairo);

	return Success();
}

S::Int S::GUI::SurfaceCairo::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	/* Setup colors.
	 */
	Color	 c1   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color2 : color1;
	Color	 c2   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color1 : color2;

	/* Setup Cairo objects and draw gradient.
	 */
	cairo_pattern_t	*pattern = cairo_pattern_create_linear(0, 0, style == OR_HORZ ? rect.GetWidth()	 : 0,
								     style == OR_VERT ? rect.GetHeight() : 0);

	cairo_pattern_add_color_stop_rgb(pattern, 0, c1.GetRed() / 255.0, c1.GetGreen() / 255.0, c1.GetBlue() / 255.0);
	cairo_pattern_add_color_stop_rgb(pattern, 1, c2.GetRed() / 255.0, c2.GetGreen() / 255.0, c2.GetBlue() / 255.0);

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source(context, pattern);
		cairo_rectangle(context, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
		cairo_fill(context);

		DestroyCairoContext();
	}

	cairo_set_source(paintContextCairo, pattern);
	cairo_rectangle(paintContextCairo, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
	cairo_fill(paintContextCairo);

	cairo_pattern_destroy(pattern);

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

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

	if (!painting)
	{
		CreateCairoContext();

		cairo_rectangle(context, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
		cairo_clip(context);
		cairo_translate(context, destRect.left, destRect.top);
		cairo_scale(context, (float) destRect.GetWidth() / srcRect.GetWidth(), (float) destRect.GetHeight() / srcRect.GetHeight());
		cairo_set_source_surface(context, srcSurface, -srcRect.left, -srcRect.top);
		cairo_paint(context);

		DestroyCairoContext();
	}

	cairo_save(paintContextCairo);

	cairo_rectangle(paintContextCairo, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	cairo_clip(paintContextCairo);
	cairo_translate(paintContextCairo, destRect.left, destRect.top);
	cairo_scale(paintContextCairo, (float) destRect.GetWidth() / srcRect.GetWidth(), (float) destRect.GetHeight() / srcRect.GetHeight());
	cairo_set_source_surface(paintContextCairo, srcSurface, -srcRect.left, -srcRect.top);
	cairo_paint(paintContextCairo);

	cairo_restore(paintContextCairo);

	cairo_surface_destroy(srcSurface);

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	cairo_surface_t	*bitmapSurface = cairo_image_surface_create_for_data(bitmap.GetBytes(), CAIRO_FORMAT_RGB24, bitmap.GetSize().cx, bitmap.GetSize().cy, bitmap.GetSize().cx * 4);
	cairo_t		*bitmapContext = cairo_create(bitmapSurface);

	cairo_rectangle(bitmapContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	cairo_clip(bitmapContext);
	cairo_translate(bitmapContext, destRect.left, destRect.top);
	cairo_scale(bitmapContext, (float) destRect.GetWidth() / srcRect.GetWidth(), (float) destRect.GetHeight() / srcRect.GetHeight());
	cairo_set_source_surface(bitmapContext, surface, -srcRect.left, -srcRect.top);
	cairo_paint(bitmapContext);

	cairo_destroy(bitmapContext);
	cairo_surface_destroy(bitmapSurface);

	return Success();
}
