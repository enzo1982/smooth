 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/direct2d/bitmapdirect2d.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/backends/platform.h>

S::GUI::BitmapBackend *CreateBitmapDirect2D_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapDirect2D(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapDirect2D_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapDirect2D(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapDirect2D_cI(const int nil)
{
	return new S::GUI::BitmapDirect2D(nil);
}

S::GUI::BitmapBackend *CreateBitmapDirect2D_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapDirect2D((const S::GUI::BitmapDirect2D &) iBitmap);
}

static S::Int Register_bitmapDirect2DTmp_pV() { return S::GUI::BitmapBackend::SetBackend(&CreateBitmapDirect2D_pV); }
static S::Int Register_bitmapDirect2DTmp_crSI() { return S::GUI::BitmapBackend::SetBackend(&CreateBitmapDirect2D_crSI); }
static S::Int Register_bitmapDirect2DTmp_cI() { return S::GUI::BitmapBackend::SetBackend(&CreateBitmapDirect2D_cI); }
static S::Int Register_bitmapDirect2DTmp_crB() { return S::GUI::BitmapBackend::SetBackend(&CreateBitmapDirect2D_crB); }

S::Int	 bitmapDirect2DTmp_pV	= S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_bitmapDirect2DTmp_pV);
S::Int	 bitmapDirect2DTmp_crSI	= S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_bitmapDirect2DTmp_crSI);
S::Int	 bitmapDirect2DTmp_cI	= S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_bitmapDirect2DTmp_cI);
S::Int	 bitmapDirect2DTmp_crB	= S::Backends::Platform::AddComponent(S::Backends::PLATFORM_DIRECT2D, &Register_bitmapDirect2DTmp_crB);

S::Void S::GUI::BitmapDirect2D::DeviceBitmap::Release()
{
	if (bitmap != NIL) bitmap->Release();
	if (device != NIL) device->Release();

	bitmap = NIL;
	device = NIL;
}

S::GUI::BitmapDirect2D::BitmapDirect2D(Void *iBitmap) : BitmapGDI(iBitmap)
{
	type	     = BITMAP_DIRECT2D;
	modification = 0;
}

S::GUI::BitmapDirect2D::BitmapDirect2D(const Size &iSize, Int iDepth) : BitmapGDI(iSize, iDepth)
{
	type	     = BITMAP_DIRECT2D;
	modification = 0;
}

S::GUI::BitmapDirect2D::BitmapDirect2D(const int nil) : BitmapGDI(nil)
{
	type	     = BITMAP_DIRECT2D;
	modification = 0;
}

S::GUI::BitmapDirect2D::BitmapDirect2D(const BitmapDirect2D &iBitmap) : BitmapGDI(iBitmap)
{
	type	     = BITMAP_DIRECT2D;
	modification = 0;
}

S::GUI::BitmapDirect2D::~BitmapDirect2D()
{
	unscaled.Release();
	scaled.Release();
}

S::Bool S::GUI::BitmapDirect2D::CreateBitmap(const Size &nSize, Int nDepth)
{
	Modified();

	return BitmapGDI::CreateBitmap(nSize, nDepth);
}

S::Void *S::GUI::BitmapDirect2D::GetSystemBitmap() const
{
	/* The system bitmap may be drawn to using GDI.
	 */
	Modified();

	return BitmapGDI::GetSystemBitmap();
}

S::UnsignedByte *S::GUI::BitmapDirect2D::GetBytes() const
{
	/* The pixel data may be written to.
	 */
	Modified();

	return BitmapGDI::GetBytes();
}

S::Bool S::GUI::BitmapDirect2D::SetPixel(const Point &point, const Color &color)
{
	Modified();

	return BitmapGDI::SetPixel(point, color);
}

ID2D1Bitmap *S::GUI::BitmapDirect2D::CreateDeviceBitmap(ID2D1DeviceContext *context, const UnsignedByte *data, const Size &size, Int depth)
{
	if (data == NIL || size.cx <= 0 || size.cy <= 0) return NIL;

	/* Convert the bottom-up DIB to top-down premultiplied 32 bit BGRA.
	 */
	Int		 bytesPerPixel = depth / 8;
	Int		 stride	       = (size.cx * bytesPerPixel + 3) & ~3;
	UnsignedInt32	*pixels	       = new UnsignedInt32 [size.cx * size.cy];

	for (Int y = 0; y < size.cy; y++)
	{
		const UnsignedByte	*row  = data + (size.cy - y - 1) * stride;
		UnsignedInt32		*line = pixels + y * size.cx;

		for (Int x = 0; x < size.cx; x++)
		{
			const UnsignedByte	*pixel = row + x * bytesPerPixel;
			UnsignedInt32		 a     = (bytesPerPixel == 4 ? pixel[3] : 255);

			if (a == 255) line[x] = 0xFF000000 | pixel[2] << 16 | pixel[1] << 8 | pixel[0];
			else	      line[x] = a << 24 | pixel[2] * a / 255 << 16 | pixel[1] * a / 255 << 8 | pixel[0] * a / 255;
		}
	}

	ID2D1Bitmap	*bitmap = NIL;

	context->CreateBitmap(D2D1::SizeU(size.cx, size.cy), pixels, size.cx * 4, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 96, 96), &bitmap);

	delete [] pixels;

	return bitmap;
}

ID2D1Bitmap *S::GUI::BitmapDirect2D::GetDeviceBitmap(ID2D1DeviceContext *context, const Size &targetSize) const
{
	if (context == NIL || bytes == NIL) return NIL;

	ID2D1Device	*device = NIL;

	context->GetDevice(&device);

	/* Reuse the cached bitmap if it is still valid.
	 */
	Bool		 scale = (targetSize != size);
	DeviceBitmap	&entry = scale ? scaled : unscaled;

	if (entry.bitmap != NIL && entry.device == device && entry.modification == modification && entry.size == targetSize)
	{
		device->Release();

		return entry.bitmap;
	}

	/* Otherwise create it anew.
	 */
	entry.Release();

	if (!scale)
	{
		entry.bitmap = CreateDeviceBitmap(context, (const UnsignedByte *) bytes, size, depth);
	}
	else
	{
		Bitmap	 scaledBitmap = Scale(targetSize);

		entry.bitmap = CreateDeviceBitmap(context, scaledBitmap.GetBytes(), scaledBitmap.GetSize(), scaledBitmap.GetDepth());
	}

	entry.device	   = device;
	entry.size	   = targetSize;
	entry.modification = modification;

	return entry.bitmap;
}
