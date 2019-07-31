 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/gui/application/application.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

#include <unistd.h>
#include <stdio.h>

#include <fribidi/fribidi.h>

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

	window	= (X11::Window) iWindow;

	bitmap	= NIL;
	gc	= NIL;

	display	= Backends::BackendXLib::GetDisplay();

	if (window != NIL)
	{
		size = maxSize;

		if (maxSize == Size())
		{
			size.cx = XDisplayWidth(display, XDefaultScreen(display)) + 2;
			size.cy = XDisplayHeight(display, XDefaultScreen(display)) + 2;
		}

		rightToLeft.SetSurfaceSize(size);

		XWindowAttributes	 windowAttributes;

		XGetWindowAttributes(display, window, &windowAttributes);

		bitmap = XCreatePixmap(display, window, size.cx, size.cy, windowAttributes.depth);
		gc     = XCreateGC(display, bitmap, 0, NIL);

		paintRects.Add(Rect(Point(0, 0), size));

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

		paintRects.RemoveAll();

		XWindowAttributes	 windowAttributes;

		XGetWindowAttributes(display, window, &windowAttributes);

		bitmap = XCreatePixmap(display, DefaultRootWindow(display), size.cx, size.cy, windowAttributes.depth);
		gc     = XCreateGC(display, bitmap, 0, NIL);

		paintRects.Add(Rect(Point(0, 0), size));
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
		XCopyArea(display, bitmap, window, gc, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight(), pRect.left, pRect.top);
	}

	return Success();
}

S::Int S::GUI::SurfaceXLib::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect		 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());
	XRectangle	 clipRect;

	clipRect.x	= pRect.left;
	clipRect.y	= pRect.top;
	clipRect.width	= pRect.GetWidth();
	clipRect.height	= pRect.GetHeight();

	XSetClipRectangles(display, gc, 0, 0, &clipRect, 1, Unsorted);

	paintRects.Add(pRect);

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceXLib::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0)
	{
		PaintRect(paintRects.GetLast());

		XSetClipMask(display, gc, None);
	}
	else
	{
		const Rect	&paintRect = paintRects.GetNth(paintRects.Length() - 2);
		XRectangle	 clipRect;

		clipRect.x	= paintRect->left;
		clipRect.y	= paintRect->top;
		clipRect.width	= paintRect->GetWidth();
		clipRect.height	= paintRect->GetHeight();

		XSetClipRectangles(display, gc, 0, 0, &clipRect, 1, Unsorted);
	}

	paintRects.RemoveNth(paintRects.Length() - 1);

	return Success();
}

