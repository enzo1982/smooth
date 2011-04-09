 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
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

using namespace X11;

S::GUI::SurfaceBackend *CreateSurfaceXLib(S::Void *iSurface, const S::GUI::Size &)
{
	return new S::GUI::SurfaceXLib(iSurface);
}

S::Int	 surfaceXLibTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceXLib);

S::GUI::SurfaceXLib::SurfaceXLib(Void *iDc, const Size &maxSize)
{
	type	= SURFACE_XLIB;

	window	= (Window) iDc;
	rWindow	= (Window) iDc;

	bitmap	= NIL;

	display	= Backends::BackendXLib::GetDisplay();

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
	Rect	 pRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iPRect));

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

S::Int S::GUI::SurfaceXLib::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPoint));

	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	XDrawPoint(display, window, gc, point.x, point.y);
	XDrawPoint(display, bitmap, gc, point.x, point.y);

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	Point	 pos1 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos1));
	Point	 pos2 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos2));

	XDrawLine(display, window, gc, pos1.x, pos1.y, pos2.x, pos2.y);
	XDrawLine(display, bitmap, gc, pos1.x, pos1.y, pos2.x, pos2.y);

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect		 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));
	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

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
		for (Int x = rect.left								 + 1;  x <  rect.right;	 x += 2) SetPixel(Point(x, rect.top), color);
		for (Int y = rect.top	 - (rect.right - rect.left			   ) % 2 + 2;  y <  rect.bottom; y += 2) SetPixel(Point(rect.right - 1, y), color);
		for (Int x = rect.right	 - (rect.right - rect.left + rect.bottom - rect.top) % 2 - 2;  x >= rect.left;	 x -= 2) SetPixel(Point(x, rect.bottom - 1), color);
		for (Int y = rect.bottom - (			     rect.bottom - rect.top) % 2 - 1;  y >= rect.top;	 y -= 2) SetPixel(Point(rect.left, y), color);
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
	int	 lineHeight = font.GetTextSizeY() + 3;
	int	 txtsize = string.Length();
	String	 line;

	XGCValues	 gcValues;

	gcValues.foreground = Color(font.GetColor().GetBlue(), font.GetColor().GetGreen(), font.GetColor().GetRed());

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

		Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

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

		rect.top += lineHeight;
	}

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitFromBitmap(const Bitmap &bmp, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bmp	   == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iDestRect));
	GC	 gc	  = XCreateGC(display, bitmap, 0, NIL);

	XCopyArea(display, (Drawable) bmp.GetSystemBitmap(), window, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);
	XCopyArea(display, (Drawable) bmp.GetSystemBitmap(), bitmap, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);

	XFreeGC(display, gc);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitToBitmap(const Rect &iSrcRect, const Bitmap &bmp, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bmp	   == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iSrcRect));
	GC	 gc	 = XCreateGC(display, bitmap, 0, NIL);

	XCopyArea(display, window, (Drawable) bmp.GetSystemBitmap(), gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);

	XFreeGC(display, gc);

	return Success();
}
