 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/groupbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_GROUPBOX = S::Object::RequestObjectID();

S::GUI::GroupBox::GroupBox(String text, Point pos, Size size)
{
	type			= OBJ_GROUPBOX;
	objectProperties->text	= text;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);

	GetTextSize();
}

S::GUI::GroupBox::~GroupBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::GroupBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!IsVisible())	return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	surface->Frame(frame, FRAME_DOWN);

	frame.left++;
	frame.top++;
	frame.right--;
	frame.bottom--;

	surface->Frame(frame, FRAME_UP);

	textRect.left	= frame.left + 9;
	textRect.top	= frame.top - METRIC_GBTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	Font	 font = objectProperties->font;

	if (!active) font.SetColor(Setup::GrayTextColor);

	surface->SetText(objectProperties->text, textRect, font);

	return Success;
}

S::Int S::GUI::GroupBox::Activate()
{
	active = True;

	if (!registered)	return Success;
	if (!IsVisible())	return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	surface->SetText(objectProperties->text, textRect, objectProperties->font);

	return Success;
}

S::Int S::GUI::GroupBox::Deactivate()
{
	active = False;

	if (!registered)	return Success;
	if (!IsVisible())	return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
	textRect.right	= textRect.left + objectProperties->textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	Font	 font = objectProperties->font;

	font.SetColor(Setup::GrayTextColor);

	surface->SetText(objectProperties->text, textRect, font);

	return Success;
}

S::Int S::GUI::GroupBox::Hide()
{
	if (!visible) return Success;

	Bool	 wasVisible = IsVisible();

	visible = False;

	if (!registered) return Success;

	if (wasVisible)
	{
		Rect	 rect;
		Point	 realPos = GetRealPosition();
		Surface	*surface = myContainer->GetDrawSurface();

		rect.left	= realPos.x + 10;
		rect.top	= realPos.y - METRIC_GBTEXTOFFSETY + 1;
		rect.right	= rect.left + objectProperties->textSize.cx + 3;
		rect.bottom	= rect.top + Math::Round(objectProperties->textSize.cy * 1.2);

		surface->Box(rect, Setup::BackgroundColor, FILLED);

		rect.left	= realPos.x;
		rect.top	= realPos.y;
		rect.right	= realPos.x + objectProperties->size.cx + 1;
		rect.bottom	= realPos.y + objectProperties->size.cy + 1;

		surface->Box(rect, Setup::BackgroundColor, FILLED);
	}

	return Success;
}
