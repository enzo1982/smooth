 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cairo/surfacecairo.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

#ifdef __WIN32__
#	include <cairo/cairo-win32.h>
#else
#	include <cairo/cairo-xlib.h>
#	include <smooth/backends/xlib/backendxlib.h>
#endif

S::GUI::SurfaceBackend *CreateSurfaceCairo(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCairo(iSurface, maxSize);
}

S::Int	 surfaceCairoTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceCairo);

S::Int	  S::GUI::SurfaceCairo::surfaceDPI	= -1;

S::GUI::SurfaceCairo::SurfaceCairo(Void *iWindow, const Size &maxSize)
{
	type	= SURFACE_CAIRO;

#ifdef __WIN32__
	window	= (HWND) iWindow;

	gdi_dc	= NIL;
#else
	window	= (Window) iWindow;
	display	= Backends::BackendXLib::GetDisplay();
	visual	= XDefaultVisual(display, XDefaultScreen(display));
#endif

	context = NIL;
	surface = NIL;

	if (window != NIL)
	{
		size = maxSize;

#ifdef __WIN32__
		HDC	 gdi_dc = GetWindowDC(window);

		if (maxSize == Size())
		{
			size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
			size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;
		}
#else
		if (maxSize == Size())
		{
			size.cx = XDisplayWidth(display, XDefaultScreen(display)) + 2;
			size.cy = XDisplayHeight(display, XDefaultScreen(display)) + 2;
		}
#endif

		rightToLeft.SetSurfaceSize(size);

#ifdef __WIN32__
		HDC	 bmp_dc = CreateCompatibleDC(gdi_dc);
		HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		cDc_contexts.Add(bmp_dc);
		cDc_bitmaps.Add((HBITMAP) SelectObject(bmp_dc, bitmap));
		cDc_rects.Add(new Rect(Point(0, 0), size));

		ReleaseDC(window, gdi_dc);

		cairo_surface_t	*surface = cairo_win32_surface_create(bmp_dc);
#else
		cairo_surface_t	*surface = cairo_xlib_surface_create(display, window, visual, size.cx, size.cy);
#endif

		cairo_t		*context = cairo_create(surface);
	
		cairo_set_antialias(context, CAIRO_ANTIALIAS_NONE);

		cairo_surfaces.Add(surface);
		cairo_contexts.Add(context);

		allocSize = size;
	}
}

S::GUI::SurfaceCairo::~SurfaceCairo()
{
	if (window != NIL)
	{
		cairo_destroy(cairo_contexts.GetFirst());
		cairo_surface_destroy(cairo_surfaces.GetFirst());

#ifdef __WIN32__
		HBITMAP	 bitmap = (HBITMAP) SelectObject(cDc_contexts.GetFirst(), cDc_bitmaps.GetFirst());

		DeleteDC(cDc_contexts.GetFirst());
		::DeleteObject(bitmap);

		delete cDc_rects.GetFirst();
#endif
	}
}

S::Int S::GUI::SurfaceCairo::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
#ifdef __WIN32__
		HBITMAP	 bitmap = (HBITMAP) SelectObject(cDc_contexts.GetFirst(), cDc_bitmaps.GetFirst());

		::DeleteObject(bitmap);

		delete cDc_rects.GetFirst();

		cDc_bitmaps.RemoveAll();
		cDc_rects.RemoveAll();

		HDC	 gdi_dc = GetWindowDC(window);

		bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		cDc_bitmaps.Add((HBITMAP) SelectObject(cDc_contexts.GetFirst(), bitmap));
		cDc_rects.Add(new Rect(Point(0, 0), size));

		ReleaseDC(window, gdi_dc);
#endif
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
#ifdef __WIN32__
		HDC	 gdi_dc = GetWindowDC(window);

		BitBlt(gdi_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetFirst(), pRect.left, pRect.top, SRCCOPY);

		ReleaseDC(window, gdi_dc);
#endif
	}

	return Success();
}

S::Int S::GUI::SurfaceCairo::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iPRect));

