 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/xlib/surfacexlib.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

S::GUI::SurfaceBackend *CreateSurfaceXLib(S::Void *iSurface, const S::GUI::Size &)
{
	return new S::GUI::SurfaceXLib(iSurface);
}

S::Int	 surfaceXLibTmp = S::GUI::SurfaceBackend::AddBackend(&CreateSurfaceXLib);

S::GUI::SurfaceXLib::SurfaceXLib(Void *iDc, const Size &maxSize)
{
	type	= SURFACE_XLIB;

	window	= (Window) iDc;
	rWindow	= (Window) iDc;

	bitmap	= NIL;

	display	= NIL;

	for (Int i = 0; i < Backends::Backend::GetNOfBackends(); i++)
	{
		if (Backends::Backend::GetNthBackend(i)->GetBackendType() == Backends::BACKEND_XLIB) display = ((Backends::BackendXLib *) Backends::Backend::GetNthBackend(i))->GetDisplay();
	}

	if (window != NIL)
	{
		XWindowAttributes	 windowAttributes;

		XGetWindowAttributes(display, window, &windowAttributes);

		size.cx	= windowAttributes.width + 2;
		size.cy	= windowAttributes.height + 2;

		rightToLeft.SetSurfaceSize(size);

		bitmap = XCreatePixmap(display, window, size.cx, size.cy, windowAttributes.depth);

		Bitmap	 bmpXLib(bitmap);

		BlitToBitmap(Rect(Point(0, 0), size), bmpXLib, Rect(Point(0, 0), size));
	}
}

S::GUI::SurfaceXLib::~SurfaceXLib()
{
	if (bitmap != NIL)
	{
		XFreePixmap(display, bitmap);
	}
}

S::Int S::GUI::SurfaceXLib::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		GC	 gc = XCreateGC(display, bitmap, 0, NIL);

		XCopyArea(display, bitmap, window, gc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, pRect.left, pRect.top);

		XFreeGC(display, gc);
	}

	return Success();
}

S::Int S::GUI::SurfaceXLib::StartPaint(const Rect &iPRect)
{
	Rect	 pRect = rightToLeft.TranslateRect(iPRect);

	if (paintRect.left == -1 && paintRect.top == -1 && paintRect.right == -1 && paintRect.bottom == -1)
	{
		paintRect = pRect;
	}
	else
	{
		paintRect.left = (Int) Math::Min(paintRect.left, pRect.left);
		paintRect.top = (Int) Math::Min(paintRect.top, pRect.top);
		paintRect.right = (Int) Math::Max(paintRect.right, pRect.right);
		paintRect.bottom = (Int) Math::Max(paintRect.bottom, pRect.bottom);
	}

	if (!painting)
	{
		if (rWindow != NIL) window = (unsigned) -1;
	}

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceXLib::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0)
	{
		window = rWindow;

		PaintRect(paintRect);

		paintRect.left = -1;
		paintRect.top = -1;
		paintRect.right = -1;
		paintRect.bottom = -1;
	}

	return Success();
}

S::Void *S::GUI::SurfaceXLib::GetSystemSurface() const
{
	return (Void *) window;
}

S::Int S::GUI::SurfaceXLib::SetPixel(Int x, Int y, const Color &color)
{
	if (window == NIL) return Success();

	XGCValues	 gcValues;

	gcValues.foreground = color;

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	XDrawPoint(display, window, gc, rightToLeft.TranslateX(x), rightToLeft.TranslateY(y));
	XDrawPoint(display, bitmap, gc, rightToLeft.TranslateX(x), rightToLeft.TranslateY(y));

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	XGCValues	 gcValues;

	gcValues.foreground = color;

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	XDrawLine(display, window, gc, pos1.x, pos1.y, pos2.x, pos2.y);
	XDrawLine(display, bitmap, gc, pos1.x, pos1.y, pos2.x, pos2.y);

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect		 rect = rightToLeft.TranslateRect(iRect);
	XGCValues	 gcValues;

	gcValues.foreground = color;

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	if (style & Rect::Filled)
	{
		XFillRectangle(display, window, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		XFillRectangle(display, bitmap, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}
	else if (style == Rect::Outlined)
	{
		XDrawRectangle(display, window, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		XDrawRectangle(display, bitmap, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
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
				SetPixel(x, y, color);

				dot = False;
			}
			else dot = True;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == True)
			{
				SetPixel(x, y, color);

				dot = False;
			}
			else dot = True;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == True)
			{
				SetPixel(x, y, color);

				dot = False;
			}
			else dot = True;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == True)
			{
				SetPixel(x, y, color);

				dot = False;
			}
			else dot = True;
		}
	}

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
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

	XGCValues	 gcValues;

	gcValues.foreground = font.GetColor();

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	for (Int j = 0; j < txtsize; j++) if (string[j] == 10) lines++;

	for (Int i = 0; i < lines; i++)
	{
		origoffset = offset;

		for (int j = 0; j <= txtsize; j++)
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
			}
		}

		Rect	 rect = rightToLeft.TranslateRect(iRect);

		if (Setup::enableUnicode)
		{
			XDrawString16(display, window, gc, rect.left, rect.top, (XChar2b *) (wchar_t *) line, line.Length());
			XDrawString16(display, bitmap, gc, rect.left, rect.top, (XChar2b *) (wchar_t *) line, line.Length());
		}
		else
		{
			XDrawString(display, window, gc, rect.left, rect.top, line, line.Length());
			XDrawString(display, bitmap, gc, rect.left, rect.top, line, line.Length());
		}

		rect.top += height;
	}

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();

	Bitmap	 bmp	  = oBitmap;
	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);
	GC	 gc	  = XCreateGC(display, bitmap, 0, NIL);

	XCopyArea(display, (Drawable) bmp.GetSystemBitmap(), window, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);
	XCopyArea(display, (Drawable) bmp.GetSystemBitmap(), bitmap, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
{
	if (window == NIL) return Success();

	Bitmap	 bmp	 = oBitmap;
	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);
	GC	 gc	 = XCreateGC(display, bitmap, 0, NIL);

	XCopyArea(display, window, (Drawable) bmp.GetSystemBitmap(), gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);

	XFreeGC(display, gc);

	return Success();
}
