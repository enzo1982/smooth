 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/rect.h>

S::Rect::Rect()
{
	left	= 0;
	top	= 0;
	right	= 0;
	bottom	= 0;
}

S::Rect::Rect(Point iPos, Size iSize)
{
	left	= iPos.x;
	top	= iPos.y;
	right	= left + iSize.cx;
	bottom	= top + iSize.cy;
}

#ifdef __WIN32__
S::Rect::operator RECT()
{
	RECT	 rect;

	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom	= bottom;

	return rect;
}

S::Rect &S::Rect::operator =(const RECT rect)
{
	left	= rect.left;
	top	= rect.top;
	right	= rect.right;
	bottom	= rect.bottom;

	return *this;
}
#endif
