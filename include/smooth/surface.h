 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACE_
#define _H_OBJSMOOTH_SURFACE_

#define SSurface SMOOTHSurface

class SMOOTHSurface;

#include "definitions.h"

class SMOOTHAPI SMOOTHSurface
{
	protected:
		SMOOTHSize		 size;
	public:
					 SMOOTHSurface();
		virtual			~SMOOTHSurface();

		SMOOTHSize		 GetSize();

		virtual SMOOTHInt	 GetPixel(SMOOTHInt, SMOOTHInt);

		virtual SMOOTHInt	 SetPixel(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		virtual SMOOTHInt	 Line(SMOOTHPoint, SMOOTHPoint, SMOOTHInt);
		virtual SMOOTHInt	 Frame(SMOOTHRect, SMOOTHInt);
		virtual SMOOTHInt	 Box(SMOOTHRect, SMOOTHInt, SMOOTHInt);

		virtual SMOOTHInt	 SetText(SMOOTHString, SMOOTHRect, SMOOTHString, SMOOTHInt, SMOOTHInt, SMOOTHInt);

		virtual SMOOTHInt	 BlitFromBitmap(HBITMAP, SMOOTHRect, SMOOTHRect);
		virtual SMOOTHInt	 BlitToBitmap(SMOOTHRect, HBITMAP, SMOOTHRect);
};

#endif
