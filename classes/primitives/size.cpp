 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SIZE_
#define __OBJSMOOTH_SIZE_

#include <smooth/size.h>

SMOOTHSize::SMOOTHSize()
{
	cx = 0;
	cy = 0;
}

SMOOTHSize::SMOOTHSize(SMOOTHInt iCX, SMOOTHInt iCY)
{
	cx = iCX;
	cy = iCY;
}

#ifdef __WIN32__
SMOOTHSize::operator SIZE()
{
	SIZE	 size;

	size.cx = cx;
	size.cy = cy;

	return size;
}

SMOOTHSize &SMOOTHSize::operator =(const SIZE size)
{
	cx = size.cx;
	cy = size.cy;

	return *this;
}
#endif

#endif
