 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/primitives/point.h>
#include <smooth/misc/math.h>

S::Point::Point()
{
	x = 0;
	y = 0;
}

S::Point::Point(Int iX, Int iY)
{
	x = iX;
	y = iY;
}

S::Point S::Point::operator +(const Point &point)
{
	Point	 retP(x + point.x, y + point.y);

	return retP;
}

S::Point S::Point::operator -(const Point &point)
{
	Point	 retP(x - point.x, y - point.y);

	return retP;
}

S::Point S::Point::operator *(const Float factor)
{
	Point	 retP(Math::Round(x * factor), Math::Round(y * factor));

	return retP;
}

S::Point S::Point::operator /(const Float divisor)
{
	Point	 retP(Math::Round(x / divisor), Math::Round(y / divisor));

	return retP;
}

S::Bool S::Point::operator ==(const Point &point)
{
	if (x == point.x && y == point.y)	return True;
	else					return False;
}

S::Bool S::Point::operator !=(const Point &point)
{
	if (x != point.x || y != point.y)	return True;
	else					return False;
}

#ifdef __WIN32__
S::Point::operator POINT()
{
	POINT	 point;

	point.x = x;
	point.y = y;

	return point;
}

S::Point &S::Point::operator =(const POINT &point)
{
	x = point.x;
	y = point.y;

	return *this;
}
#endif
