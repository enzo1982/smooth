 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/surface.h>

S::GUI::Surface::Surface()
{
	size.cx	= 0;
	size.cy	= 0;
}

S::GUI::Surface::~Surface()
{
}

S::Size S::GUI::Surface::GetSize()
{
	return size;
}

S::Int S::GUI::Surface::SetPixel(Int x, Int y, Int color)
{
	return Success;
}

S::Int S::GUI::Surface::GetPixel(Int x, Int y)
{
	return 0;
}

S::Int S::GUI::Surface::Line(Point pos1, Point pos2, Int color)
{
	return Success;
}

S::Int S::GUI::Surface::Frame(Rect rect, Int style)
{
	return Success;
}

S::Int S::GUI::Surface::Box(Rect rect, Int color, Int style)
{
	return Success;
}

S::Int S::GUI::Surface::SetText(String string, Rect rect, String font, Int size, Int color, Int weight)
{
	return Success;
}

S::Int S::GUI::Surface::BlitFromBitmap(HBITMAP bitmap, Rect srcRect, Rect destRect)
{
	return Success;
}

S::Int S::GUI::Surface::BlitToBitmap(Rect srcRect, HBITMAP bitmap, Rect destRect)
{
	return Success;
}
