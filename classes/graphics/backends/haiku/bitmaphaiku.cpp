 /* The smooth Class Library
  * Copyright (C) 1998-2024 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/haiku/bitmaphaiku.h>

#include <View.h>

S::GUI::BitmapBackend *CreateBitmapHaiku_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapHaiku(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapHaiku_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapHaiku(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapHaiku_cI(const int nil)
{
	return new S::GUI::BitmapHaiku(nil);
}

S::GUI::BitmapBackend *CreateBitmapHaiku_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapHaiku((const S::GUI::BitmapHaiku &) iBitmap);
}

S::Int	 bitmapHaikuTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_pV);
S::Int	 bitmapHaikuTmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_crSI);
S::Int	 bitmapHaikuTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_cI);
S::Int	 bitmapHaikuTmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_crB);

S::GUI::BitmapHaiku::BitmapHaiku(Void *iBitmap)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapHaiku::BitmapHaiku(const Size &iSize, Int iDepth)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	CreateBitmap(iSize, iDepth);
}

S::GUI::BitmapHaiku::BitmapHaiku(const int nil)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	SetSystemBitmap(NIL);
}

S::GUI::BitmapHaiku::BitmapHaiku(const BitmapHaiku &iBitmap)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	SetSystemBitmap((Void *) iBitmap.bitmap);
}

S::GUI::BitmapHaiku::~BitmapHaiku()
{
	DeleteBitmap();
}

S::Bool S::GUI::BitmapHaiku::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth == -1)		  nDepth = 24;
	if (nDepth != 24 && nDepth != 32) nDepth = 24;

	bitmap	= new BBitmap(BRect(0, 0, nSize.cx - 1, nSize.cy - 1), nDepth == 32 ? B_RGBA32 : B_RGB32, true);

	if (bitmap == NIL) return False;

	bytes	= bitmap->Bits();

	size	= nSize;
	depth	= nDepth;
	bpp	= 32;
	align	= 4;

	return True;
}

S::Bool S::GUI::BitmapHaiku::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		delete bitmap;

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapHaiku::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		BBitmap	*haikuBitmap = (BBitmap *) nBitmap;
		BRect	 bounds	     = haikuBitmap->Bounds();

		CreateBitmap(Size(bounds.right + 1, bounds.bottom + 1), haikuBitmap->ColorSpace() == B_RGBA32 ? 32 : 24);

		BView	*view = new BView(bounds, NULL, B_FOLLOW_ALL_SIDES, 0);

		bitmap->AddChild(view);
		bitmap->Lock();

		view->DrawBitmap(haikuBitmap, BPoint(0, 0));

		bitmap->Unlock();
		bitmap->RemoveChild(view);

		delete view;
	}

	return True;
}

S::Void *S::GUI::BitmapHaiku::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapHaiku::SetPixel(const Point &point, const Color &iColor)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	Color		 color	= iColor.ConvertTo(Color::RGBA);

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = point.y * (size.cx * 4) + point.x * 4;

	switch (depth)
	{
		case 24:
			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 2] =  color	 & 255;

			return True;
		case 32:
			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 2] =  color	 & 255;
			data[offset + 3] = (color >> 24) & 255;

			return True;
	}

	return False;
}

S::GUI::Color S::GUI::BitmapHaiku::GetPixel(const Point &point) const
{
	if (bytes == NIL)			      return 0;
	if (point.y >= size.cy || point.x >= size.cx) return 0;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = point.y * (size.cx * 4) + point.x * 4;

	switch (depth)
	{
		case 24:
			return Color(			      data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2], Color::RGB);
		case 32:
			return Color(data[offset + 3] << 24 | data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2], Color::RGBA);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapHaiku::operator =(const BitmapBackend &newBitmap)
{
	if (&newBitmap == this) return *this;

	SetSystemBitmap((Void *) ((BitmapHaiku &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapHaiku::operator ==(const int nil) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapHaiku::operator !=(const int nil) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