S::Void *S::GUI::SurfaceXLib::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceXLib::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = 96 * Application::GetScaleFactor();

	if (surfaceDPI != -1) return surfaceDPI;

	/* Evaluate GDK_SCALE setting.
	 */
	Short	 dpi   = 96;
	Int	 scale = (Int64) Number::FromIntString(getenv("GDK_SCALE"));

	if (scale > 0) dpi *= scale;

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
			if (!painting) XFillRectangle(display, window, gc, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());

			XFillRectangle(display, bitmap, gc, rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting) XDrawRectangle(display, window, gc, rect.left, rect.top, rect.GetWidth() - 1, rect.GetHeight() - 1);

		XDrawRectangle(display, bitmap, gc, rect.left, rect.top, rect.GetWidth() - 1, rect.GetHeight() - 1);
	}
	else if (style & Rect::Inverted)
	{
		Bitmap	 area(rect.GetSize());

		BlitToBitmap(iRect, area, Rect(Point(0, 0), area.GetSize()));

		area.InvertColors();

		BlitFromBitmap(area, Rect(Point(0, 0), area.GetSize()), iRect);
	}
	else if (style & Rect::Dotted)
	{
		if (!painting)
		{
			for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) XDrawPoint(display, window, gc, x, rect.top);
			for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) XDrawPoint(display, window, gc, rect.right - 1, y);
			for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) XDrawPoint(display, window, gc, x, rect.bottom - 1);
			for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) XDrawPoint(display, window, gc, rect.left, y);
		}

		for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) XDrawPoint(display, bitmap, gc, x, rect.top);
		for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) XDrawPoint(display, bitmap, gc, rect.right - 1, y);
		for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) XDrawPoint(display, bitmap, gc, x, rect.bottom - 1);
		for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) XDrawPoint(display, bitmap, gc, rect.left, y);
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

	XftFont		*xfont = XftFontOpenName(display, XDefaultScreen(display), String(font.GetName()).Append("-").Append(String::FromInt(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 96.0))).Append(":").Append(font.GetWeight() >= Font::Bold ? "bold" : "medium").Append(font.GetStyle() & Font::Italic ? ":italic" : ""));
	XftDraw		*wdraw = XftDrawCreate(display, window, XDefaultVisual(display, XDefaultScreen(display)), XDefaultColormap(display, XDefaultScreen(display)));
	XftDraw		*bdraw = XftDrawCreate(display, bitmap, XDefaultVisual(display, XDefaultScreen(display)), XDefaultColormap(display, XDefaultScreen(display)));

	/* Allocate text color.
	 */
	XRenderColor	 xrcolor;
	XftColor	 xftcolor;

	xrcolor.red   = font.GetColor().GetRed()   * 256;
	xrcolor.green = font.GetColor().GetGreen() * 256;
	xrcolor.blue  = font.GetColor().GetBlue()  * 256;
	xrcolor.alpha = 0xffff;

	XftColorAllocValue(display, DefaultVisual(display, DefaultScreen(display)), DefaultColormap(display, DefaultScreen(display)), &xrcolor, &xftcolor);

	/* Set clipping area.
	 */
	const Rect	&pRect = paintRects.GetLast();
	XRectangle	 clipRect;

	clipRect.x	= pRect.left;
	clipRect.y	= pRect.top;
	clipRect.width	= pRect.GetWidth();
	clipRect.height	= pRect.GetHeight();

	XftDrawSetClipRectangles(wdraw, 0, 0, &clipRect, 1);
	XftDrawSetClipRectangles(bdraw, 0, 0, &clipRect, 1);

	/* Draw text line by line.
	 */
	const Array<String>	&lines = string.Explode("\n");

	foreach (String line, lines)
	{
		Int	 lineLength = line.Length();

		/* Check for right to left characters in text.
		 */
		Bool	 rtlCharacters = False;

		for (Int i = 0; i < lineLength; i++) if (line[i] >= 0x0590 && line[i] <= 0x08FF) { rtlCharacters = True; break; }

		/* Draw text, reordering if necessary.
		 */
		Rect	 tRect = rightToLeft.TranslateRect(rect);

		if (rtlCharacters && Setup::useIconv)
		{
			/* Reorder the string with fribidi.
			 */
			FriBidiChar	*visual = new FriBidiChar [lineLength + 1];
			FriBidiParType	 type = (rightToLeft.GetRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

			fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), lineLength, &type, visual, NIL, NIL, NIL);

			visual[lineLength] = 0;

			line.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;
		}

		if (!painting) XftDrawString16(wdraw, &xftcolor, xfont, tRect.left, tRect.top + lineHeight - 4, (XftChar16 *) line.ConvertTo("UCS2"), lineLength);

		XftDrawString16(bdraw, &xftcolor, xfont, tRect.left, tRect.top + lineHeight - 4, (XftChar16 *) line.ConvertTo("UCS2"), lineLength);

		rect.top += lineHeight;
	}

	String::ExplodeFinish();

	/* Clean up everything.
	 */
	XftColorFree(display, DefaultVisual(display, DefaultScreen(display)), DefaultColormap(display, DefaultScreen(display)), &xftcolor);

	XftDrawDestroy(wdraw);
	XftDrawDestroy(bdraw);

	XftFontClose(display, xfont);

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Convert format if depths do not match.
	 */
	XWindowAttributes	 windowAttributes;

	XGetWindowAttributes(display, window, &windowAttributes);

	const Bitmap	*srcBitmap = &bitmap;
	Size		 srcSize   =  bitmap.GetSize();

	if (bitmap.GetDepth() != windowAttributes.depth)
	{
		Bitmap	*copy  = new Bitmap(size, windowAttributes.depth);
		XImage	*image = (XImage *) bitmap.GetSystemBitmap();
		Point	 point;

		for (point.y = 0; point.y < srcSize.cy; point.y++)
		{
			for (point.x = 0; point.x < srcSize.cx; point.x++)
			{
				Long	 value = XGetPixel(image, point.x, point.y);

				copy->SetPixel(point, Color(((value >> 24) & 255) << 24 | (value & 255) << 16 | ((value >> 8) & 255) << 8 | ((value >> 16) & 255)));
			}
		}

		srcBitmap = copy;
	}

	/* Copy the image.
	 */
	if (destRect.GetSize() == srcRect.GetSize())
	{
		if (!painting) XPutImage(display, window, gc, (XImage *) srcBitmap->GetSystemBitmap(), srcRect.left, srcRect.top, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());

		XPutImage(display, this->bitmap, gc, (XImage *) srcBitmap->GetSystemBitmap(), srcRect.left, srcRect.top, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	}
	else
	{
		Float	 scaleFactorX = Float(srcRect.GetWidth()) / Float(destRect.GetWidth());
		Float	 scaleFactorY = Float(srcRect.GetHeight()) / Float(destRect.GetHeight());

		const Bitmap	&bitmap = srcBitmap->Scale(Size(Float(srcSize.cx) / scaleFactorX, Float(srcSize.cy) / scaleFactorY));

		if (!painting) XPutImage(display, window, gc, (XImage *) bitmap.GetSystemBitmap(), Float(srcRect.left) / scaleFactorX, Float(srcRect.top) / scaleFactorY, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());

		XPutImage(display, this->bitmap, gc, (XImage *) bitmap.GetSystemBitmap(), Float(srcRect.left) / scaleFactorX, Float(srcRect.top) / scaleFactorY, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight());
	}

	/* Delete copy if we created one earlier.
	 */
	if (bitmap.GetDepth() != windowAttributes.depth) delete srcBitmap;

	return Success();
}

