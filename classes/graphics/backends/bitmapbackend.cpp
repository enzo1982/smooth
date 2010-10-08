 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

#if defined __WIN32__ && defined SMOOTH_STATIC
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

S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_pV)(S::Void *)				= &CreateBitmapBackend_pV;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_III)(S::Int, S::Int, S::Int)		= &CreateBitmapBackend_III;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_cI)(const int)				= &CreateBitmapBackend_cI;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_crS)(const S::GUI::BitmapBackend &)	= &CreateBitmapBackend_crS;

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	backend_creator_pV = backend;

	return Success();
}

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(Int, Int, Int))
{
	if (backend == NIL) return Error();

	backend_creator_III = backend;

	return Success();
}

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(const int))
{
	if (backend == NIL) return Error();

	backend_creator_cI = backend;

	return Success();
}

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(const BitmapBackend &))
{
	if (backend == NIL) return Error();

	backend_creator_crS = backend;

	return Success();
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Void *iBitmap)
{
	return backend_creator_pV(iBitmap);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Int cx, Int cy, Int bpp)
{
	return backend_creator_III(cx, cy, bpp);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const int nil)
{
	return backend_creator_cI(nil);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const BitmapBackend &iBitmap)
{
	return backend_creator_crS(iBitmap);
}

S::GUI::BitmapBackend::BitmapBackend(Void *iBitmap)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
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

S::Short S::GUI::BitmapBackend::GetBitmapType() const
{
	return type;
}

const S::GUI::Size &S::GUI::BitmapBackend::GetSize() const
{
	return size;
}

S::Byte S::GUI::BitmapBackend::GetDepth() const
{
	return depth;
}

S::UnsignedByte *S::GUI::BitmapBackend::GetBytes() const
{
	return (UnsignedByte *) bytes;
}

S::Byte S::GUI::BitmapBackend::GetLineAlignment() const
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

S::Int S::GUI::BitmapBackend::Scale(const Size &newSize)
{
	return Error();
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
			offset = (size.cy - ++point.y) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			data[offset + 0] = (color >> 16) & 255;
			data[offset + 1] = (color >> 8) & 255;
			data[offset + 2] = color & 255;

			done = True;

			break;
		case 32:
			offset = (size.cy - ++point.y) * (((4 - ((size.cx * 4) & 3)) & 3) + size.cx * 4) + point.x * 4;

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
			offset = (size.cy - ++point.y) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			color = Color(data[offset + 2], data[offset + 1], data[offset + 0]);

			break;
		case 32:
			offset = (size.cy - ++point.y) * (((4 - ((size.cx * 4) & 3)) & 3) + size.cx * 4) + point.x * 4;

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
