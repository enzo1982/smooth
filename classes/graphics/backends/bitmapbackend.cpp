 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::BitmapBackend *CreateBitmapBackend_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapBackend(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapBackend_cI(const int nil)
{
	return new S::GUI::BitmapBackend(nil);
}

S::GUI::BitmapBackend *CreateBitmapBackend_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapBackend(iBitmap);
}

S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_pV)(S::Void *)				= &CreateBitmapBackend_pV;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_crSI)(const S::GUI::Size &, S::Int)	= &CreateBitmapBackend_crSI;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_cI)(const int)				= &CreateBitmapBackend_cI;
S::GUI::BitmapBackend *(*S::GUI::BitmapBackend::backend_creator_crB)(const S::GUI::BitmapBackend &)	= &CreateBitmapBackend_crB;

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	backend_creator_pV = backend;

	return Success();
}

S::Int S::GUI::BitmapBackend::SetBackend(BitmapBackend *(*backend)(const Size &, Int))
{
	if (backend == NIL) return Error();

	backend_creator_crSI = backend;

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

	backend_creator_crB = backend;

	return Success();
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(Void *iBitmap)
{
	return backend_creator_pV(iBitmap);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const Size &iSize, Int iDepth)
{
	return backend_creator_crSI(iSize, iDepth);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const int nil)
{
	return backend_creator_cI(nil);
}

S::GUI::BitmapBackend *S::GUI::BitmapBackend::CreateBackendInstance(const BitmapBackend &iBitmap)
{
	return backend_creator_crB(iBitmap);
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
	bpp	= 0;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const Size &iSize, Int iDepth)
{
	type	= BITMAP_NONE;

	size	= iSize;
	depth	= iDepth;

	bytes	= NIL;
	bpp	= 0;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const int nil)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	bpp	= 0;
	align	= 0;
}

S::GUI::BitmapBackend::BitmapBackend(const BitmapBackend &iBitmap)
{
	type	= BITMAP_NONE;

	size	= Size(0, 0);
	depth	= 0;

	bytes	= NIL;
	bpp	= 0;
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

S::Byte S::GUI::BitmapBackend::GetBitsPerPixel() const
{
	return bpp;
}

S::Byte S::GUI::BitmapBackend::GetLineAlignment() const
{
	return align;
}

S::Bool S::GUI::BitmapBackend::CreateBitmap(const Size &nSize, Int nDepth)
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
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			SetPixel(point, GetPixel(point).Grayscale());
		}
	}

	return Success();
}

S::Int S::GUI::BitmapBackend::InvertColors()
{
	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Color	 pixel = GetPixel(point);

			SetPixel(point, Color(255 - pixel.GetRed(), 255 - pixel.GetGreen(), 255 - pixel.GetBlue()));
		}
	}

	return Success();
}

S::Int S::GUI::BitmapBackend::ReplaceColor(const Color &iColor1, const Color &iColor2)
{
	Color	 color1 = iColor1.ConvertTo(depth == 32 ? Color::RGBA : Color::RGB);
	Color	 color2 = iColor2.ConvertTo(depth == 32 ? Color::RGBA : Color::RGB);

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

S::Int S::GUI::BitmapBackend::SetBackgroundColor(const Color &iColor)
{
	if (depth != 32) return Success();

	Color	 color = iColor.ConvertTo(Color::RGB);

	Point	 point;

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx; point.x++)
		{
			Color	 pixel = GetPixel(point);

			if (pixel.GetAlpha() != 255)
			{
				Color	 result = Color((pixel.GetRed()   * pixel.GetAlpha() + color.GetRed()   * (255 - pixel.GetAlpha())) / 255,
							(pixel.GetGreen() * pixel.GetAlpha() + color.GetGreen() * (255 - pixel.GetAlpha())) / 255,
							(pixel.GetBlue()  * pixel.GetAlpha() + color.GetBlue()  * (255 - pixel.GetAlpha())) / 255);

				SetPixel(point, 255 << 24 | result);
			}
		}
	}

	return Success();
}

S::GUI::Bitmap S::GUI::BitmapBackend::Scale(const Size &newSize) const
{
	Bitmap	 bitmap(newSize, depth);

	Point	 srcPoint;
	Point	 destPoint;

	Float	 scaleFactorX = Float(size.cx) / Float(newSize.cx);
	Float	 scaleFactorY = Float(size.cy) / Float(newSize.cy);

	for (destPoint.y = 0; destPoint.y < newSize.cy; destPoint.y++)
	{
		for (destPoint.x = 0; destPoint.x < newSize.cx; destPoint.x++)
		{
			Float	 red = 0, green = 0, blue = 0, alpha = 0;

			for (srcPoint.x = Int(Float(destPoint.x) * scaleFactorX); srcPoint.x < Int(Float(destPoint.x + 1) * scaleFactorX + 0.9999); srcPoint.x++)
			{
				for (srcPoint.y = Int(Float(destPoint.y) * scaleFactorY); srcPoint.y < Int(Float(destPoint.y + 1) * scaleFactorY + 0.9999); srcPoint.y++)
				{
					Int	 color	 = GetPixel(srcPoint);

					Float	 weightX = (1.0 - Math::Max(0.0, Float(destPoint.x) * scaleFactorX - Float(srcPoint.x)) - Math::Max(0.0, Float(srcPoint.x + 1) - Float(destPoint.x + 1) * scaleFactorX)) / scaleFactorX;
					Float	 weightY = (1.0 - Math::Max(0.0, Float(destPoint.y) * scaleFactorY - Float(srcPoint.y)) - Math::Max(0.0, Float(srcPoint.y + 1) - Float(destPoint.y + 1) * scaleFactorY)) / scaleFactorY;

					red   += Float(	color	     & 255) * weightX * weightY;
					green += Float((color >>  8) & 255) * weightX * weightY;
					blue  += Float((color >> 16) & 255) * weightX * weightY;

					if (depth == 32) alpha += Float((color >> 24) & 255) * weightX * weightY;
				}
			}

			bitmap.SetPixel(destPoint, Color(Int(red) | (Int(green) << 8) | (Int(blue) << 16) | (Int(alpha) << 24), Color::RGBA));
		}
	}

	return bitmap;
}

S::Bool S::GUI::BitmapBackend::SetPixel(const Point &iPoint, const Color &color)
{
	return False;
}

S::GUI::Color S::GUI::BitmapBackend::GetPixel(const Point &iPoint) const
{
	return 0;
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
