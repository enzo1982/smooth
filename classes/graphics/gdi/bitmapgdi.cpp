 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/gdi/bitmapgdi.h>

S::GUI::BitmapGDI	 S::GUI::SI_DEFAULT = S::GUI::BitmapGDI();

S::GUI::BitmapGDI::BitmapGDI(HBITMAP iBitmap)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetBitmap(iBitmap);
}

S::GUI::BitmapGDI::BitmapGDI(Int cx, Int cy, Int bpp)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	CreateBitmap(cx, cy, bpp);
}

S::GUI::BitmapGDI::BitmapGDI(const int nil)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetBitmap(NIL);
}

S::GUI::BitmapGDI::BitmapGDI(const BitmapGDI &iBitmap)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetBitmap(iBitmap.bitmap);
}

S::GUI::BitmapGDI::~BitmapGDI()
{
	DeleteBitmap();
}

S::Bool S::GUI::BitmapGDI::CreateBitmap(Int cx, Int cy, Int bpp)
{
	DeleteBitmap();

	if (bpp != 24 && bpp != 32) bpp = 32;

	UnsignedByte	*buffer = new UnsignedByte [sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)];
	BITMAPINFO	*bmpInfo = (BITMAPINFO *) buffer;

	bmpInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmpInfo->bmiHeader.biWidth		= cx;
	bmpInfo->bmiHeader.biHeight		= cy;
	bmpInfo->bmiHeader.biPlanes		= 1;
	bmpInfo->bmiHeader.biBitCount		= bpp;
	bmpInfo->bmiHeader.biCompression	= BI_RGB;
	bmpInfo->bmiHeader.biSizeImage		= 0; 
	bmpInfo->bmiHeader.biXPelsPerMeter	= 0; 
	bmpInfo->bmiHeader.biYPelsPerMeter	= 0; 
	bmpInfo->bmiHeader.biClrUsed		= 0; 
	bmpInfo->bmiHeader.biClrImportant	= 0; 
	bmpInfo->bmiColors[0].rgbBlue		= 0;
	bmpInfo->bmiColors[0].rgbGreen		= 0;
	bmpInfo->bmiColors[0].rgbRed		= 0;
	bmpInfo->bmiColors[0].rgbReserved	= 0;

	bitmap	= CreateDIBSection(NIL, bmpInfo, DIB_RGB_COLORS, (Void **) &bytes, NIL, 0);

	delete [] buffer; 

	if (bitmap == NIL) return False;

	size	= Size(cx, cy);
	depth	= bpp;

	BITMAP	 bmp;

	GetObjectA(bitmap, sizeof(bmp), &bmp);

	align	= bmp.bmWidthBytes;

	return True;
}

S::Bool S::GUI::BitmapGDI::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		::DeleteObject(bitmap);

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapGDI::SetBitmap(HBITMAP nBitmap)
{
	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		BITMAP	 bmp;

		GetObjectA(nBitmap, sizeof(bmp), &bmp);

		CreateBitmap(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);

		HDC	 ddc = GetWindowDC(NIL);
		HDC	 cdc1 = CreateCompatibleDC(ddc);
		HDC	 cdc2 = CreateCompatibleDC(ddc);
		HBITMAP	 backup1 = (HBITMAP) SelectObject(cdc1, nBitmap);
		HBITMAP	 backup2 = (HBITMAP) SelectObject(cdc2, bitmap);

		BitBlt(cdc2, 0, 0, size.cx, size.cy, cdc1, 0, 0, SRCCOPY);

		nBitmap	= (HBITMAP) SelectObject(cdc1, backup1);
		bitmap	= (HBITMAP) SelectObject(cdc2, backup2);

		DeleteDC(cdc1);
		DeleteDC(cdc2);
		ReleaseDC(NIL, ddc);
	}

	return True;
}

HBITMAP S::GUI::BitmapGDI::GetBitmap()
{
	return bitmap;
}

S::GUI::BitmapGDI &S::GUI::BitmapGDI::operator =(const int nil)
{
	SetBitmap(NIL);

	return *this;
}

S::GUI::BitmapGDI &S::GUI::BitmapGDI::operator =(const BitmapGDI &newBitmap)
{
	SetBitmap(newBitmap.bitmap);

	return *this;
}

S::Bool S::GUI::BitmapGDI::operator ==(const int nil)
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapGDI::operator !=(const int nil)
{
	if (bitmap == NIL)	return False;
	else			return True;
}
