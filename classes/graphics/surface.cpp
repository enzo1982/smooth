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
#include <smooth/color.h>

#ifdef __WIN32__
#include <smooth/graphics/gdi/surfacegdi.h>
#endif

S::GUI::Surface::Surface(Void *iSurface)
{
#ifdef __WIN32__
	backend = new SurfaceGDI(iSurface);
#else
	backend = new SurfaceBackend(iSurface);
#endif
}

S::GUI::Surface::~Surface()
{
	delete backend;
}

S::Int S::GUI::Surface::GetSurfaceType()
{
	return backend->GetSurfaceType();
}

S::Int S::GUI::Surface::SetSize(Size nSize)
{
	return backend->SetSize(nSize);
}

S::Size S::GUI::Surface::GetSize()
{
	return backend->GetSize();
}

S::Int S::GUI::Surface::PaintRect(Rect pRect)
{
	return backend->PaintRect(pRect);
}

S::Int S::GUI::Surface::StartPaint(Rect pRect)
{
	return backend->StartPaint(pRect);
}

S::Int S::GUI::Surface::EndPaint()
{
	return backend->EndPaint();
}

S::Void *S::GUI::Surface::GetContext()
{
	return backend->GetContext();
}

S::Int S::GUI::Surface::SetPixel(Int x, Int y, Int color)
{
	return backend->SetPixel(x, y, color);
}

S::Int S::GUI::Surface::GetPixel(Int x, Int y)
{
	return backend->GetPixel(x, y);
}

S::Int S::GUI::Surface::Line(Point pos1, Point pos2, Int color)
{
	return backend->Line(pos1, pos2, color);
}

S::Int S::GUI::Surface::Frame(Rect rect, Int style)
{
	return backend->Frame(rect, style);
}

S::Int S::GUI::Surface::Box(Rect rect, Int color, Int style)
{
	return backend->Box(rect, color, style);
}

S::Int S::GUI::Surface::SetText(String string, Rect rect, Font font)
{
	return backend->SetText(string, rect, font);
}

S::Int S::GUI::Surface::SetShadowedText(String string, Rect rect, Font font)
{
	return backend->SetShadowedText(string, rect, font);
}

S::Int S::GUI::Surface::Gradient(Rect rect, Int color1, Int color2, Int style)
{
	return backend->Gradient(rect, color1, color2, style);
}

S::Int S::GUI::Surface::Bar(Point p1, Point p2, Int orientation)
{
	return backend->Bar(p1, p2, orientation);
}

S::Int S::GUI::Surface::BlitFromBitmap(const Bitmap &bitmap, Rect srcRect, Rect destRect)
{
	return backend->BlitFromBitmap(bitmap, srcRect, destRect);
}

S::Int S::GUI::Surface::BlitToBitmap(Rect srcRect, const Bitmap &bitmap, Rect destRect)
{
	return backend->BlitToBitmap(srcRect, bitmap, destRect);
}
