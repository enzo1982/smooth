 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SURFACE_
#define __OBJSMOOTH_SURFACE_

#include <smooth/surface.h>
#include <smooth/stk.h>

SMOOTHSurface::SMOOTHSurface()
{
	size.cx	= 0;
	size.cy	= 0;
}

SMOOTHSurface::~SMOOTHSurface()
{
}

SMOOTHSize SMOOTHSurface::GetSize()
{
	return size;
}

SMOOTHInt SMOOTHSurface::SetPixel(SMOOTHInt x, SMOOTHInt y, SMOOTHInt color)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::GetPixel(SMOOTHInt x, SMOOTHInt y)
{
	return 0;
}

SMOOTHInt SMOOTHSurface::Line(SMOOTHPoint pos1, SMOOTHPoint pos2, SMOOTHInt color)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::Frame(SMOOTHRect rect, SMOOTHInt style)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::Box(SMOOTHRect rect, SMOOTHInt color, SMOOTHInt style)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::SetText(SMOOTHString string, SMOOTHRect rect, SMOOTHString font, SMOOTHInt size, SMOOTHInt color, SMOOTHInt weight)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::BlitFromBitmap(HBITMAP bitmap, SMOOTHRect srcRect, SMOOTHRect destRect)
{
	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurface::BlitToBitmap(SMOOTHRect srcRect, HBITMAP bitmap, SMOOTHRect destRect)
{
	return SMOOTH::Success;
}

#endif
