 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACEGDI_
#define _H_OBJSMOOTH_SURFACEGDI_

#define SSurfaceGDI SMOOTHSurfaceGDI

class SMOOTHSurfaceGDI;

#include "surface.h"

class SMOOTHAPI SMOOTHSurfaceGDI : public SMOOTHSurface
{
	protected:
		HDC		 gdi_dc;
	public:
				 SMOOTHSurfaceGDI(HDC);
				~SMOOTHSurfaceGDI();

		SMOOTHInt	 GetPixel(SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 SetPixel(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHInt	 Line(SMOOTHPoint, SMOOTHPoint, SMOOTHInt);
		SMOOTHInt	 Frame(SMOOTHRect, SMOOTHInt);
		SMOOTHInt	 Box(SMOOTHRect, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 SetText(SMOOTHString, SMOOTHRect, SMOOTHString, SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 BlitFromBitmap(HBITMAP, SMOOTHRect, SMOOTHRect);
		SMOOTHInt	 BlitToBitmap(SMOOTHRect, HBITMAP, SMOOTHRect);
};

#endif