#ifdef __WIN32__
	Rect	 oRect = *(cDc_rects.GetLast());
	HDC	 bmp_dc = CreateCompatibleDC(cDc_contexts.GetFirst());
	HBITMAP	 bitmap = CreateCompatibleBitmap(cDc_contexts.GetFirst(), size.cx, size.cy);

	cDc_bitmaps.Add((HBITMAP) SelectObject(bmp_dc, bitmap));

	if (pRect.left >= oRect.left && pRect.top >= oRect.top && pRect.right <= oRect.right && pRect.bottom <= oRect.bottom)	BitBlt(bmp_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetLast(), pRect.left, pRect.top, SRCCOPY);
	else															BitBlt(bmp_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetFirst(), pRect.left, pRect.top, SRCCOPY);

	cDc_contexts.Add(bmp_dc);
	cDc_rects.Add(new Rect(pRect));

	cairo_surface_t	*surface = cairo_win32_surface_create(bmp_dc);
	cairo_t		*context = cairo_create(surface);

	cairo_set_antialias(context, CAIRO_ANTIALIAS_NONE);

	cairo_surfaces.Add(surface);
	cairo_contexts.Add(context);
#endif

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceCairo::EndPaint()
{
	if (!painting) return Error();

	painting--;

#ifdef __WIN32__
	cairo_destroy(cairo_contexts.GetLast());
	cairo_surface_destroy(cairo_surfaces.GetLast());

	cairo_contexts.Remove(cairo_contexts.GetNthIndex(cairo_contexts.Length() - 1));
	cairo_surfaces.Remove(cairo_surfaces.GetNthIndex(cairo_surfaces.Length() - 1));

	Rect	 iRect = Rect::OverlapRect(*(cDc_rects.GetLast()), *(cDc_rects.GetNth(cDc_rects.Length() - 2)));

	BitBlt(cDc_contexts.GetNth(cDc_contexts.Length() - 2), iRect.left, iRect.top, iRect.right - iRect.left, iRect.bottom - iRect.top, cDc_contexts.GetLast(), iRect.left, iRect.top, SRCCOPY);

	if (painting == 0) PaintRect(iRect);

	HBITMAP	 bitmap = (HBITMAP) SelectObject(cDc_contexts.GetLast(), cDc_bitmaps.GetLast());

	DeleteDC(cDc_contexts.GetLast());
	::DeleteObject(bitmap);

	delete cDc_rects.GetLast();

	cDc_contexts.Remove(cDc_contexts.GetNthIndex(cDc_contexts.Length() - 1));
	cDc_bitmaps.Remove(cDc_bitmaps.GetNthIndex(cDc_bitmaps.Length() - 1));
	cDc_rects.Remove(cDc_rects.GetNthIndex(cDc_rects.Length() - 1));
#endif

	return Success();
}

S::Void *S::GUI::SurfaceCairo::GetSystemSurface() const
{
	return (Void *) window;
}

S::Int S::GUI::SurfaceCairo::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	Int	 dpi = 96;

	surfaceDPI = dpi;

	return dpi;
}

S::Void S::GUI::SurfaceCairo::CreateCairoContext()
{
	if (context != NIL || surface != NIL) return;

#ifdef __WIN32__
	gdi_dc = GetWindowDC(window);

	surface = cairo_win32_surface_create(gdi_dc);
#else
	surface = cairo_xlib_surface_create(display, window, visual, size.cx, size.cy);
#endif

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

#ifdef __WIN32__
	ReleaseDC(window, gdi_dc);

	gdi_dc = NIL;
#endif
}

S::Int S::GUI::SurfaceCairo::SetPixel(const Point &point, const Color &color)
{
	if (window == NIL) return Success();

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
		cairo_rectangle(context, rightToLeft.TranslateX(fontSize.TranslateX(point.x)), rightToLeft.TranslateY(fontSize.TranslateY(point.y)), 1, 1);
		cairo_fill(context);

		DestroyCairoContext();
	}

	cairo_set_source_rgb(cairo_contexts.GetLast(), color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_rectangle(cairo_contexts.GetLast(), rightToLeft.TranslateX(fontSize.TranslateX(point.x)), rightToLeft.TranslateY(fontSize.TranslateY(point.y)), 1, 1);
	cairo_fill(cairo_contexts.GetLast());

	return Success();
}

