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
	}

	return color;
}
