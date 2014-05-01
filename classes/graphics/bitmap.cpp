 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::Bitmap::Bitmap(const Size &iSize, Int iDepth)
{
	backend = BitmapBackend::CreateBackendInstance(iSize, iDepth);
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

S::Short S::GUI::Bitmap::GetBitmapType() const
{
	return backend->GetBitmapType();
}

const S::GUI::Size &S::GUI::Bitmap::GetSize() const
{
	return backend->GetSize();
}

S::Byte S::GUI::Bitmap::GetDepth() const
{
	return backend->GetDepth();
}

S::UnsignedByte *S::GUI::Bitmap::GetBytes() const
{
	return backend->GetBytes();
}

S::Byte S::GUI::Bitmap::GetBitsPerPixel() const
{
	return backend->GetBitsPerPixel();
}

S::Byte S::GUI::Bitmap::GetLineAlignment() const
{
	return backend->GetLineAlignment();
}

S::Bool S::GUI::Bitmap::CreateBitmap(const Size &nSize, Int nDepth)
{
	return backend->CreateBitmap(nSize, nDepth);
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

S::Int S::GUI::Bitmap::ReplaceColor(const Color &color1, const Color &color2)
{
	return backend->ReplaceColor(color1, color2);
}

S::Int S::GUI::Bitmap::SetBackgroundColor(const Color &color)
{
	return backend->SetBackgroundColor(color);
}

S::GUI::Bitmap S::GUI::Bitmap::Scale(const Size &newSize) const
{
	if (GetSize() == Size() || GetSize() == newSize) return *this;

	return backend->Scale(newSize);
}

S::Int S::GUI::Bitmap::BlitFromSurface(Surface *surface, const Rect &srcRect, const Rect &destRect)
{
	if (surface == NIL) return Error();

	return surface->BlitToBitmap(srcRect, *this, destRect);
}

S::Int S::GUI::Bitmap::BlitToSurface(const Rect &srcRect, Surface *surface, const Rect &destRect) const
{
	if (surface == NIL) return Error();

	return surface->BlitFromBitmap(*this, srcRect, destRect);
}

S::Bool S::GUI::Bitmap::SetPixel(const Point &point, const Color &color)
{
	return backend->SetPixel(point, color);
}

S::GUI::Color S::GUI::Bitmap::GetPixel(const Point &point) const
{
	return backend->GetPixel(point);
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
