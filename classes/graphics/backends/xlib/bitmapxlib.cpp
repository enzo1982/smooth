 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/xlib/bitmapxlib.h>
#include <smooth/backends/xlib/backendxlib.h>

using namespace X11;

S::GUI::BitmapBackend *CreateBitmapXLib_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapXLib(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapXLib_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapXLib(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapXLib_cI(const int nil)
{
	return new S::GUI::BitmapXLib(nil);
}

S::GUI::BitmapBackend *CreateBitmapXLib_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapXLib((const S::GUI::BitmapXLib &) iBitmap);
}

S::Int	 bitmapXLibTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_pV);
S::Int	 bitmapXLibTmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_crSI);
S::Int	 bitmapXLibTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_cI);
S::Int	 bitmapXLibTmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_crB);

S::GUI::BitmapXLib::BitmapXLib(Void *iBitmap)
{
	Initialize();

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapXLib::BitmapXLib(const Size &iSize, Int iDepth)
{
	Initialize();

	CreateBitmap(iSize, iDepth);
}

S::GUI::BitmapXLib::BitmapXLib(const int nil)
{
	Initialize();

	SetSystemBitmap(NIL);
}

S::GUI::BitmapXLib::BitmapXLib(const BitmapXLib &iBitmap)
{
	Initialize();

	SetSystemBitmap((Void *) iBitmap.bitmap);
}

S::GUI::BitmapXLib::~BitmapXLib()
{
	DeleteBitmap();
}

S::Void S::GUI::BitmapXLib::Initialize()
{
	type	= BITMAP_XLIB;
	bitmap	= NIL;

	display	= Backends::BackendXLib::GetDisplay();
}

S::Bool S::GUI::BitmapXLib::IsDepthSupported(Int bpp)
{
	Bool			 supported = False;
	int			 count	   = 0;
	XPixmapFormatValues	*values	   = XListPixmapFormats(display, &count);

	for (int i = 0; i < count; i++) if (values[i].depth == bpp) { supported = True; break; }

	XFree(values);

	return supported;
}

S::Bool S::GUI::BitmapXLib::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1 || !IsDepthSupported(nDepth)) nDepth = XDefaultDepth(display, XDefaultScreen(display));

	bitmap	= XCreatePixmap(display, DefaultRootWindow(display), nSize.cx, nSize.cy, nDepth);
	bytes	= NIL;

	if (bitmap == NIL) return False;

	size	= nSize;
	depth	= nDepth;
	bpp	= depth;
	align	= 4;

	return True;
}

S::Bool S::GUI::BitmapXLib::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		XFreePixmap(display, bitmap);

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Int S::GUI::BitmapXLib::Scale(const Size &newSize)
{
	if (bitmap == NIL) return Error();

	if (size == newSize) return Success();

	Pixmap	 backup = bitmap;
	Size	 backupSize = size;

	bitmap = NIL;

	CreateBitmap(newSize, depth);

	GC		 gc = XCreateGC(display, bitmap, 0, NIL);
	XImage		*image = XGetImage(display, backup, 0, 0, backupSize.cx, backupSize.cy, AllPlanes, XYPixmap);
	Point		 point;
	XGCValues	 gcValues;

	Float	 scaleFactorX = backupSize.cx / newSize.cx;
	Float	 scaleFactorY = backupSize.cy / newSize.cy;

	for (point.y = 0; point.y < newSize.cy; point.y++)
	{
		for (point.x = 0; point.x < newSize.cx; point.x++)
		{
			Float	 red = 0, green = 0, blue = 0;

			for (Int srcX = point.x * scaleFactorX; srcX < (point.x + 1) * scaleFactorX; srcX++)
			{
				for (Int srcY = point.y * scaleFactorY; srcY < (point.y + 1) * scaleFactorY; srcY++)
				{
					Long	 value = XGetPixel(image, srcX, srcY);

					red   += Float((value >> 16) & 255) * (Math::Min(1.0, (point.x + 1) * scaleFactorX - srcX) * Math::Min(1.0, (point.y + 1) * scaleFactorY - srcY));
					green += Float((value >> 8)  & 255) * (Math::Min(1.0, (point.x + 1) * scaleFactorX - srcX) * Math::Min(1.0, (point.y + 1) * scaleFactorY - srcY));
					blue  += Float( value	     & 255) * (Math::Min(1.0, (point.x + 1) * scaleFactorX - srcX) * Math::Min(1.0, (point.y + 1) * scaleFactorY - srcY));
				}
			}

			red   /= scaleFactorX * scaleFactorY;
			green /= scaleFactorX * scaleFactorY;
			blue  /= scaleFactorX * scaleFactorY;

			if	(depth == 16) gcValues.foreground =		  ((Int(red) >> 3) << 11) | ((Int(green) >> 2) << 5) | (Int(blue) >> 3);
			else if (depth == 24) gcValues.foreground =		  ( Int(red)       << 16) | ( Int(green) << 8)	     | (Int(blue)     );
			else if (depth == 32) gcValues.foreground = (255 << 24) | ( Int(red)       << 16) | ( Int(green) << 8)	     | (Int(blue)     );

			XChangeGC(display, gc, GCForeground, &gcValues);

			XDrawPoint(display, bitmap, gc, point.x, point.y);
		}
	}

	XFreeGC(display, gc);

	XFreePixmap(display, backup);

	return Success();
}

