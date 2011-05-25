 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/surface.h>
#include <smooth/graphics/backends/surfacebackend.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>

S::Threads::Mutex	 S::GUI::Surface::mutex;

S::GUI::Surface::Surface(Void *iSurface, const Size &maxSize)
{
	backend = SurfaceBackend::CreateBackendInstance(iSurface, maxSize);
}

S::GUI::Surface::~Surface()
{
	delete backend;
}

S::Short S::GUI::Surface::GetSurfaceType() const
{
	return backend->GetSurfaceType();
}

S::Int S::GUI::Surface::SetSize(const Size &nSize)
{
	return backend->SetSize(nSize);
}

const S::GUI::Size &S::GUI::Surface::GetSize() const
{
	return backend->GetSize();
}

S::Int S::GUI::Surface::SetRightToLeft(Bool nRightToLeft)
{
	return backend->SetRightToLeft(nRightToLeft);
}

S::Int S::GUI::Surface::PaintRect(const Rect &pRect)
{
	mutex.Lock();

	Int	 rVal = backend->PaintRect(pRect);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::StartPaint(const Rect &pRect)
{
	mutex.Lock();

	Int	 rVal = backend->StartPaint(pRect);

	return rVal;
}

S::Int S::GUI::Surface::EndPaint()
{
	Int	 rVal = backend->EndPaint();

	mutex.Release();

	return rVal;
}

S::Void *S::GUI::Surface::GetSystemSurface() const
{
	return backend->GetSystemSurface();
}

S::Short S::GUI::Surface::GetSurfaceDPI() const
{
	return backend->GetSurfaceDPI();
}

S::Int S::GUI::Surface::SetPixel(const Point &point, const Color &color)
{
	mutex.Lock();

	Int	 rVal = backend->SetPixel(point, color);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::Line(const Point &pos1, const Point &pos2, const Color &color)
{
	mutex.Lock();

	Int	 rVal = backend->Line(pos1, pos2, color);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::Frame(const Rect &rect, Short style)
{
	mutex.Lock();

	Int	 rVal = backend->Frame(rect, style);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::Box(const Rect &rect, const Color &color, Int style, const Size &ellipse)
{
	mutex.Lock();

	Int	 rVal = backend->Box(rect, color, style, ellipse);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::SetText(const String &string, const Rect &rect, const Font &font, Bool shadow)
{
	mutex.Lock();

	Int	 rVal = backend->SetText(string, rect, font, shadow);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::Gradient(const Rect &rect, const Color &color1, const Color &color2, Int style)
{
	mutex.Lock();

	Int	 rVal = backend->Gradient(rect, color1, color2, style);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::Bar(const Point &p1, const Point &p2, Int orientation)
{
	mutex.Lock();

	Int	 rVal = backend->Bar(p1, p2, orientation);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &destRect)
{
	mutex.Lock();

	Int	 rVal = backend->BlitFromBitmap(bitmap, srcRect, destRect);

	mutex.Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitToBitmap(const Rect &srcRect, Bitmap &bitmap, const Rect &destRect)
{
	mutex.Lock();

	Int	 rVal = backend->BlitToBitmap(srcRect, bitmap, destRect);

	mutex.Release();

	return rVal;
}
