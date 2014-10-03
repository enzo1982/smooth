 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/gdi/bitmapgdi.h>

S::GUI::BitmapBackend *CreateBitmapGDI_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapGDI(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapGDI_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapGDI(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapGDI_cI(const int nil)
{
	return new S::GUI::BitmapGDI(nil);
}

S::GUI::BitmapBackend *CreateBitmapGDI_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapGDI((const S::GUI::BitmapGDI &) iBitmap);
}

S::Int	 bitmapGDITmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDI_pV);
S::Int	 bitmapGDITmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDI_crSI);
S::Int	 bitmapGDITmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDI_cI);
S::Int	 bitmapGDITmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDI_crB);

S::GUI::BitmapGDI::BitmapGDI(Void *iBitmap)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapGDI::BitmapGDI(const Size &iSize, Int iDepth)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	CreateBitmap(iSize, iDepth);
}

S::GUI::BitmapGDI::BitmapGDI(const int nil)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetSystemBitmap(NIL);
}

S::GUI::BitmapGDI::BitmapGDI(const BitmapGDI &iBitmap)
{
	type	= BITMAP_GDI;
	bitmap	= NIL;

	SetSystemBitmap(iBitmap.bitmap);
}

S::GUI::BitmapGDI::~BitmapGDI()
{
	DeleteBitmap();
}

S::Bool S::GUI::BitmapGDI::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1)		  nDepth = 24;
	if (nDepth != 24 && nDepth != 32) nDepth = 24;

	UnsignedByte	*buffer = new UnsignedByte [sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)];
	BITMAPINFO	*bmpInfo = (BITMAPINFO *) buffer;

	bmpInfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmpInfo->bmiHeader.biWidth		= nSize.cx;
	bmpInfo->bmiHeader.biHeight		= nSize.cy;
	bmpInfo->bmiHeader.biPlanes		= 1;
	bmpInfo->bmiHeader.biBitCount		= nDepth;
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

	size	= nSize;
	depth	= nDepth;
	bpp	= depth;
	align	= 4;

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
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapGDI::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		BITMAP	 bmp;

		GetObject(nBitmap, sizeof(bmp), &bmp);

		CreateBitmap(Size(bmp.bmWidth, bmp.bmHeight), bmp.bmBitsPixel);

		HDC	 dc1 = CreateCompatibleDC(NIL);
		HDC	 dc2 = CreateCompatibleDC(NIL);
		HBITMAP	 backup1 = (HBITMAP) SelectObject(dc1, nBitmap);
		HBITMAP	 backup2 = (HBITMAP) SelectObject(dc2, bitmap);

		BitBlt(dc2, 0, 0, size.cx, size.cy, dc1, 0, 0, SRCCOPY);

			  (HBITMAP) SelectObject(dc1, backup1);
		bitmap	= (HBITMAP) SelectObject(dc2, backup2);

		DeleteDC(dc1);
		DeleteDC(dc2);
	}

	return True;
}

S::Void *S::GUI::BitmapGDI::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapGDI::SetPixel(const Point &point, const Color &iColor)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	Color		 color	= iColor.ConvertTo(Color::RGBA);

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

S::GUI::Color S::GUI::BitmapGDI::GetPixel(const Point &point) const
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

S::GUI::BitmapBackend &S::GUI::BitmapGDI::operator =(const BitmapBackend &newBitmap)
{
	if (&newBitmap == this) return *this;

	SetSystemBitmap(((BitmapGDI &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapGDI::operator ==(const int nil) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapGDI::operator !=(const int nil) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
