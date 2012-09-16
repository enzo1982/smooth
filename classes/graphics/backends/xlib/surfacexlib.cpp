 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <unistd.h>
#include <stdio.h>

using namespace X11;

S::GUI::SurfaceBackend *CreateSurfaceXLib(S::Void *iSurface, const S::GUI::Size &)
{
	return new S::GUI::SurfaceXLib(iSurface);
}

S::Int		 surfaceXLibTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceXLib);

S::Short	 S::GUI::SurfaceXLib::surfaceDPI = -1;

S::GUI::SurfaceXLib::SurfaceXLib(Void *iWindow, const Size &maxSize)
{
	type	= SURFACE_XLIB;

	window	= (Window) iWindow;

	bitmap	= NIL;
	gc	= NIL;

	display	= Backends::BackendXLib::GetDisplay();

	if (window != NIL)
	{
		size = maxSize;

		XGetWindowAttributes(display, window, &windowAttributes);

		if (maxSize == Size())
		{
			size.cx = XDisplayWidth(display, XDefaultScreen(display)) + 2;
			size.cy = XDisplayHeight(display, XDefaultScreen(display)) + 2;
		}

		rightToLeft.SetSurfaceSize(size);

		bitmap = XCreatePixmap(display, window, size.cx, size.cy, windowAttributes.depth);
		gc     = XCreateGC(display, bitmap, 0, NIL);

		paintRects.Add(new Rect(Point(0, 0), size));

		allocSize = size;
	}

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceXLib::~SurfaceXLib()
{
	if (bitmap != NIL)
	{
		XFreeGC(display, gc);
		XFreePixmap(display, bitmap);

		delete paintRects.GetFirst();
	}
}

S::Int S::GUI::SurfaceXLib::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		XFreeGC(display, gc);
		XFreePixmap(display, bitmap);

		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		bitmap = XCreatePixmap(display, DefaultRootWindow(display), size.cx, size.cy, windowAttributes.depth);
		gc     = XCreateGC(display, bitmap, 0, NIL);

		paintRects.Add(new Rect(Point(0, 0), size));
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceXLib::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceXLib::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		XCopyArea(display, bitmap, window, gc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, pRect.left, pRect.top);
	}

	return Success();
}

S::Int S::GUI::SurfaceXLib::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect		 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), *(paintRects.GetLast()));
	XRectangle	 clipRect;

	clipRect.x	= pRect.left;
	clipRect.y	= pRect.top;
	clipRect.width	= pRect.GetWidth();
	clipRect.height	= pRect.GetHeight();

	XSetClipRectangles(display, gc, 0, 0, &clipRect, 1, Unsorted);

	paintRects.Add(new Rect(pRect));

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceXLib::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0)
	{
		PaintRect(*(paintRects.GetLast()));

		XSetClipMask(display, gc, None);
	}
	else
	{
		Rect		*paintRect = paintRects.GetNth(paintRects.Length() - 2);
		XRectangle	 clipRect;

		clipRect.x	= paintRect->left;
		clipRect.y	= paintRect->top;
		clipRect.width	= paintRect->GetWidth();
		clipRect.height	= paintRect->GetHeight();

		XSetClipRectangles(display, gc, 0, 0, &clipRect, 1, Unsorted);
	}

	delete paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	return Success();
}

S::Void *S::GUI::SurfaceXLib::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceXLib::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	Short	 dpi = 96;

	/* Search the path for gsettings.
	 */
	String			 path  = getenv("PATH");
	const Array<String>	&paths = path.Explode(":");

	foreach (const String &path, paths)
	{
		/* Check for gsettings in this path.
		 */
		if (File(String(path).Append("/").Append("gsettings")).Exists())
		{
			/* If gsettings exists, use it to get the font scaling factor.
			 */
			FILE	*pstdin = popen("gsettings get org.gnome.desktop.interface text-scaling-factor", "r");

			if (pstdin != NIL)
			{
				float	 factor = 1.0;

				if (fscanf(pstdin, "%f", &factor) > 0) dpi = Math::Round(96.0 * factor);

				pclose(pstdin);
			}

			break;
		}
	}

	String::ExplodeFinish();

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceXLib::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

	XChangeGC(display, gc, GCForeground, &gcValues);

	if (!painting) XDrawPoint(display, window, gc, point.x, point.y);

	XDrawPoint(display, bitmap, gc, point.x, point.y);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if	(pos1.x < pos2.x) pos2.x--;
		else if (pos1.x > pos2.x) pos2.x++;

		if	(pos1.y < pos2.y) pos2.y--;
		else if (pos1.y > pos2.y) pos2.y++;
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (pos1.x == pos2.x && pos1.y < pos2.y) pos2.y--;
	if (pos1.x == pos2.x && pos1.y > pos2.y) pos1.y--;

	if (pos1.y == pos2.y && pos1.x < pos2.x) pos2.x--;
	if (pos1.y == pos2.y && pos1.x > pos2.x) pos1.x--;

	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

	XChangeGC(display, gc, GCForeground, &gcValues);

	if (!painting) XDrawLine(display, window, gc, pos1.x, pos1.y, pos2.x, pos2.y);

	XDrawLine(display, bitmap, gc, pos1.x, pos1.y, pos2.x, pos2.y);

	return Success();
}

