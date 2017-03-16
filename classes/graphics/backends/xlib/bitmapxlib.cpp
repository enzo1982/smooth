 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
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

S::Bool S::GUI::BitmapXLib::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1)		  nDepth = XDefaultDepth(display, XDefaultScreen(display));
	if (nDepth != 24 && nDepth != 32) nDepth = 24;

	/* Find best fit for pixel format.
	 */
	int			 count	 = 0;
	int			 index	 = 0;
	XPixmapFormatValues	*formats = XListPixmapFormats(display, &count);

	for (Int i = 0; i < count; i++)
	{
		if (Math::Abs(formats[i].depth - nDepth) < Math::Abs(formats[index].depth - nDepth)) index = i;

		if (formats[i].depth == nDepth) break;
	}

	/* Allocate and create image.
	 */
	bytes	= new UnsignedByte [nSize.cy * nSize.cx * (formats[index].bits_per_pixel / 8) + nSize.cy * (((nSize.cx * formats[index].bits_per_pixel) % formats[index].scanline_pad) / 8)];
	bitmap	= XCreateImage(display, XDefaultVisual(display, XDefaultScreen(display)), formats[index].depth, ZPixmap, 0, (char *) bytes, nSize.cx, nSize.cy, formats[index].scanline_pad, 0);

	if (bitmap == NIL) { delete [] (UnsignedByte *) bytes; XFree(formats); return False; }

	size	= nSize;
	depth	= formats[index].depth;
	bpp	= formats[index].bits_per_pixel;
	align	= formats[index].scanline_pad / 8;

	XFree(formats);

	return True;
}

S::Bool S::GUI::BitmapXLib::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		delete [] bitmap->data;

		bitmap->data = NIL;

		XDestroyImage(bitmap);

		bitmap	     = NIL;

		size	     = Size(0, 0);
		depth	     = 0;

		bytes	     = NIL;
		bpp	     = 0;
		align	     = 0;
	}

	return True;
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
		XImage	*image = (XImage *) nBitmap;

		CreateBitmap(Size(image->width, image->height), image->depth);

		memcpy(bitmap->data, image->data, image->height * image->bytes_per_line);
	}

	return True;
}

S::Void *S::GUI::BitmapXLib::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapXLib::SetPixel(const Point &iPoint, const Color &iColor)
{
	if (bitmap == NIL) return Error();

	Color	 color	= iColor.ConvertTo(Color::RGBA);

	if	(depth == 16) XPutPixel(bitmap, iPoint.x, iPoint.y, 			       ((color.GetRed() >> 3) << 11) | ((color.GetGreen() >> 2) << 5) | (color.GetBlue() >> 3));
	else if (depth == 24) XPutPixel(bitmap, iPoint.x, iPoint.y, 			       ( color.GetRed()	      << 16) | ( color.GetGreen()       << 8) | (color.GetBlue()     ));
	else if (depth == 32) XPutPixel(bitmap, iPoint.x, iPoint.y, (color.GetAlpha() << 24) | ( color.GetRed()	      << 16) | ( color.GetGreen()	<< 8) | (color.GetBlue()     ));

	return True;
}

S::GUI::Color S::GUI::BitmapXLib::GetPixel(const Point &iPoint) const
{
	if (bitmap == NIL) return Color();

	Long	 value = XGetPixel(bitmap, iPoint.x, iPoint.y);

	if	(depth == 16) return				   Color(((value >> 11) &  31) << 3, ((value >> 5) &  63) << 2, (value &  31) << 3);
	else if (depth == 24) return				   Color( (value >> 16) & 255,	      (value >> 8) & 255,	 value & 255      );
	else if (depth == 32) return ((value >> 24) & 255) << 24 | Color( (value >> 16) & 255,	      (value >> 8) & 255,	 value & 255      );

	return Color();
}

S::GUI::BitmapBackend &S::GUI::BitmapXLib::operator =(const BitmapBackend &newBitmap)
{
	if (&newBitmap == this) return *this;

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
