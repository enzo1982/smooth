 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LINE_
#define __OBJSMOOTH_LINE_

#include <smooth/line.h>

SMOOTHLine::SMOOTHLine()
{
	sx = 0;
	sy = 0;
	ex = 0;
	ey = 0;
}

SMOOTHLine::SMOOTHLine(SMOOTHPoint iStart, SMOOTHPoint iEnd)
{
	sx = iStart.x;
	sy = iStart.y;
	ex = iEnd.x;
	ey = iEnd.y;
}

#endif
