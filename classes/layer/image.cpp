 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/image.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Image::classID = S::Object::RequestClassID();

S::GUI::Image::Image(Bitmap &bmp, Point pos, Size size)
{
	type	= classID;
	bitmap	= bmp;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0 && size.cy == 0)
	{
		objectProperties->size.cx = Math::Round(bitmap.GetSize().cx * Setup::FontSize);
		objectProperties->size.cy = Math::Round(bitmap.GetSize().cy * Setup::FontSize);
	}
	else
	{
		objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
}

S::GUI::Image::~Image()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Image::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 bmpRect;
	Point	 realPos = GetRealPosition();

	bmpRect.left	= realPos.x;
	bmpRect.top	= realPos.y;
	bmpRect.right	= realPos.x + objectProperties->size.cx;
	bmpRect.bottom	= realPos.y + objectProperties->size.cy;

	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), objectProperties->size), bmpRect);

	return Success;
}

S::Int S::GUI::Image::SetBitmap(const Bitmap &newBmp)
{
	Bool	 prevVisible = visible;

	if (visible) Hide();

	bitmap = newBmp;

	if (objectProperties->size.cx == 0 && objectProperties->size.cy == 0)
	{
		objectProperties->size.cx = Math::Round(bitmap.GetSize().cx * Setup::FontSize);
		objectProperties->size.cy = Math::Round(bitmap.GetSize().cy * Setup::FontSize);
	}

	if (prevVisible) Show();

	return Success;
}

S::GUI::Bitmap &S::GUI::Image::GetBitmap()
{
	return bitmap;
}
