 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/gdiplus/bitmapgdiplus.h>

S::GUI::BitmapBackend *CreateBitmapGDIPlus_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapGDIPlus(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapGDIPlus_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapGDIPlus(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapGDIPlus_cI(const int nil)
{
	return new S::GUI::BitmapGDIPlus(nil);
}

S::GUI::BitmapBackend *CreateBitmapGDIPlus_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapGDIPlus((const S::GUI::BitmapGDIPlus &) iBitmap);
}

S::Int	 bitmapGDITmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_pV);
S::Int	 bitmapGDITmp_III	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_III);
S::Int	 bitmapGDITmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_cI);
S::Int	 bitmapGDITmp_crS	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_crS);

S::GUI::BitmapGDIPlus::BitmapGDIPlus(Void *iBitmap)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapGDIPlus::BitmapGDIPlus(Int cx, Int cy, Int bpp)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	CreateBitmap(cx, cy, bpp);
}

S::GUI::BitmapGDIPlus::BitmapGDIPlus(const int nil)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	SetSystemBitmap(NIL);
}

S::GUI::BitmapGDIPlus::BitmapGDIPlus(const BitmapGDIPlus &iBitmap)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	SetSystemBitmap(iBitmap.GetSystemBitmap());
}

S::GUI::BitmapGDIPlus::~BitmapGDIPlus()
{
	DeleteBitmap();
}

S::Bool S::GUI::BitmapGDIPlus::CreateBitmap(Int cx, Int cy, Int bpp)
{
	DeleteBitmap();

	if (bpp == -1) bpp = 32;
	if (bpp != 32) bpp = 32;

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

	bytes = new UnsignedByte [(bpp / 8) * cx * cy];

	bitmap	= new Gdiplus::Bitmap(bmpInfo, bytes);

	delete [] buffer;

	if (bitmap == NIL) return False;

	size	= Size(cx, cy);
	depth	= bpp;
	align	= (bpp / 8) * cx;

	return True;
}

S::Bool S::GUI::BitmapGDIPlus::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		delete bitmap;

		if (hBitmap != NIL) ::DeleteObject(hBitmap);

		bitmap	= NIL;
		hBitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		delete [] (UnsignedByte *) bytes;

		bytes	= NIL;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapGDIPlus::SetSystemBitmap(Void *nBitmap)
{
	if (hBitmap != NIL && nBitmap == hBitmap) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		BITMAP	 bmp;

		GetObjectA(nBitmap, sizeof(bmp), &bmp);

		CreateBitmap(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);

		Gdiplus::Graphics	 graphics(bitmap);
		Gdiplus::Bitmap		 nBitmapB((HBITMAP) nBitmap, NIL);

		graphics.DrawImage(&nBitmapB, 0, 0, bmp.bmWidth, bmp.bmHeight);
	}

	return True;
}

S::Void *S::GUI::BitmapGDIPlus::GetSystemBitmap() const
{
	if (bitmap == NIL) return NIL;

	if (hBitmap != NIL) ::DeleteObject(hBitmap);

	bitmap->GetHBITMAP(Gdiplus::Color(255, 255, 255), &hBitmap);

	return (Void *) hBitmap;
}

S::Bool S::GUI::BitmapGDIPlus::SetPixel(const Point &point, const Color &color)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - point.y - 1) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 2] =  color	 & 255;

			return True;
		case 32:
			offset = (size.cy - point.y - 1) * (				      size.cx * 4) + point.x * 4;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 2] =  color	 & 255;
			data[offset + 3] = (color >> 24) & 255;

			return True;
	}

	return False;
}

S::GUI::Color S::GUI::BitmapGDIPlus::GetPixel(const Point &point) const
{
	if (bytes == NIL)			      return 0;
	if (point.y >= size.cy || point.x >= size.cx) return 0;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - point.y - 1) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			return Color(			      data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
		case 32:
			offset = (size.cy - point.y - 1) * (				      size.cx * 4) + point.x * 4;

			return Color(data[offset + 3] << 24 | data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapGDIPlus::operator =(const BitmapBackend &newBitmap)
{
	SetSystemBitmap(((BitmapGDIPlus &) newBitmap).GetSystemBitmap());

	return *this;
}

S::Bool S::GUI::BitmapGDIPlus::operator ==(const int nil) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapGDIPlus::operator !=(const int nil) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
