 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/activearea.h>
#include <smooth/definitions.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>

const S::Int	 S::GUI::ActiveArea::classID = S::Object::RequestClassID();

S::GUI::ActiveArea::ActiveArea(Int color, Point iPos, Size iSize)
{
	type		= classID;
	areaColor	= color;

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 20;

	borderWidth	= 1;
}

S::GUI::ActiveArea::~ActiveArea()
{
}

S::Int S::GUI::ActiveArea::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame		= Rect(GetRealPosition(), size);

	switch (message)
	{
		default:
		case SP_PAINT:
			surface->Box(frame, areaColor, FILLED);
			surface->Frame(frame, FRAME_DOWN);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ActiveArea::SetColor(Int newColor)
{
	areaColor = newColor;

	if (!registered)	return Success;
	if (!visible)		return Success;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ActiveArea::GetColor()
{
	return areaColor;
}
