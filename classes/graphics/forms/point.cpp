 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/forms/point.h>
#include <smooth/misc/math.h>

S::GUI::Point S::GUI::Point::operator +(const Point &point) const
{
	Point	 retP(x + point.x, y + point.y);

	return retP;
}

S::GUI::Point S::GUI::Point::operator -(const Point &point) const
{
	Point	 retP(x - point.x, y - point.y);

	return retP;
}

S::GUI::Point S::GUI::Point::operator *(const Float factor) const
{
	Point	 retP(Math::Round(x * factor), Math::Round(y * factor));

	return retP;
}

S::GUI::Point S::GUI::Point::operator /(const Float divisor) const
{
	Point	 retP(Math::Round(x / divisor), Math::Round(y / divisor));

	return retP;
}

S::GUI::Point &S::GUI::Point::operator +=(const Point &point)
{
	x += point.x;
	y += point.y;

	return *this;
}

S::GUI::Point &S::GUI::Point::operator -=(const Point &point)
{
	x -= point.x;
	y -= point.y;

	return *this;
}

S::GUI::Point &S::GUI::Point::operator *=(const Float factor)
{
	x = Math::Round(x * factor);
	y = Math::Round(y * factor);

	return *this;
}

S::GUI::Point &S::GUI::Point::operator /=(const Float divisor)
{
	x = Math::Round(x / divisor);
	y = Math::Round(y / divisor);

	return *this;
}

S::Bool S::GUI::Point::operator ==(const Point &point) const
{
	if (x == point.x && y == point.y)	return True;
	else					return False;
}

S::Bool S::GUI::Point::operator !=(const Point &point) const
{
	if (x != point.x || y != point.y)	return True;
	else					return False;
}

#ifdef __WIN32__
S::GUI::Point::operator POINT() const
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
