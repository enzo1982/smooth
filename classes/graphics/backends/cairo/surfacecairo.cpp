 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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

#include <cairo/cairo-win32.h>

S::GUI::SurfaceBackend *CreateSurfaceCairo(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCairo(iSurface, maxSize);
}

S::Int	 surfaceCairoTmp = S::GUI::SurfaceBackend::AddBackend(&CreateSurfaceCairo);

S::GUI::SurfaceCairo::SurfaceCairo(Void *iWindow, const Size &maxSize)
{
	type = SURFACE_CAIRO;

	window = (HWND) iWindow;

	surface = NIL;
	context = NIL;

	if (window != NIL)
	{
		dc = GetWindowDC(window);

		size = maxSize;

		if (maxSize == Size())
		{
			size.cx	= GetDeviceCaps(dc, HORZRES) + 2;
			size.cy	= GetDeviceCaps(dc, VERTRES) + 2;
		}

		rightToLeft.SetSurfaceSize(size);

		allocSize = size;
	}
}

S::GUI::SurfaceCairo::~SurfaceCairo()
{
	if (window != NIL)
	{
		ReleaseDC(window, dc);
	}
}

S::Int S::GUI::SurfaceCairo::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceCairo::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceCairo::PaintRect(const Rect &pRect)
{
	return Error();
}

S::Int S::GUI::SurfaceCairo::StartPaint(const Rect &iPRect)
{
	return Success();
}

S::Int S::GUI::SurfaceCairo::EndPaint()
{
	return Success();
}

S::Void *S::GUI::SurfaceCairo::GetSystemSurface() const
{
	return (Void *) window;
}

S::Int S::GUI::SurfaceCairo::CreateContext()
{
	if (surface != NIL || context != NIL) return Error();

	surface = cairo_win32_surface_create(dc);
	context = cairo_create(surface);

	cairo_set_antialias(context, CAIRO_ANTIALIAS_NONE);

	return Success();
}

S::Int S::GUI::SurfaceCairo::DestroyContext()
{
	if (surface == NIL || context == NIL) return Error();

	cairo_destroy(context);
	cairo_surface_destroy(surface);

	context = NIL;
	surface = NIL;

	return Success();
}

S::Int S::GUI::SurfaceCairo::SetPixel(Int x, Int y, const Color &color)
{
	if (window == NIL) return Success();

	CreateContext();

	cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_rectangle(context, rightToLeft.TranslateX(fontSize.TranslateX(x)), rightToLeft.TranslateY(fontSize.TranslateY(y)), 1, 1);
	cairo_fill(context);

	DestroyContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos1));
	Point	 pos2 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos2));

	CreateContext();

	cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);
	cairo_set_line_width(context, 1);
	cairo_move_to(context, pos1.x, pos1.y);
	cairo_line_to(context, pos2.x, pos2.y);
	cairo_stroke(context);

	DestroyContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	CreateContext();

	cairo_set_source_rgb(context, color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
		}
		else
		{
			cairo_rectangle(context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			cairo_fill(context);
		}
	}
	else if (style == Rect::Outlined)
	{
		cairo_rectangle(context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		cairo_stroke(context);
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
				cairo_rectangle(context, x, y, 1, 1);
				cairo_fill(context);

				dot = False;
			}
			else dot = True;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == True)
			{
				cairo_rectangle(context, x, y, 1, 1);
				cairo_fill(context);

				dot = False;
			}
			else dot = True;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == True)
			{
				cairo_rectangle(context, x, y, 1, 1);
				cairo_fill(context);

				dot = False;
			}
			else dot = True;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == True)
			{
				cairo_rectangle(context, x, y, 1, 1);
				cairo_fill(context);

				dot = False;
			}
			else dot = True;
		}
	}

	DestroyContext();

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
	int	 height = font.GetLineSizeY(string) + 3;
	int	 txtsize = string.Length();
	String	 line;
	Rect	 rect = iRect;

	CreateContext();

	cairo_select_font_face(context, font.GetName(),
			       (font.GetStyle() == Font::Italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL),
			       (font.GetWeight() == Font::Bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL));

	cairo_set_font_size(context, font.GetSize() * fontSize.TranslateY(96) / 72.0);

	cairo_set_source_rgb(context, font.GetColor().GetRed() / 255.0, font.GetColor().GetGreen() / 255.0, font.GetColor().GetBlue() / 255.0);

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

		RECT	 Rect = rightToLeft.TranslateRect(fontSize.TranslateRect(rect));

		if (rightToLeft.GetRightToLeft()) Rect.right--;

		cairo_move_to(context, Rect.left, Rect.top + font.GetSize() * fontSize.TranslateY(96) / 72.0);
		cairo_show_text(context, line.ConvertTo("UTF-8"));

		rect.top += height;
	}

	DestroyContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	CreateContext();

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

	cairo_set_source(context, pattern);
	cairo_rectangle(context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	cairo_fill(context);

	cairo_pattern_destroy(pattern);

	DestroyContext();

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();

	Bitmap	 bitmap	  = oBitmap;
	Rect	 destRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iDestRect));
	HDC	 gdi_dc	  = GetWindowDC(window);
	HDC	 cdc	  = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	  = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);
	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceCairo::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
{
	if (window == NIL) return Success();

	Bitmap	 bitmap	 = oBitmap;
	Rect	 srcRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iSrcRect));
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

	return Success();
}
