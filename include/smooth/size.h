 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SIZE_
#define _H_OBJSMOOTH_SIZE_

#define SSize SMOOTHSize

class SMOOTHSize;

#include "primitive.h"
#include "definitions.h"

class SMOOTHAPI SMOOTHSize : public SMOOTHPrimitive
{
	public:
#ifdef __WIN32__
		operator		 SIZE();
		SMOOTHSize &operator	 =(const SIZE);
#endif
		SMOOTHInt		 cx;
		SMOOTHInt		 cy;
					 SMOOTHSize();
					 SMOOTHSize(SMOOTHInt, SMOOTHInt);
};

#endif
