 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/surface.h>
#include <smooth/graphics/surfacebackend.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/threads/mutex.h>

S::GUI::Surface::Surface(Void *iSurface)
{
	if (iSurface == NIL)	mutex = NIL;
	else			mutex = new Threads::Mutex();

	backend = SurfaceBackend::CreateBackendInstance(iSurface);
}

S::GUI::Surface::~Surface()
{
	delete backend;

	if (mutex != NIL) Object::DeleteObject(mutex);
}

S::Int S::GUI::Surface::GetSurfaceType()
{
	return backend->GetSurfaceType();
}

S::Int S::GUI::Surface::SetSize(Size nSize)
{
	return backend->SetSize(nSize);
}

const S::GUI::Size S::GUI::Surface::GetSize()
{
	return backend->GetSize();
}

S::Int S::GUI::Surface::PaintRect(Rect pRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->PaintRect(pRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::StartPaint(Rect pRect)
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

S::Void *S::GUI::Surface::GetSystemSurface()
{
	return backend->GetSystemSurface();
}

S::Int S::GUI::Surface::SetPixel(Int x, Int y, Int color)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->SetPixel(x, y, color);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::GetPixel(Int x, Int y)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->GetPixel(x, y);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Line(Point pos1, Point pos2, Int color)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Line(pos1, pos2, color);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Frame(Rect rect, Int style)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Frame(rect, style);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Box(Rect rect, Int color, Int style)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Box(rect, color, style);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::SetText(String string, Rect rect, Font font, Bool shadow)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->SetText(string, rect, font, shadow);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Gradient(Rect rect, Int color1, Int color2, Int style)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Gradient(rect, color1, color2, style);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::Bar(Point p1, Point p2, Int orientation)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->Bar(p1, p2, orientation);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitFromBitmap(const Bitmap &bitmap, Rect srcRect, Rect destRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->BlitFromBitmap(bitmap, srcRect, destRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}

S::Int S::GUI::Surface::BlitToBitmap(Rect srcRect, const Bitmap &bitmap, Rect destRect)
{
	if (mutex != NIL) mutex->Lock();

	Int	 rVal = backend->BlitToBitmap(srcRect, bitmap, destRect);

	if (mutex != NIL) mutex->Release();

	return rVal;
}
