 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/rect.h>
#include <smooth/misc/math.h>

S::GUI::Rect::Rect()
{
	left	= 0;
	top	= 0;
	right	= 0;
	bottom	= 0;
}

S::GUI::Rect::Rect(const Point &iPos, const Size &iSize)
{
	left	= iPos.x;
	top	= iPos.y;
	right	= left + iSize.cx;
	bottom	= top + iSize.cy;
}

S::GUI::Rect S::GUI::Rect::operator *(const Float factor)
{
	Rect	 retR(Point(left, top) * factor, Size(right - left, bottom - top) * factor);

	return retR;
}

S::GUI::Rect S::GUI::Rect::operator /(const Float divisor)
{
	Rect	 retR(Point(left, top) / divisor, Size(right - left, bottom - top) / divisor);

	return retR;
}

S::Bool S::GUI::Rect::operator ==(const Rect &rect)
{
	if (left == rect.left && top == rect.top && right == rect.right && bottom == rect.bottom)	return True;
	else												return False;
}

S::Bool S::GUI::Rect::operator !=(const Rect &rect)
{
	if (left != rect.left || top != rect.top || right != rect.right || bottom != rect.bottom)	return True;
	else												return False;
}

#ifdef __WIN32__
S::GUI::Rect::operator RECT()
{
	RECT	 rect;

	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom	= bottom;

	return rect;
}

S::GUI::Rect &S::GUI::Rect::operator =(const RECT &rect)
{
	left	= rect.left;
	top	= rect.top;
	right	= rect.right;
	bottom	= rect.bottom;

	return *this;
}
#endif


S::Bool S::GUI::Rect::DoRectsOverlap(const Rect &rect1, const Rect &rect2)
{
	if ((rect1.left < rect2.right) && (rect1.right > rect2.left) && (rect1.top < rect2.bottom) && (rect1.bottom > rect2.top))
	{
		return true;
	}
	else
	{
		return false;
	}
}

S::GUI::Rect S::GUI::Rect::OverlapRect(const Rect &rect1, const Rect &rect2)
{
	Rect	 oRect;

	if (DoRectsOverlap(rect1, rect2))
	{
		oRect.left	= (Int) Math::Max(rect1.left, rect2.left);
		oRect.top	= (Int) Math::Max(rect1.top, rect2.top);
		oRect.right	= (Int) Math::Min(rect1.right, rect2.right);
		oRect.bottom	= (Int) Math::Min(rect1.bottom, rect2.bottom);
	}

	return oRect;
}