S::Int S::GUI::SurfaceCairo::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos1));
	Point	 pos2 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos2));

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
		cairo_set_line_width(context, 1);
		cairo_move_to(context, pos1.x + 0.5, pos1.y);
		cairo_line_to(context, pos2.x + 0.5, pos2.y);
		cairo_stroke(context);

		DestroyCairoContext();
	}

	cairo_set_source_rgb(cairo_contexts.GetLast(), color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_set_line_width(cairo_contexts.GetLast(), 1);
	cairo_move_to(cairo_contexts.GetLast(), pos1.x + 0.5, pos1.y);
	cairo_line_to(cairo_contexts.GetLast(), pos2.x + 0.5, pos2.y);
	cairo_stroke(cairo_contexts.GetLast());

	return Success();
}

S::Int S::GUI::SurfaceCairo::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	CreateCairoContext();

	cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_set_source_rgb(cairo_contexts.GetLast(), color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
		}
		else
		{
			if (!painting)
			{
				cairo_rectangle(context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
				cairo_fill(context);
			}

			cairo_rectangle(cairo_contexts.GetLast(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			cairo_fill(cairo_contexts.GetLast());
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting)
		{
			cairo_set_line_width(context, 1);
			cairo_rectangle(context, rect.left + 0.5, rect.top + 0.5, rect.right - rect.left - 1, rect.bottom - rect.top - 1);
			cairo_stroke(context);
		}

		cairo_set_line_width(cairo_contexts.GetLast(), 1);
		cairo_rectangle(cairo_contexts.GetLast(), rect.left + 0.5, rect.top + 0.5, rect.right - rect.left - 1, rect.bottom - rect.top - 1);
		cairo_stroke(cairo_contexts.GetLast());
	}
	else if (style & Rect::Inverted)
	{
	}
	else if (style & Rect::Dotted)
	{
		Bool	 dot = False;
		Int	 x;
		Int	 y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == True)
			{
				if (!painting)
				{
					cairo_rectangle(context, x, y, 1, 1);
					cairo_fill(context);
				}

				cairo_rectangle(cairo_contexts.GetLast(), x, y, 1, 1);
				cairo_fill(cairo_contexts.GetLast());

				dot = False;
			}
			else dot = True;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == True)
			{
				if (!painting)
				{
					cairo_rectangle(context, x, y, 1, 1);
					cairo_fill(context);
				}

				cairo_rectangle(cairo_contexts.GetLast(), x, y, 1, 1);
				cairo_fill(cairo_contexts.GetLast());

				dot = False;
			}
			else dot = True;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == True)
			{
				if (!painting)
				{
					cairo_rectangle(context, x, y, 1, 1);
					cairo_fill(context);
				}

				cairo_rectangle(cairo_contexts.GetLast(), x, y, 1, 1);
				cairo_fill(cairo_contexts.GetLast());

				dot = False;
			}
			else dot = True;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == True)
			{
				if (!painting)
				{
					cairo_rectangle(context, x, y, 1, 1);
					cairo_fill(context);
				}

				cairo_rectangle(cairo_contexts.GetLast(), x, y, 1, 1);
				cairo_fill(cairo_contexts.GetLast());

				dot = False;
			}
			else dot = True;
		}
	}

	DestroyCairoContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 txtsize = string.Length();
	String	 line;
	Rect	 rect = iRect;
	Int	 lineHeight = font.GetTextSizeY("abcdefghijklmnopqrstuvwxyz") + 3;

	if (rightToLeft.GetRightToLeft()) rect.right--;

	for (Int j = 0; j < txtsize; j++) if (string[j] == 10) lines++;

	for (Int i = 0; i < lines; i++)
	{
		Bool	 rtlCharacters = False;

		origoffset = offset;

		for (Int j = 0; j <= txtsize; j++)
		{
			if (j + origoffset == txtsize)
			{
				line[j] = 0;
				break;
			}

			if (string[j + origoffset] == 10 || string[j + origoffset] == 0)
			{
				offset++;
				line[j] = 0;
				break;
			}
			else
			{
				offset++;
				line[j] = string[j + origoffset];

				if (line[j] >= 0x0590 && line[j] <= 0x07BF) rtlCharacters = True;
			}
		}

		rect = rightToLeft.TranslateRect(fontSize.TranslateRect(rect));

		if (!painting)
		{
			CreateCairoContext();

			cairo_select_font_face(context, font.GetName(),
					       (font.GetStyle() == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
					       (font.GetWeight() == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

			cairo_set_font_size(context, font.GetSize() * fontSize.TranslateY(96) / 72.0);

			cairo_set_source_rgb(context, font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

			cairo_move_to(context, rect.left, rect.top + font.GetSize() * fontSize.TranslateY(96) / 72.0);
			cairo_show_text(context, line.ConvertTo("UTF-8"));

			DestroyCairoContext();
		}

		cairo_select_font_face(cairo_contexts.GetLast(), font.GetName(),
				       (font.GetStyle() == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
				       (font.GetWeight() == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

		cairo_set_font_size(cairo_contexts.GetLast(), font.GetSize() * fontSize.TranslateY(96) / 72.0);

		cairo_set_source_rgb(cairo_contexts.GetLast(), font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

		cairo_move_to(cairo_contexts.GetLast(), rect.left, rect.top + font.GetSize() * fontSize.TranslateY(96) / 72.0);
		cairo_show_text(cairo_contexts.GetLast(), line.ConvertTo("UTF-8"));

		rect.top += lineHeight;
	}

	return Success();
}

S::Int S::GUI::SurfaceCairo::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	cairo_pattern_t	*pattern = NIL;

	switch (style)
	{
		case OR_HORZ:
			pattern = cairo_pattern_create_linear(0, 0, rect.right - rect.left, 0);

			if (rightToLeft.GetRightToLeft())
			{
				cairo_pattern_add_color_stop_rgb(pattern, 1, color1.GetRed() / 255.0, color1.GetGreen() / 255.0, color1.GetBlue() / 255.0);
				cairo_pattern_add_color_stop_rgb(pattern, 0, color2.GetRed() / 255.0, color2.GetGreen() / 255.0, color2.GetBlue() / 255.0);
			}
			else
			{
				cairo_pattern_add_color_stop_rgb(pattern, 0, color1.GetRed() / 255.0, color1.GetGreen() / 255.0, color1.GetBlue() / 255.0);
				cairo_pattern_add_color_stop_rgb(pattern, 1, color2.GetRed() / 255.0, color2.GetGreen() / 255.0, color2.GetBlue() / 255.0);
			}

			break;
		case OR_VERT:
			pattern = cairo_pattern_create_linear(0, 0, 0, rect.bottom - rect.top);

			cairo_pattern_add_color_stop_rgb(pattern, 0, color1.GetRed() / 255.0, color1.GetGreen() / 255.0, color1.GetBlue() / 255.0);
			cairo_pattern_add_color_stop_rgb(pattern, 1, color2.GetRed() / 255.0, color2.GetGreen() / 255.0, color2.GetBlue() / 255.0);

			break;
	}

	if (!painting)
	{
		CreateCairoContext();

		cairo_set_source(context, pattern);
		cairo_rectangle(context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		cairo_fill(context);

		DestroyCairoContext();
	}

	cairo_set_source(cairo_contexts.GetLast(), pattern);
	cairo_rectangle(cairo_contexts.GetLast(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	cairo_fill(cairo_contexts.GetLast());

	cairo_pattern_destroy(pattern);

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();

	Bitmap	 bitmap	  = oBitmap;
	Rect	 destRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iDestRect));

#ifdef __WIN32__
	HDC	 gdi_dc	  = GetWindowDC(window);
	HDC	 cdc	  = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	  = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		if (!painting) BitBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
		BitBlt(cDc_contexts.GetLast(), destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		if (!painting) StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
		StretchBlt(cDc_contexts.GetLast(), destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);
	ReleaseDC(window, gdi_dc);
#endif

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
{
	if (window == NIL) return Success();

	Bitmap	 bitmap	 = oBitmap;
	Rect	 srcRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iSrcRect));

#ifdef __WIN32__
	HDC	 gdi_dc	 = GetWindowDC(window);
	HDC	 cdc	 = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	 = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);
	ReleaseDC(window, gdi_dc);
#endif

	return Success();
}