S::Int S::GUI::SurfaceXLib::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	XImage	*image = XGetImage(display, this->bitmap, srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight(), AllPlanes, ZPixmap);
	Point	 point;

	Float	 scaleFactorX = Float(srcRect.GetWidth()) / Float(destRect.GetWidth());
	Float	 scaleFactorY = Float(srcRect.GetHeight()) / Float(destRect.GetHeight());

	for (point.y = 0; point.y < destRect.GetHeight(); point.y++)
	{
		for (point.x = 0; point.x < destRect.GetWidth(); point.x++)
		{
			Float	 red = 0, green = 0, blue = 0;

			for (Int srcX = Math::Floor(Float(point.x) * scaleFactorX); srcX < Math::Ceil(Float(point.x + 1) * scaleFactorX); srcX++)
			{
				for (Int srcY = Math::Floor(Float(point.y) * scaleFactorY); srcY < Math::Ceil(Float(point.y + 1) * scaleFactorY); srcY++)
				{
					Long	 value	 = XGetPixel(image, srcX, srcY);

					Float	 weightX = (1.0 - Math::Max(0.0, Float(point.x) * scaleFactorX - Float(srcX)) - Math::Max(0.0, Float(srcX + 1) - Float(point.x + 1) * scaleFactorX)) / scaleFactorX;
					Float	 weightY = (1.0 - Math::Max(0.0, Float(point.y) * scaleFactorY - Float(srcY)) - Math::Max(0.0, Float(srcY + 1) - Float(point.y + 1) * scaleFactorY)) / scaleFactorY;

					red   += Float((value >> 16) & 255) * weightX * weightY;
					green += Float((value >> 8)  & 255) * weightX * weightY;
					blue  += Float( value	     & 255) * weightX * weightY;
				}
			}

			bitmap.SetPixel(point, Color(red, green, blue));
		}
	}

	XDestroyImage(image);

	return Success();
}
