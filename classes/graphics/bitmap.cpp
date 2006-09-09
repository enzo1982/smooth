 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/backends/bitmapbackend.h>
#include <smooth/graphics/surface.h>

S::GUI::Bitmap::Bitmap(Void *iBitmap)
{
	backend = BitmapBackend::CreateBackendInstance(iBitmap);
}

S::GUI::Bitmap::Bitmap(Int cx, Int cy, Int bpp)
{
	backend = BitmapBackend::CreateBackendInstance(cx, cy, bpp);
}

S::GUI::Bitmap::Bitmap(const int nil)
{
	backend = BitmapBackend::CreateBackendInstance(nil);
}

S::GUI::Bitmap::Bitmap(const Bitmap &iBitmap)
{
	backend = BitmapBackend::CreateBackendInstance(*(iBitmap.backend));
}

S::GUI::Bitmap::~Bitmap()
{
	delete backend;
}

S::Int S::GUI::Bitmap::GetBitmapType() const
{
	return backend->GetBitmapType();
}

const S::GUI::Size &S::GUI::Bitmap::GetSize() const
{
	return backend->GetSize();
}

S::Int S::GUI::Bitmap::GetDepth() const
{
	return backend->GetDepth();
}

S::UnsignedByte *S::GUI::Bitmap::GetBytes() const
{
	return backend->GetBytes();
}

S::Int S::GUI::Bitmap::GetLineAlignment() const
{
	return backend->GetLineAlignment();
}

S::Bool S::GUI::Bitmap::CreateBitmap(Int cx, Int cy, Int bpp)
{
	return backend->CreateBitmap(cx, cy, bpp);
}

S::Bool S::GUI::Bitmap::DeleteBitmap()
{
	return backend->DeleteBitmap();
}

S::Bool S::GUI::Bitmap::SetSystemBitmap(Void *nBitmap)
{
	return backend->SetSystemBitmap(nBitmap);
}

S::Void *S::GUI::Bitmap::GetSystemBitmap() const
{
	return backend->GetSystemBitmap();
}

S::Int S::GUI::Bitmap::GrayscaleBitmap()
{
	return backend->GrayscaleBitmap();
}

S::Int S::GUI::Bitmap::InvertColors()
{
	return backend->InvertColors();
}

S::Int S::GUI::Bitmap::ReplaceColor(Int color1, Int color2)
{
	return backend->ReplaceColor(color1, color2);
}

S::Int S::GUI::Bitmap::BlitFromSurface(Surface *surface, const Rect &srcRect, const Rect &destRect)
{
	if (surface == NIL) return Error();

	return surface->BlitToBitmap(srcRect, *this, destRect);
}

S::Int S::GUI::Bitmap::BlitToSurface(const Rect &srcRect, Surface *surface, const Rect &destRect)
{
	if (surface == NIL) return Error();

	return surface->BlitFromBitmap(*this, srcRect, destRect);
}

S::Bool S::GUI::Bitmap::SetPixel(Int x, Int y, UnsignedLong color)
{
	return backend->SetPixel(x, y, color);
}

S::UnsignedLong S::GUI::Bitmap::GetPixel(Int x, Int y) const
{
	return backend->GetPixel(x, y);
}

S::GUI::Bitmap &S::GUI::Bitmap::operator =(const int nil)
{
	*backend = nil;

	return *this;
}

S::GUI::Bitmap &S::GUI::Bitmap::operator =(const Bitmap &newBitmap)
{
	*backend = *(newBitmap.backend);

	return *this;
}

S::Bool S::GUI::Bitmap::operator ==(const int nil) const
{
	return (*backend == nil);
}

S::Bool S::GUI::Bitmap::operator !=(const int nil) const
{
	return (*backend != nil);
}
