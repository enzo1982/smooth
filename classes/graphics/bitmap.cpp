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
#include <smooth/graphics/surface.h>
#include <smooth/color.h>

S::GUI::BitmapBase::BitmapBase()
{
	type		= -1;

	size		= Size(0, 0);
	depth		= 0;

	bytes		= NIL;
	align		= 0;
}

S::GUI::BitmapBase::BitmapBase(const int nil)
{
	type		= -1;

	size		= Size(0, 0);
	depth		= 0;

	bytes		= NIL;
	align		= 0;
}

S::GUI::BitmapBase::BitmapBase(const BitmapBase &iBitmap)
{
	type		= -1;

	size		= iBitmap.size;
	depth		= iBitmap.depth;

	bytes		= iBitmap.bytes;
	align		= iBitmap.align;
}

S::GUI::BitmapBase::~BitmapBase()
{
}

S::Int S::GUI::BitmapBase::GetBitmapType()
{
	return type;
}

S::Size S::GUI::BitmapBase::GetSize()
{
	return size;
}

S::Int S::GUI::BitmapBase::GetDepth()
{
	return depth;
}

S::UnsignedByte *S::GUI::BitmapBase::GetBytes()
{
	return bytes;
}

S::Int S::GUI::BitmapBase::GetLineAlignment()
{
	return align;
}

S::Bool S::GUI::BitmapBase::CreateBitmap(Int cx, Int cy, Int bpp)
{
	return False;
}

S::Bool S::GUI::BitmapBase::DeleteBitmap()
{
	return True;
}

S::Int S::GUI::BitmapBase::GrayscaleBitmap()
{
	if (bytes == NIL) return Error;

	Int	 color = 0;

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			color = GetPixel(x, y);
			color = (GetRed(color) + GetGreen(color) + GetBlue(color)) / 3;
			color = CombineColor(color, color, color);

			SetPixel(x, y, color);
		}
	}

	return Success;
}

S::Int S::GUI::BitmapBase::ReplaceColor(Int color1, Int color2)
{
	if (bytes == NIL) return Error;

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			if (GetPixel(x, y) == (UnsignedInt) color1) SetPixel(x, y, color2);
		}
	}

	return Success;
}

S::Int S::GUI::BitmapBase::BlitFromSurface(Surface *surface, Rect srcRect, Rect destRect)
{
	if (surface == NIL) return Error;

	return surface->BlitToBitmap(srcRect, *(Bitmap *) this, destRect);
}

S::Int S::GUI::BitmapBase::BlitToSurface(Rect srcRect, Surface *surface, Rect destRect)
{
	if (surface == NIL) return Error;

	return surface->BlitFromBitmap(*(Bitmap *) this, srcRect, destRect);
}

S::Bool S::GUI::BitmapBase::SetPixel(Int x, Int y, UnsignedLong color)
{
	if (bytes == NIL)			return False;
	if (y >= size.cy || x >= size.cx)	return False;

	Bool	 done = False;
	Int	 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + x * 3;

			bytes[offset + 0] = GetBlue(color);
			bytes[offset + 1] = GetGreen(color);
			bytes[offset + 2] = GetRed(color);

			done = True;

			break;
		case 32:
			offset = (size.cy - ++y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + x * 4;

			bytes[offset + 0] = GetBlue(color);
			bytes[offset + 1] = GetGreen(color);
			bytes[offset + 2] = GetRed(color);

			done = True;

			break;
	}

	return done;
}

S::UnsignedLong S::GUI::BitmapBase::GetPixel(Int x, Int y)
{
	if (bytes == NIL)			return 0;
	if (y >= size.cy || x >= size.cx)	return 0;

	UnsignedLong	 color = 0;
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - ++y) * (((4 - (size.cx * 3) & 3) & 3) + size.cx * 3) + x * 3;

			color = CombineColor(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0]);

			break;
		case 32:
			offset = (size.cy - ++y) * (((4 - (size.cx * 4) & 3) & 3) + size.cx * 4) + x * 4;

			color = CombineColor(bytes[offset + 2], bytes[offset + 1], bytes[offset + 0]);

			break;
	}

	return color;
}
