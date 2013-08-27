 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::BitmapBackend *CreateBitmapHaiku_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapHaiku(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapHaiku_cI(const int nil)
{
	return new S::GUI::BitmapHaiku(nil);
}

S::GUI::BitmapBackend *CreateBitmapHaiku_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapHaiku((const S::GUI::BitmapHaiku &) iBitmap);
}

S::Int	 bitmapHaikuTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_pV);
S::Int	 bitmapHaikuTmp_III	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_III);
S::Int	 bitmapHaikuTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_cI);
S::Int	 bitmapHaikuTmp_crS	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapHaiku_crS);

S::GUI::BitmapHaiku::BitmapHaiku(Void *iBitmap)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapHaiku::BitmapHaiku(Int cx, Int cy, Int bpp)
{
	type	= BITMAP_HAIKU;
	bitmap	= NIL;

	CreateBitmap(cx, cy, bpp);
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

S::Bool S::GUI::BitmapHaiku::CreateBitmap(Int cx, Int cy, Int bpp)
{
	DeleteBitmap();

	bpp = 32;

	bitmap	= new BBitmap(BRect(0, 0, cx - 1, cy - 1), B_RGB32, true);

	if (bitmap == NIL) return False;

	bytes	= bitmap->Bits();	

	size	= Size(cx, cy);
	depth	= bpp;

	align	= cx * (bpp / 8);

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
		BRect	 bounds = ((BBitmap *) nBitmap)->Bounds();

		CreateBitmap(bounds.right + 1, bounds.bottom + 1, 32);

		BView	*view = new BView(bounds, NULL, B_FOLLOW_ALL_SIDES, 0);

		bitmap->AddChild(view);
		bitmap->Lock();

		view->DrawBitmap((BBitmap *) nBitmap, BPoint(0, 0));

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

S::Bool S::GUI::BitmapHaiku::SetPixel(const Point &point, const Color &color)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

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

S::GUI::Color S::GUI::BitmapHaiku::GetPixel(const Point &point) const
{
	if (bytes == NIL)			      return 0;
	if (point.y >= size.cy || point.x >= size.cx) return 0;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = point.y * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			return Color(			      data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
		case 32:
			offset = point.y * (				      size.cx * 4) + point.x * 4;

			return Color(data[offset + 3] << 24 | data[offset + 0] << 16 | data[offset + 1] << 8 | data[offset + 2]);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapHaiku::operator =(const BitmapBackend &newBitmap)
{
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
