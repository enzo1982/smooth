 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ACTIVEAREA_
#define _H_OBJSMOOTH_ACTIVEAREA_

#define SActiveArea SMOOTHActiveArea

class SMOOTHActiveArea;

#include "object.h"

class SMOOTHAPI SMOOTHActiveArea : public SMOOTHObject
{
	protected:
		SMOOTHInt		 areaColor;
	public:
					 SMOOTHActiveArea(SMOOTHInt, SMOOTHPoint, SMOOTHSize, SMOOTHProcParam, SMOOTHVoid *);
					~SMOOTHActiveArea();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetColor(SMOOTHInt);
		SMOOTHInt		 GetColor();
};

SMOOTHVAR SMOOTHInt OBJ_ACTIVEAREA;

#endif
