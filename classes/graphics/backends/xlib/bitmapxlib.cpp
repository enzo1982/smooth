 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::BitmapBackend *CreateBitmapXLib_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapXLib(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapXLib_cI(const int nil)
{
	return new S::GUI::BitmapXLib(nil);
}

S::GUI::BitmapBackend *CreateBitmapXLib_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapXLib((const S::GUI::BitmapXLib &) iBitmap);
}

S::Int	 bitmapXLibTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_pV);
S::Int	 bitmapXLibTmp_III	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_III);
S::Int	 bitmapXLibTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_cI);
S::Int	 bitmapXLibTmp_crS	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapXLib_crS);

S::GUI::BitmapXLib::BitmapXLib(Void *iBitmap)
{
	Initialize();

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapXLib::BitmapXLib(Int cx, Int cy, Int bpp)
{
	Initialize();

	CreateBitmap(cx, cy, bpp);
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

S::Bool S::GUI::BitmapXLib::CreateBitmap(Int cx, Int cy, Int bpp)
{
	DeleteBitmap();

	if (bpp == -1)				 bpp = XDefaultDepth(display, XDefaultScreen(display));
	if (bpp != 16 && bpp != 24 && bpp != 32) bpp = 32;
	
	bitmap	= XCreatePixmap(display, DefaultRootWindow(display), cx, cy, bpp);
	bytes	= (Void *) -1;	

	if (bitmap == NIL) return False;

	size	= Size(cx, cy);
	depth	= bpp;

	align	= cx * (bpp / 8);

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
		align	= 0;
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
		unsigned int	 cx		= 0;
		unsigned int	 cy		= 0;
		unsigned int	 bpp		= 0;
		Window		 windowDummy	= NIL;
		int		 intDummy	= 0;
		unsigned int	 uIntDummy	= 0;

		XGetGeometry(display, (Pixmap) nBitmap, &windowDummy, &intDummy, &intDummy, &cx, &cy, &uIntDummy, &bpp);

		CreateBitmap(cx, cy, bpp);

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

S::Bool S::GUI::BitmapXLib::SetPixel(const Point &iPoint, const Color &color)
{
	XGCValues	 gcValues;

	if (depth == 16) gcValues.foreground = ((color.GetRed() >> 3) << 11) | ((color.GetGreen() >> 2) << 5) | (color.GetBlue() >> 3);
	else		 gcValues.foreground = ( color.GetRed()	      << 16) | ( color.GetGreen()	<< 8) | (color.GetBlue()     );

	GC	 gc = XCreateGC(display, bitmap, GCForeground, &gcValues);

	XDrawPoint(display, bitmap, gc, iPoint.x, iPoint.y);

	XFreeGC(display, gc);

	return True;
}

S::GUI::Color S::GUI::BitmapXLib::GetPixel(const Point &iPoint) const
{
	XImage	*image = XGetImage(display, bitmap, iPoint.x, iPoint.y, 1, 1, AllPlanes, XYPixmap);
	Long	 value = XGetPixel(image, 0, 0);

	XDestroyImage(image);

	if (depth == 16) return Color(((value >> 11) &  31) << 3, ((value >> 5) &  63) << 2, (value &  31) << 3);
	else		 return Color( (value >> 16) & 255,	   (value >> 8) & 255,	      value & 255      );
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
