 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TEXT_
#define __OBJSMOOTH_TEXT_

#include <smooth/text.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_TEXT = SMOOTH::RequestObjectID();

SMOOTHText::SMOOTHText(SMOOTHString text, SMOOTHPoint pos)
{
	type = OBJ_TEXT;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	objectProperties->text = text;

	GetTextSize();
}

SMOOTHText::~SMOOTHText()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHText::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	objectProperties->size = objectProperties->textSize;

	switch (message)
	{
		default:
		case SP_PAINT:
			textRect.left	= realPos.x;
			textRect.top	= realPos.y;
			textRect.right	= textRect.left + objectProperties->textSize.cx;
			textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);

			if (active)	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
			else		surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

			break;
	}

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

#endif
