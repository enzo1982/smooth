 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_RECT_
#define __OBJSMOOTH_RECT_

#include <smooth/rect.h>

SMOOTHRect::SMOOTHRect()
{
	left	= 0;
	top	= 0;
	right	= 0;
	bottom	= 0;
}

SMOOTHRect::SMOOTHRect(SMOOTHPoint iPos, SMOOTHSize iSize)
{
	left	= iPos.x;
	top	= iPos.y;
	right	= left + iSize.cx;
	bottom	= top + iSize.cy;
}

#ifdef __WIN32__
SMOOTHRect::operator RECT()
{
	RECT	 rect;

	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom	= bottom;

	return rect;
}

SMOOTHRect &SMOOTHRect::operator =(const RECT rect)
{
	left	= rect.left;
	top	= rect.top;
	right	= rect.right;
	bottom	= rect.bottom;

	return *this;
}
#endif

#endif
