 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/none/bitmapnone.h>

S::GUI::BitmapNone	 S::GUI::SI_DEFAULT = S::GUI::BitmapNone();

S::GUI::BitmapNone::BitmapNone()
{
	type = BITMAP_NONE;
}

S::GUI::BitmapNone::BitmapNone(Int cx, Int cy, Int bpp)
{
	type = BITMAP_NONE;
}

S::GUI::BitmapNone::BitmapNone(const int nil)
{
	type = BITMAP_NONE;
}

S::GUI::BitmapNone::BitmapNone(const BitmapNone &iBitmap)
{
	type = BITMAP_NONE;
}

S::GUI::BitmapNone::~BitmapNone()
{
}

S::Bool S::GUI::BitmapNone::CreateBitmap(Int cx, Int cy, Int bpp)
{
	return True;
}

S::Bool S::GUI::BitmapNone::DeleteBitmap()
{
	return True;
}

S::GUI::BitmapNone &S::GUI::BitmapNone::operator =(const int nil)
{
	return *this;
}

S::GUI::BitmapNone &S::GUI::BitmapNone::operator =(const BitmapNone &newBitmap)
{
	return *this;
}

S::Bool S::GUI::BitmapNone::operator ==(const int nil)
{
	return True;
}

S::Bool S::GUI::BitmapNone::operator !=(const int nil)
{
	return False;
}
