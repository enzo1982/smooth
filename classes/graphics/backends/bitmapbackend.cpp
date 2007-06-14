 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/bitmapbackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/color.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/graphics/backends/gdi/bitmapgdi.h>
#endif

S::GUI::BitmapBackend *CreateBitmapBackend_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapBackend(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapBackend_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapBackend(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapBackend_cI(const int nil)
{
	return new S::GUI::BitmapBackend(nil);
}

S::GUI::BitmapBackend *CreateBitmapBackend_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapBackend(iBitmap);
}

S::Int	 bitmapBackendTmp_pV	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_pV);
S::Int	 bitmapBackendTmp_III	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_III);
S::Int	 bitmapBackendTmp_cI	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_cI);
S::Int	 bitmapBackendTmp_crS	= S::GUI::BitmapBackend::AddBackend(&CreateBitmapBackend_crS);

S::Array<S::GUI::BitmapBackend *(*)(S::Void *), S::Void *>			*S::GUI::BitmapBackend::backend_creators_pV	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(S::Int, S::Int, S::Int), S::Void *>		*S::GUI::BitmapBackend::backend_creators_III	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(const int), S::Void *>			*S::GUI::BitmapBackend::backend_creators_cI	= NIL;
S::Array<S::GUI::BitmapBackend *(*)(const S::GUI::BitmapBackend &), S::Void *>	*S::GUI::BitmapBackend::backend_creators_crS	= NIL;

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	if (backend_creators_pV == NIL) backend_creators_pV = new Array<BitmapBackend *(*)(Void *), Void *>;

	backend_creators_pV->Add(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(Int, Int, Int))
{
	if (backend == NIL) return Error();

	if (backend_creators_III == NIL) backend_creators_III = new Array<BitmapBackend *(*)(Int, Int, Int), Void *>;

	backend_creators_III->Add(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(const int))
{
	if (backend == NIL) return Error();

	if (backend_creators_cI == NIL) backend_creators_cI = new Array<BitmapBackend *(*)(const int), Void *>;

	backend_creators_cI->Add(backend);

	return Success();
}

S::Int S::GUI::BitmapBackend::AddBackend(BitmapBackend *(*backend)(const BitmapBackend &))
{
	if (backend == NIL) return Error();

	if (backend_creators_crS == NIL) backend_creators_crS = new Array<BitmapBackend *(*)(const BitmapBackend &), Void *>;

	backend_creators_crS->Add(backend);

	return Success();
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Void *iBitmap)
{
	if (backend_creators_pV->GetFirst() != &CreateBitmapBackend_pV)	return backend_creators_pV->GetFirst()(iBitmap);
	else								return backend_creators_pV->GetLast()(iBitmap);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Int cx, Int cy, Int bpp)
{
	if (backend_creators_III->GetFirst() != &CreateBitmapBackend_III)	return backend_creators_III->GetFirst()(cx, cy, bpp);
	else									return backend_creators_III->GetLast()(cx, cy, bpp);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const int nil)
{
	if (backend_creators_cI->GetFirst() != &CreateBitmapBackend_cI)	return backend_creators_cI->GetFirst()(nil);
	else								return backend_creators_cI->GetLast()(nil);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const BitmapBackend &iBitmap)
{
	if (backend_creators_crS->GetFirst() != &CreateBitmapBackend_crS)	return backend_creators_crS->GetFirst()(iBitmap);
	else									return backend_creators_crS->GetLast()(iBitmap);
}

S::GUI::BitmapBackend::BitmapBackend(Void *iBitmap)
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) BitmapGDI();
#endif

	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(Int cx, Int cy, Int bpp)
{
	type	= BITMAP_NONE;

	size	= Size(cx, cy);
	depth	= bpp;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const int nil)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const BitmapBackend &iBitmap)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	align	= 0;
}

S::GUI::BitmapBackend::~BitmapBackend()
{
}

S::Int S::GUI::BitmapBackend::GetBitmapType() const
{
	return type;
}

const S::GUI::Size &S::GUI::BitmapBackend::GetSize() const
{
	return size;
}

S::Int S::GUI::BitmapBackend::GetDepth() const
{
	return depth;
}

S::UnsignedByte *S::GUI::BitmapBackend::GetBytes() const
{
	return (UnsignedByte *) bytes;
}

S::Int S::GUI::BitmapBackend::GetLineAlignment() const
{
	return align;
}

S::Bool S::GUI::BitmapBackend::CreateBitmap(Int cx, Int cy, Int bpp)
{
	return False;
}

S::Bool S::GUI::BitmapBackend::DeleteBitmap()
{
	return True;
}

S::Bool S::GUI::BitmapBackend::SetSystemBitmap(Void *nBitmap)
{
	return False;
}

S::Void *S::GUI::BitmapBackend::GetSystemBitmap() const
{
	return NIL;
}

S::Int S::GUI::BitmapBackend::GrayscaleBitmap()
{
	if (bytes == NIL) return Error();

	Point	 point;
	Color	 color;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			color = GetPixel(point);
			color = (color.GetRed() + color.GetGreen() + color.GetBlue()) / 3;
			color = Color(color, color, color);

			SetPixel(point, color);
		}
	}

	return Success();
}

S::Int S::GUI::BitmapBackend::InvertColors()
{
	if (bytes == NIL) return Error();

	Point	 point;
	Color	 color;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			color = GetPixel(point);
			color = Color(255 - color.GetRed(), 255 - color.GetGreen(), 255 - color.GetBlue());

			SetPixel(point, color);
		}
	}

	return Success();
}

S::Int S::GUI::BitmapBackend::ReplaceColor(const Color &color1, const Color &color2)
{
	if (bytes == NIL) return Error();

	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			if (GetPixel(point) == color1) SetPixel(point, color2);
		}
	}

	return Success();
}

S::Bool S::GUI::BitmapBackend::SetPixel(const Point &iPoint, const Color &color)
{
	if (bytes == NIL)				return False;
	if (iPoint.y >= size.cy || iPoint.x >= size.cx)	return False;

	UnsignedByte	*data = ((UnsignedByte *) bytes);
	Point		 point = iPoint;
	Bool		 done = False;
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++point.y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + point.x * 3;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >> 8) & 255;
			data[offset + 2] = color & 255;

			done = True;

			break;
		case 32:
			offset = (size.cy - ++point.y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + point.x * 4;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >> 8) & 255;
			data[offset + 2] = color & 255;

			done = True;

			break;
	}

	return done;
}

S::GUI::Color S::GUI::BitmapBackend::GetPixel(const Point &iPoint) const
{
	if (bytes == NIL)				return 0;
	if (iPoint.y >= size.cy || iPoint.x >= size.cx)	return 0;

	UnsignedByte	*data = ((UnsignedByte *) bytes);
	Point		 point = iPoint;
	Color		 color = 0;
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++point.y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + point.x * 3;

			color = Color(data[offset + 2], data[offset + 1], data[offset + 0]);

			break;
		case 32:
			offset = (size.cy - ++point.y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + point.x * 4;

			color = Color(data[offset + 2], data[offset + 1], data[offset + 0]);

			break;
	}

	return color;
}

S::GUI::BitmapBackend &S::GUI::BitmapBackend::operator =(const int nil)
{
	SetSystemBitmap(NIL);

	return *this;
}

S::GUI::BitmapBackend &S::GUI::BitmapBackend::operator =(const BitmapBackend &newBitmap)
{
	return *this;
}

S::Bool S::GUI::BitmapBackend::operator ==(const int nil) const
{
	return True;
}

S::Bool S::GUI::BitmapBackend::operator !=(const int nil) const
{
	return False;
}
