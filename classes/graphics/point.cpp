 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/point.h>
#include <smooth/misc/math.h>

S::GUI::Point::Point()
{
	x = 0;
	y = 0;
}

S::GUI::Point::Point(Int iX, Int iY)
{
	x = iX;
	y = iY;
}

S::GUI::Point S::GUI::Point::operator +(const Point &point)
{
	Point	 retP(x + point.x, y + point.y);

	return retP;
}

S::GUI::Point S::GUI::Point::operator -(const Point &point)
{
	Point	 retP(x - point.x, y - point.y);

	return retP;
}

S::GUI::Point S::GUI::Point::operator *(const Float factor)
{
	Point	 retP(Math::Round(x * factor), Math::Round(y * factor));

	return retP;
}

S::GUI::Point S::GUI::Point::operator /(const Float divisor)
{
	Point	 retP(Math::Round(x / divisor), Math::Round(y / divisor));

	return retP;
}

S::Bool S::GUI::Point::operator ==(const Point &point)
{
	if (x == point.x && y == point.y)	return True;
	else					return False;
}

S::Bool S::GUI::Point::operator !=(const Point &point)
{
	if (x != point.x || y != point.y)	return True;
	else					return False;
}

#ifdef __WIN32__
S::GUI::Point::operator POINT()
{
	POINT	 point;

	point.x = x;
	point.y = y;

	return point;
}

S::GUI::Point &S::GUI::Point::operator =(const POINT &point)
{
	x = point.x;
	y = point.y;

	return *this;
}
#endif
