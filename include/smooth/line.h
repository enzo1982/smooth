 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LINE_
#define _H_OBJSMOOTH_LINE_

#define SLine SMOOTHLine

class SMOOTHLine;

#include "primitive.h"
#include "point.h"

class SMOOTHAPI SMOOTHLine : public SMOOTHPrimitive
{
	public:
		SMOOTHInt	 sx;
		SMOOTHInt	 sy;
		SMOOTHInt	 ex;
		SMOOTHInt	 ey;
				 SMOOTHLine();
				 SMOOTHLine(SMOOTHPoint, SMOOTHPoint);
};

#endif
