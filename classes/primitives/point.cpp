 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/point.h>

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

#ifdef __WIN32__
S::Point::operator POINT()
{
	POINT	 point;

	point.x = x;
	point.y = y;

	return point;
}

S::Point &S::Point::operator =(const POINT point)
{
	x = point.x;
	y = point.y;

	return *this;
}
#endif
