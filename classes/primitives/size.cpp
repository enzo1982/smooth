 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/size.h>

S::Size::Size()
{
	cx = 0;
	cy = 0;
}

S::Size::Size(Int iCX, Int iCY)
{
	cx = iCX;
	cy = iCY;
}

S::Size S::Size::operator +(const Size &size)
{
	Size	 retP(cx + size.cx, cy + size.cy);

	return retP;
}

S::Size S::Size::operator -(const Size &size)
{
	Size	 retS(cx - size.cx, cy - size.cy);

	return retS;
}

#ifdef __WIN32__
S::Size::operator SIZE()
{
	SIZE	 size;

	size.cx = cx;
	size.cy = cy;

	return size;
}

S::Size &S::Size::operator =(const SIZE size)
{
	cx = size.cx;
	cy = size.cy;

	return *this;
}
#endif
