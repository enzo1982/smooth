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

S::GUI::BitmapGDI::BitmapGDI(HBITMAP iBitmap)
{
	type = BITMAP_GDI;

	SetBitmap(iBitmap);
}

S::GUI::BitmapGDI::BitmapGDI(const int nil)
{
	type = BITMAP_GDI;

	SetBitmap(NIL);
}

S::GUI::BitmapGDI::BitmapGDI(const BitmapGDI &iBitmap)
{
	type = BITMAP_GDI;

	SetBitmap(iBitmap.bitmap);
}

S::GUI::BitmapGDI::~BitmapGDI()
{
	SetBitmap(NIL);
}

S::Bool S::GUI::BitmapGDI::SetBitmap(HBITMAP nBitmap)
{
	if (nBitmap == NIL)
	{
		bitmap		= NIL;

		size		= Size(0, 0);
		depth		= 0;

		bytes		= NIL;
		align		= 0;
	}
	else
	{
		BITMAP	 bmp;

		ZeroMemory(&bmp, sizeof(bmp));
		GetObjectA(nBitmap, sizeof(bmp), &bmp);

		bitmap		= nBitmap;

		size		= Size(bmp.bmWidth, bmp.bmHeight);
		depth		= bmp.bmBitsPixel;

		bytes		= (UnsignedByte *) bmp.bmBits;
		align		= bmp.bmWidthBytes;
	}

	return True;
}

HBITMAP S::GUI::BitmapGDI::GetBitmap()
{
	return bitmap;
}

S::GUI::BitmapBase &S::GUI::BitmapGDI::operator =(const int nil)
{
	SetBitmap(NIL);

	return *this;
}

S::GUI::BitmapBase &S::GUI::BitmapGDI::operator =(const BitmapBase &newBitmap)
{
	SetBitmap(((BitmapGDI *) &newBitmap)->bitmap);

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
