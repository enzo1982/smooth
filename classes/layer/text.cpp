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

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 textRect;

	objectProperties->size = objectProperties->textSize;

	textRect.left	= realPos.x;
	textRect.top	= realPos.y;
	textRect.right	= textRect.left + objectProperties->textSize.cx;
	textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);

	if (active)	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
	else		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

#endif
