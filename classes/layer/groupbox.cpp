 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_GROUPBOX_
#define __OBJSMOOTH_GROUPBOX_

#include <smooth/groupbox.h>
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

SMOOTHInt	 OBJ_GROUPBOX = SMOOTH::RequestObjectID();

SMOOTHGroupBox::SMOOTHGroupBox(SMOOTHString text, SMOOTHPoint pos, SMOOTHSize size)
{
	type			= OBJ_GROUPBOX;
	objectProperties->text	= text;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);

	GetTextSize();
}

SMOOTHGroupBox::~SMOOTHGroupBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHGroupBox::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	Frame(dc, frame, FRAME_DOWN);

	frame.left++;
	frame.top++;
	frame.right--;
	frame.bottom--;

	Frame(dc, frame, FRAME_UP);

	textRect.left	= frame.left + 9;
	textRect.top	= frame.top - METRIC_GBTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);

	Box(dc, textRect, SMOOTH::Setup::BackgroundColor, FILLED);

	textRect.left++;

	if (active)	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
	else		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHGroupBox::Activate()
{
	active = SMOOTH::True;

	if (!registered)	return SMOOTH::Success;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);

	Box(dc, textRect, SMOOTH::Setup::BackgroundColor, FILLED);

	textRect.left++;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHGroupBox::Deactivate()
{
	active = SMOOTH::False;

	if (!registered)	return SMOOTH::Success;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);

	Box(dc, textRect, SMOOTH::Setup::BackgroundColor, FILLED);

	textRect.left++;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHGroupBox::Hide()
{
	if (!visible)		return SMOOTH::Success;

	visible = SMOOTH::False;

	if (!registered)	return SMOOTH::Success;

	SMOOTHRect	 rect;
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	rect.left	= realPos.x + 10;
	rect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
	rect.right	= rect.left + objectProperties->textSize.cx + 3;
	rect.bottom	= rect.top + roundtoint(objectProperties->textSize.cy * 1.2);

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	return SMOOTH::Success;
}

#endif