S::Int S::GUI::SurfaceXLib::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect		 rect = rightToLeft.TranslateRect(iRect);
	XGCValues	 gcValues;

	gcValues.foreground = Color(color.GetBlue(), color.GetGreen(), color.GetRed());

	XChangeGC(display, gc, GCForeground, &gcValues);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			/* ToDo: Allow drawing of rounded rects.
			 */
		}
		else
		{
			if (!painting) XFillRectangle(display, window, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

			XFillRectangle(display, bitmap, gc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting) XDrawRectangle(display, window, gc, rect.left, rect.top, rect.right - rect.left - 1, rect.bottom - rect.top - 1);

		XDrawRectangle(display, bitmap, gc, rect.left, rect.top, rect.right - rect.left - 1, rect.bottom - rect.top - 1);
	}
	else if (style & Rect::Inverted)
	{
		Bitmap	 area(rect.right - rect.left, rect.bottom - rect.top);

		BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

		area.InvertColors();

		BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
	}
	else if (style & Rect::Dotted)
	{
		if (!painting)
		{
			for (Int x = rect.left								 + 1;  x <  rect.right;	 x += 2) XDrawPoint(display, window, gc, x, rect.top);
			for (Int y = rect.top	 - (rect.right - rect.left			   ) % 2 + 2;  y <  rect.bottom; y += 2) XDrawPoint(display, window, gc, rect.right - 1, y);
			for (Int x = rect.right	 - (rect.right - rect.left + rect.bottom - rect.top) % 2 - 2;  x >= rect.left;	 x -= 2) XDrawPoint(display, window, gc, x, rect.bottom - 1);
			for (Int y = rect.bottom - (			     rect.bottom - rect.top) % 2 - 1;  y >= rect.top;	 y -= 2) XDrawPoint(display, window, gc, rect.left, y);
		}

		for (Int x = rect.left								 + 1;  x <  rect.right;	 x += 2) XDrawPoint(display, bitmap, gc, x, rect.top);
		for (Int y = rect.top	 - (rect.right - rect.left			   ) % 2 + 2;  y <  rect.bottom; y += 2) XDrawPoint(display, bitmap, gc, rect.right - 1, y);
		for (Int x = rect.right	 - (rect.right - rect.left + rect.bottom - rect.top) % 2 - 2;  x >= rect.left;	 x -= 2) XDrawPoint(display, bitmap, gc, x, rect.bottom - 1);
		for (Int y = rect.bottom - (			     rect.bottom - rect.top) % 2 - 1;  y >= rect.top;	 y -= 2) XDrawPoint(display, bitmap, gc, rect.left, y);
	}

	return Success();
}

S::Int S::GUI::SurfaceXLib::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	Rect		 rect	    = iRect;
	Int		 lineHeight = font.GetScaledTextSizeY() + 3;

	XGCValues	 gcValues;
	X11::Font	 xfont = XLoadFont(display, String("-*-").Append(font.GetName().ToLower()).Append("-").Append(font.GetWeight() >= Font::Bold ? "bold" : "medium").Append("-").Append(font.GetStyle() & Font::Italic ? "i" : "r").Append("-normal-*-").Append(String::FromInt(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0))).Append("-*-*-*-*-*-*-*"));

	gcValues.foreground = Color(font.GetColor().GetBlue(), font.GetColor().GetGreen(), font.GetColor().GetRed());
	gcValues.font	    = xfont;

	XChangeGC(display, gc, GCForeground | GCFont, &gcValues);

	const Array<String>	&lines = string.Explode("\n");

	foreach (const String &line, lines)
	{
		Rect	 tRect = rightToLeft.TranslateRect(rect);

		if (Setup::enableUnicode)
		{
			if (!painting) XDrawString16(display, window, gc, tRect.left, tRect.top + lineHeight - 4, (XChar2b *) line.ConvertTo("UTF-16BE"), line.Length());

			XDrawString16(display, bitmap, gc, tRect.left, tRect.top + lineHeight - 4, (XChar2b *) line.ConvertTo("UTF-16BE"), line.Length());
		}
		else
		{
			if (!painting) XDrawString(display, window, gc, tRect.left, tRect.top + lineHeight - 4, line, line.Length());

			XDrawString(display, bitmap, gc, tRect.left, tRect.top + lineHeight - 4, line, line.Length());
		}

		rect.top += lineHeight;
	}

	String::ExplodeFinish();

	XUnloadFont(display, xfont);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		if (!painting) XCopyArea(display, (Drawable) bitmap.GetSystemBitmap(), window, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);

		XCopyArea(display, (Drawable) bitmap.GetSystemBitmap(), this->bitmap, gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);
	}
	else
	{
		/* ToDo: Allow copying from bitmaps of different
		 *	 size than destination.
		 */
	}

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		XCopyArea(display, this->bitmap, (Drawable) bitmap.GetSystemBitmap(), gc, srcRect.left, srcRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, destRect.left, destRect.top);
	}
	else
	{
		/* ToDo: Allow copying to bitmaps of different
		 *	 size than original.
		 */
	}

	return Success();
}
