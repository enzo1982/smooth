 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/bitmapbackend.h>

#ifdef __WIN32__
#include <smooth/graphics/gdi/bitmapgdi.h>
#endif

S::GUI::Bitmap	 S::GUI::SI_DEFAULT = S::GUI::Bitmap();

S::GUI::Bitmap::Bitmap(Void *iBitmap)
{
#ifdef __WIN32__
	backend = new BitmapGDI(iBitmap);
#else
	backend = new BitmapBackend(iBitmap);
#endif
}

S::GUI::Bitmap::Bitmap(Int cx, Int cy, Int bpp)
{
#ifdef __WIN32__
	backend = new BitmapGDI(cx, cy, bpp);
#else
	backend = new BitmapBackend(cx, cy, bpp);
#endif
}

S::GUI::Bitmap::Bitmap(const int nil)
{
#ifdef __WIN32__
	backend = new BitmapGDI(nil);
#else
	backend = new BitmapBackend(nil);
#endif
}

S::GUI::Bitmap::Bitmap(const Bitmap &iBitmap)
{
#ifdef __WIN32__
	backend = new BitmapGDI((BitmapGDI &) *(iBitmap.backend));
#else
	backend = new BitmapBackend((BitmapNone &) *(iBitmap.backend));
#endif
}

S::GUI::Bitmap::~Bitmap()
{
	delete backend;
}

S::Int S::GUI::Bitmap::GetBitmapType()
{
	return backend->GetBitmapType();
}

S::Size S::GUI::Bitmap::GetSize()
{
	return backend->GetSize();
}

S::Int S::GUI::Bitmap::GetDepth()
{
	return backend->GetDepth();
}

S::UnsignedByte *S::GUI::Bitmap::GetBytes()
{
	return backend->GetBytes();
}

S::Int S::GUI::Bitmap::GetLineAlignment()
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

S::Bool S::GUI::Bitmap::SetBitmap(Void *nBitmap)
{
	return backend->SetBitmap(nBitmap);
}

S::Void *S::GUI::Bitmap::GetBitmap()
{
	return backend->GetBitmap();
}

S::Int S::GUI::Bitmap::GrayscaleBitmap()
{
	return backend->GrayscaleBitmap();
}

S::Int S::GUI::Bitmap::ReplaceColor(Int color1, Int color2)
{
	return backend->ReplaceColor(color1, color2);
}

S::Int S::GUI::Bitmap::BlitFromSurface(Surface *surface, Rect srcRect, Rect destRect)
{
	return backend->BlitFromSurface(surface, srcRect, destRect);
}

S::Int S::GUI::Bitmap::BlitToSurface(Rect srcRect, Surface *surface, Rect destRect)
{
	return backend->BlitToSurface(srcRect, surface, destRect);
}

S::Bool S::GUI::Bitmap::SetPixel(Int x, Int y, UnsignedLong color)
{
	return backend->SetPixel(x, y, color);
}

S::UnsignedLong S::GUI::Bitmap::GetPixel(Int x, Int y)
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

S::Bool S::GUI::Bitmap::operator ==(const int nil)
{
	return (*backend == nil);
}

S::Bool S::GUI::Bitmap::operator !=(const int nil)
{
	return (*backend != nil);
}
