 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POINT_
#define _H_OBJSMOOTH_POINT_

#define SPoint SMOOTHPoint

class SMOOTHPoint;

#include "primitive.h"

class SMOOTHAPI SMOOTHPoint : public SMOOTHPrimitive
{
	public:
#ifdef __WIN32__
		operator		 POINT();
		SMOOTHPoint &operator	 =(const POINT);
#endif
		SMOOTHInt		 x;
		SMOOTHInt		 y;
					 SMOOTHPoint();
					 SMOOTHPoint(SMOOTHInt, SMOOTHInt);
};

#endif
