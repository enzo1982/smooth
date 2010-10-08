 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/threads/mutex.h>

S::GUI::Surface::Surface(Void *iSurface, const Size &maxSize)
{
	if (iSurface == NIL)	mutex = NIL;
	else			mutex = new Threads::Mutex();

	backend = SurfaceBackend::CreateBackendInstance(iSurface, maxSize);
}

S::GUI::Surface::~Surface()
{
	delete backend;

	if (mutex != NIL) delete mutex;
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
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->PaintRect(pRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::StartPaint(const Rect &pRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->StartPaint(pRect);

	return rVal;
}

S::Int S::GUI::Surface::EndPaint()
{
	Int	 rVal = backend->EndPaint();

	if (mutex != NIL) mutex->Release();

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
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->SetPixel(point, color);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Line(const Point &pos1, const Point &pos2, const Color &color)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Line(pos1, pos2, color);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Frame(const Rect &rect, Short style)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Frame(rect, style);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Box(const Rect &rect, const Color &color, Int style, const Size &ellipse)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Box(rect, color, style, ellipse);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::SetText(const String &string, const Rect &rect, const Font &font, Bool shadow)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->SetText(string, rect, font, shadow);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Gradient(const Rect &rect, const Color &color1, const Color &color2, Int style)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Gradient(rect, color1, color2, style);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Bar(const Point &p1, const Point &p2, Int orientation)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Bar(p1, p2, orientation);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &destRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->BlitFromBitmap(bitmap, srcRect, destRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitToBitmap(const Rect &srcRect, const Bitmap &bitmap, const Rect &destRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->BlitToBitmap(srcRect, bitmap, destRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}
