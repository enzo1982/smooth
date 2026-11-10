 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/wayland/bitmapwayland.h>

#include <string.h>

S::GUI::BitmapBackend *CreateBitmapWayland_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapWayland(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapWayland_crSI(const S::GUI::Size &iSize, S::Int iDepth)
{
	return new S::GUI::BitmapWayland(iSize, iDepth);
}

S::GUI::BitmapBackend *CreateBitmapWayland_cI(const int nil)
{
	return new S::GUI::BitmapWayland(nil);
}

S::GUI::BitmapBackend *CreateBitmapWayland_crB(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapWayland((const S::GUI::BitmapWayland &) iBitmap);
}

S::Int	 bitmapWaylandTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapWayland_pV);
S::Int	 bitmapWaylandTmp_crSI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapWayland_crSI);
S::Int	 bitmapWaylandTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapWayland_cI);
S::Int	 bitmapWaylandTmp_crB	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapWayland_crB);

S::GUI::BitmapWayland::BitmapWayland(Void *iBitmap)
{
	Initialize();

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapWayland::BitmapWayland(const Size &iSize, Int iDepth)
{
	Initialize();

	CreateBitmap(iSize, iDepth);
}

S::GUI::BitmapWayland::BitmapWayland(const int nil)
{
	Initialize();

	SetSystemBitmap(NIL);
}

S::GUI::BitmapWayland::BitmapWayland(const BitmapWayland &iBitmap)
{
	Initialize();

	SetSystemBitmap((Void *) iBitmap.bitmap);
}

S::GUI::BitmapWayland::~BitmapWayland()
{
	DeleteBitmap();
}

S::Void S::GUI::BitmapWayland::Initialize()
{
	type	= BITMAP_WAYLAND;
	bitmap	= NIL;
}

S::Bool S::GUI::BitmapWayland::CreateBitmap(const Size &nSize, Int nDepth)
{
	DeleteBitmap();

	if (nDepth != 32) nDepth = 24;

	if (nSize.cx <= 0 || nSize.cy <= 0) return False;

	/* Both formats use 32 bits per pixel with the
	 * same layout as 24 and 32 bit XImages.
	 */
	bitmap = cairo_image_surface_create(nDepth == 32 ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24, nSize.cx, nSize.cy);

	if (cairo_surface_status(bitmap) != CAIRO_STATUS_SUCCESS)
	{
		cairo_surface_destroy(bitmap);

		bitmap = NIL;

		return False;
	}

	size	= nSize;
	depth	= nDepth;
	bytes	= cairo_image_surface_get_data(bitmap);
	bpp	= 32;
	align	= 4;

	return True;
}

S::Bool S::GUI::BitmapWayland::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		cairo_surface_destroy(bitmap);

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		bpp	= 0;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapWayland::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		cairo_surface_t	*image	= (cairo_surface_t *) nBitmap;
		Size		 nSize	= Size(cairo_image_surface_get_width(image), cairo_image_surface_get_height(image));
		Int		 nDepth = cairo_image_surface_get_format(image) == CAIRO_FORMAT_ARGB32 ? 32 : 24;

		if (!CreateBitmap(nSize, nDepth)) return False;

		cairo_surface_flush(image);

		Int	 srcStride = cairo_image_surface_get_stride(image);
		Int	 dstStride = cairo_image_surface_get_stride(bitmap);

		for (Int y = 0; y < nSize.cy; y++) memcpy((UnsignedByte *) bytes + y * dstStride, cairo_image_surface_get_data(image) + y * srcStride, nSize.cx * 4);

		cairo_surface_mark_dirty(bitmap);
	}

	return True;
}

S::Void *S::GUI::BitmapWayland::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapWayland::SetPixel(const Point &iPoint, const Color &iColor)
{
	if (bitmap == NIL) return False;

	if (iPoint.x < 0 || iPoint.y < 0 || iPoint.x >= size.cx || iPoint.y >= size.cy) return False;

	Color		 color = iColor.ConvertTo(Color::RGBA);
	UnsignedInt	*pixel = (UnsignedInt *) ((UnsignedByte *) bytes + iPoint.y * size.cx * 4 + iPoint.x * 4);

	if (depth == 32) *pixel = (color.GetAlpha() << 24) | (color.GetRed() << 16) | (color.GetGreen() << 8) | color.GetBlue();
	else		 *pixel = (			  color.GetRed() << 16) | (color.GetGreen() << 8) | color.GetBlue();

	return True;
}

S::GUI::Color S::GUI::BitmapWayland::GetPixel(const Point &iPoint) const
{
	if (bitmap == NIL) return Color();

	if (iPoint.x < 0 || iPoint.y < 0 || iPoint.x >= size.cx || iPoint.y >= size.cy) return Color();

	UnsignedInt	 value = *(UnsignedInt *) ((UnsignedByte *) bytes + iPoint.y * size.cx * 4 + iPoint.x * 4);

	if (depth == 32) return ((value >> 24) & 255) << 24 | Color((value >> 16) & 255, (value >> 8) & 255, value & 255, Color::RGBA);
	else		 return				      Color((value >> 16) & 255, (value >> 8) & 255, value & 255, Color::RGB);
}

S::GUI::BitmapBackend &S::GUI::BitmapWayland::operator =(const BitmapBackend &newBitmap)
{
	if (&newBitmap == this) return *this;

	SetSystemBitmap((Void *) ((BitmapWayland &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapWayland::operator ==(const int nil) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapWayland::operator !=(const int nil) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}
