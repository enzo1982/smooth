 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_RECT_
#define _H_OBJSMOOTH_RECT_

#define SRect SMOOTHRect

class SMOOTHRect;

#include "primitive.h"
#include "definitions.h"

class SMOOTHAPI SMOOTHRect : public SMOOTHPrimitive
{
	public:
#ifdef __WIN32__
		operator		 RECT();
		SMOOTHRect &operator	 =(const RECT);
#endif
		SMOOTHInt		 left;
		SMOOTHInt		 top;
		SMOOTHInt		 right;
		SMOOTHInt		 bottom;
					 SMOOTHRect();
					 SMOOTHRect(SMOOTHPoint, SMOOTHSize);
};

#endif
