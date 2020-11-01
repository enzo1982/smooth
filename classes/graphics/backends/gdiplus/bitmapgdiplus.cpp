 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::BitmapBackend *CreateBitmapGDIPlus_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapGDIPlus(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapGDIPlus_cI(const int nil)
{
	return new S::GUI::BitmapGDIPlus(nil);
}

S::GUI::BitmapBackend *CreateBitmapGDIPlus_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapGDIPlus((const S::GUI::BitmapGDIPlus &) iBitmap);
}

S::Int	 bitmapGDIPlusTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_pV);
S::Int	 bitmapGDIPlusTmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_crSI);
S::Int	 bitmapGDIPlusTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_cI);
S::Int	 bitmapGDIPlusTmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapGDIPlus_crB);

S::GUI::BitmapGDIPlus::BitmapGDIPlus(Void *iBitmap)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapGDIPlus::BitmapGDIPlus(const Size &iSize, Int iDepth)
{
	type	= BITMAP_GDIPLUS;
	bitmap	= NIL;
	hBitmap	= NIL;

	CreateBitmap(iSize, iDepth);
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

	if (iBitmap != NIL)
	{
		CreateBitmap(iBitmap.GetSize(), iBitmap.GetDepth());

		memcpy(bytes, iBitmap.GetBytes(), (bpp / 8) * size.cx * size.cy);
	}
}

S::GUI::BitmapGDIPlus::~BitmapGDIPlus()
{
	DeleteBitmap();
}

S::Bool S::GUI::BitmapGDIPlus::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1) nDepth = 32;
	if (nDepth != 32) nDepth = 32;

	bytes	= new UnsignedByte [(nDepth / 8) * nSize.cx * nSize.cy];
	bitmap	= new Gdiplus::Bitmap(nSize.cx, nSize.cy, nSize.cx * (nDepth / 8), PixelFormat32bppARGB, (BYTE *) bytes);

	if (bitmap == NIL) return False;

	size	= nSize;
	depth	= nDepth;
	bpp	= depth;
	align	= 4;

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
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapGDIPlus::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		Gdiplus::Bitmap		*gdipBitmap = (Gdiplus::Bitmap *) nBitmap;

		CreateBitmap(Size(gdipBitmap->GetWidth(), gdipBitmap->GetHeight()), 32);

		Gdiplus::Graphics	 graphics(bitmap);

		graphics.DrawImage(gdipBitmap, 0, 0, gdipBitmap->GetWidth(), gdipBitmap->GetHeight());
	}

	return True;
}

S::Void *S::GUI::BitmapGDIPlus::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapGDIPlus::SetPixel(const Point &point, const Color &iColor)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	Color		 color	= iColor.ConvertTo(Color::RGBA);

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = point.y * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 2] =  color	 & 255;

			return True;
		case 32:
			offset = point.y * (				      size.cx * 4) + point.x * 4;

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
			offset = point.y * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			return Color(			      data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2], Color::RGB);
		case 32:
			offset = point.y * (				      size.cx * 4) + point.x * 4;

			return Color(data[offset + 3] << 24 | data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2], Color::RGBA);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapGDIPlus::operator =(const BitmapBackend &newBitmap)
{
	if (&newBitmap == this) return *this;

	DeleteBitmap();

	if (newBitmap != NIL)
	{
		CreateBitmap(newBitmap.GetSize(), newBitmap.GetDepth());

		memcpy(bytes, newBitmap.GetBytes(), (bpp / 8) * size.cx * size.cy);
	}

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