S::Bool S::GUI::BitmapXLib::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		unsigned int	 cx		= 0;
		unsigned int	 cy		= 0;
		unsigned int	 bpp		= 0;
		Window		 windowDummy	= NIL;
		int		 intDummy	= 0;
		unsigned int	 uIntDummy	= 0;

		XGetGeometry(display, (Pixmap) nBitmap, &windowDummy, &intDummy, &intDummy, &cx, &cy, &uIntDummy, &bpp);

		CreateBitmap(Size(cx, cy), bpp);

		GC	 gc = XCreateGC(display, bitmap, 0, NIL);

		XCopyArea(display, (Pixmap) nBitmap, bitmap, gc, 0, 0, cx, cy, 0, 0);

		XFreeGC(display, gc);
	}

	return True;
}

S::Void *S::GUI::BitmapXLib::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Int S::GUI::BitmapXLib::GrayscaleBitmap()
{
	if (bitmap == NIL) return Error();

	XImage	*image = XGetImage(display, bitmap, 0, 0, size.cx, size.cy, AllPlanes, XYPixmap);
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Long	 value = XGetPixel(image, point.x, point.y);
			Color	 pixel = Color(((value >> 24) & 255) << 24 | (value & 255) << 16 | ((value >> 8) & 255) << 8 | ((value >> 16) & 255));

			SetPixel(point, pixel.Grayscale());
		}
	}

	XDestroyImage(image);

	return Success();
}

S::Int S::GUI::BitmapXLib::InvertColors()
{
	if (bitmap == NIL) return Error();

	XImage	*image = XGetImage(display, bitmap, 0, 0, size.cx, size.cy, AllPlanes, XYPixmap);
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Long	 value = XGetPixel(image, point.x, point.y);
			Color	 pixel = Color(((value >> 24) & 255) << 24 | (value & 255) << 16 | ((value >> 8) & 255) << 8 | ((value >> 16) & 255));

			SetPixel(point, Color(255 - pixel.GetRed(), 255 - pixel.GetGreen(), 255 - pixel.GetBlue()));
		}
	}

	XDestroyImage(image);

	return Success();
}

S::Int S::GUI::BitmapXLib::ReplaceColor(const Color &color1, const Color &color2)
{
	if (bitmap == NIL) return Error();

	XImage	*image = XGetImage(display, bitmap, 0, 0, size.cx, size.cy, AllPlanes, XYPixmap);
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Long	 value = XGetPixel(image, point.x, point.y);
			Color	 pixel = Color(((value >> 24) & 255) << 24 | (value & 255) << 16 | ((value >> 8) & 255) << 8 | ((value >> 16) & 255));

			if (pixel == color1) SetPixel(point, color2);
		}
	}

	XDestroyImage(image);

	return Success();
}

S::Int S::GUI::BitmapXLib::SetBackgroundColor(const Color &color)
{
	if (bitmap == NIL) return Error();
	if (depth  != 32)  return Success();

	XImage	*image = XGetImage(display, bitmap, 0, 0, size.cx, size.cy, AllPlanes, XYPixmap);
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Long	 value = XGetPixel(image, point.x, point.y);
			Color	 pixel = Color(((value >> 24) & 255) << 24 | (value & 255) << 16 | ((value >> 8) & 255) << 8 | ((value >> 16) & 255));

			if (pixel.GetAlpha() != 255) SetPixel(point, Color((pixel.GetRed()   * pixel.GetAlpha() + color.GetRed()   * (255 - pixel.GetAlpha())) / 255,
									   (pixel.GetGreen() * pixel.GetAlpha() + color.GetGreen() * (255 - pixel.GetAlpha())) / 255,
									   (pixel.GetBlue()  * pixel.GetAlpha() + color.GetBlue()  * (255 - pixel.GetAlpha())) / 255));
		}
	}

	XDestroyImage(image);

	return Success();
}

S::Bool S::GUI::BitmapXLib::SetPixel(const Point &iPoint, const Color &color)
{
	if (bitmap == NIL) return Error();

	XGCValues	 gcValues;

	if	(depth == 16) gcValues.foreground =			       ((color.GetRed() >> 3) << 11) | ((color.GetGreen() >> 2) << 5) | (color.GetBlue() >> 3);
	else if (depth == 24) gcValues.foreground =			       ( color.GetRed()	      << 16) | ( color.GetGreen()       << 8) | (color.GetBlue()     );
	else if (depth == 32) gcValues.foreground = (color.GetAlpha() << 24) | ( color.GetRed()	      << 16) | ( color.GetGreen()	<< 8) | (color.GetBlue()     );

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	XDrawPoint(display, bitmap, gc, iPoint.x, iPoint.y);

	XFreeGC(display, gc);

	return True;
}

S::GUI::Color S::GUI::BitmapXLib::GetPixel(const Point &iPoint) const
{
	if (bitmap == NIL) return Color();

	XImage	*image = XGetImage(display, bitmap, iPoint.x, iPoint.y, 1, 1, AllPlanes, XYPixmap);
	Long	 value = XGetPixel(image, 0, 0);

	XDestroyImage(image);

	if	(depth == 16) return	 255		   << 24 | Color(((value >> 11) &  31) << 3, ((value >> 5) &  63) << 2, (value &  31) << 3);
	else if (depth == 24) return	 255		   << 24 | Color( (value >> 16) & 255,	      (value >> 8) & 255,	 value & 255      );
	else if (depth == 32) return ((value >> 24) & 255) << 24 | Color( (value >> 16) & 255,	      (value >> 8) & 255,	 value & 255      );

	return Color();
}

S::GUI::BitmapBackend &S::GUI::BitmapXLib::operator =(const BitmapBackend &newBitmap)
{
	SetSystemBitmap((Void *) ((BitmapXLib &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapXLib::operator ==(const int nil) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapXLib::operator !=(const int nil) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
