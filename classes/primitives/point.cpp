 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_POINT_
#define __OBJSMOOTH_POINT_

#include <smooth/point.h>

SMOOTHPoint::SMOOTHPoint()
{
	x = 0;
	y = 0;
}

SMOOTHPoint::SMOOTHPoint(SMOOTHInt iX, SMOOTHInt iY)
{
	x = iX;
	y = iY;
}

#ifdef __WIN32__
SMOOTHPoint::operator POINT()
{
	POINT	 point;

	point.x = x;
	point.y = y;

	return point;
}

SMOOTHPoint &SMOOTHPoint::operator =(const POINT point)
{
	x = point.x;
	y = point.y;

	return *this;
}
#endif

#endif
