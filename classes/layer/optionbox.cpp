 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OPTIONBOX_
#define __OBJSMOOTH_OPTIONBOX_

#include <smooth/optionbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_OPTIONBOX = SMOOTH::RequestObjectID();

SMOOTHOptionBox::SMOOTHOptionBox(SMOOTHString text, SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt *var, SMOOTHInt iCode, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_OPTIONBOX;
	objectProperties->text		= text;
	variable			= var;
	code				= iCode;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;

	if (*variable == code)	state = SMOOTH::True;
	else			state = SMOOTH::False;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(16 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHOptionBox::~SMOOTHOptionBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHOptionBox::Paint(SMOOTHInt message)
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
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	SMOOTHInt	 lightColor;

	if (active)	lightColor = SMOOTH::Setup::ClientColor;
	else		lightColor = SMOOTH::Setup::BackgroundColor;

	lineStart.x = realPos.x + METRIC_OPTBOXOFFSETXY + 3;
	lineStart.y = realPos.y + METRIC_OPTBOXOFFSETXY;
	lineEnd.x = lineStart.x + 5;
	lineEnd.y = lineStart.y;

	Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

	lineStart.x--;
	lineStart.y++;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerDarkColor);

	lineStart.x--;
	lineStart.y++;
	lineEnd.x += 2;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerDarkColor);
	PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

	lineStart.x--;
	lineEnd.x++;

	for (SMOOTHInt i = 0; i < 5; i++)
	{
		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

		PaintPixel(dc, lineStart, SMOOTH::Setup::DividerDarkColor);
		PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);
	}

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerLightColor);
	PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerLightColor);
	PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

	if (*variable == code)
	{
		lineStart.x++;
		lineStart.y -= 7;
		lineEnd.x--;
		lineEnd.y -= 7;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);

		lineStart.x--;
		lineEnd.x++;

		for (SMOOTHInt i = 0; i < 3; i++)
		{
			lineStart.y++;
			lineEnd.y++;

			Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);
		}

		PaintPixel(dc, SMOOTHPoint(lineEnd.x, lineEnd.y - 1), SMOOTH::Setup::DividerDarkColor);
		PaintPixel(dc, lineEnd, SMOOTH::Setup::DividerDarkColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);

		PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerDarkColor);
		PaintPixel(dc, lineEnd, SMOOTH::Setup::DividerDarkColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);
	}

	textRect.left	= realPos.x + METRIC_OPTBOXOFFSETXY + 14;
	textRect.top	= realPos.y + METRIC_OPTBOXOFFSETXY - METRIC_OPTBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHOptionBox::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered) return SMOOTH::Error;
	if ((!active && message != SM_CHECKOPTIONBOXES) || !visible) return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHObject	*object;
	SMOOTHRect	 frame;
	HDC		 dc;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	switch (message)
	{
		case SM_CHECKOPTIONBOXES:
			if (state == SMOOTH::True && *variable != code)
			{
				state = SMOOTH::False;

				Paint(SP_PAINT);

				retVal = SMOOTH::Break;
			}
			else if (state == SMOOTH::False && *variable == code)
			{
				state = SMOOTH::True;

				Paint(SP_PAINT);

				retVal = SMOOTH::Break;
			}

			break;
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			if (objectProperties->checked)
			{
				objectProperties->clicked = SMOOTH::True;

				Frame(dc, frame, FRAME_DOWN);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;
				objectProperties->checked = SMOOTH::False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				if (*variable != code)
				{
					*variable = code;

					state = SMOOTH::True;

					Paint(SP_PAINT);

					for (SMOOTHInt i = 0; i < SMOOTHObject::objectCount; i++)
					{
						object = mainObjectManager->RequestObject(i);

						if (object != NIL)
						{
							if (object->GetObjectType() == OBJ_OPTIONBOX) object->Process(SM_CHECKOPTIONBOXES, 0, 0);
						}
					}

					SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
				}

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			if (!objectProperties->checked && IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::True;
				Frame(dc, frame, FRAME_UP);
			}
			else if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

SMOOTHInt SMOOTHOptionBox::SetText(SMOOTHString newText)
{
	if (!registered || !visible)
	{
		objectProperties->text = newText;
		return SMOOTH::Success;
	}

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 textRect;
	HDC		 dc = GetContext(wnd);

	textRect.left	= realPos.x + METRIC_OPTBOXOFFSETXY + 14;
	textRect.top	= realPos.y + METRIC_OPTBOXOFFSETXY - METRIC_OPTBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::BackgroundColor, objectProperties->fontWeight);

	objectProperties->text = newText;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

#endif
