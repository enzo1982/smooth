 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CHECKBOX_
#define __OBJSMOOTH_CHECKBOX_

#include <smooth/checkbox.h>
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

SMOOTHInt	 OBJ_CHECKBOX = SMOOTH::RequestObjectID();

SMOOTHCheckBox::SMOOTHCheckBox(SMOOTHString text, SMOOTHPoint pos, SMOOTHSize size, SMOOTHBool *var, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_CHECKBOX;
	objectProperties->text		= text;
	variable			= var;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;
	state				= *variable;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(16 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHCheckBox::~SMOOTHCheckBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHCheckBox::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	SMOOTHRect	 textRect;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	SMOOTHInt	 shadowColor;
	SMOOTHInt	 crossColor;

	frame.left	= realPos.x + METRIC_CHECKBOXOFFSETXY;
	frame.top	= realPos.y + METRIC_CHECKBOXOFFSETXY;
	frame.right	= frame.left + 10;
	frame.bottom	= frame.top + 10;

	if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

	Frame(dc, frame, FRAME_DOWN);

	if (active)
	{
		shadowColor = SMOOTH::Setup::DividerDarkColor;
		crossColor = SMOOTH::Setup::ClientTextColor;
	}
	else
	{
		shadowColor = SMOOTH::Setup::DividerDarkColor;
		crossColor = SMOOTH::Setup::GrayTextColor;
	}

	if (*variable == SMOOTH::True)
	{
		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right;
		lineEnd.y = frame.bottom;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.left + 4;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.right - 1;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.right - 1;
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.left + 3;
		lineEnd.y = frame.bottom;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, shadowColor, PS_SOLID, 1);

		lineStart.x = frame.left + 2;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);

		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom - 2;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);

		lineStart.x = frame.left + 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right - 2;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom - 1;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);

		lineStart.x = frame.right - 3;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1;
		lineEnd.y = frame.bottom - 2;

		Line(dc, lineStart, lineEnd, crossColor, PS_SOLID, 1);
	}

	textRect.left	= frame.right + 4;
	textRect.top	= frame.top - METRIC_CHECKBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	if (active)	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
	else		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHCheckBox::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered) return SMOOTH::Error;
	if ((!active && message != SM_CHECKCHECKBOXES) || !visible) return SMOOTH::Success;

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
		case SM_CHECKCHECKBOXES:
			if (state != *variable)
			{
				state = *variable;

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

				if (*variable == SMOOTH::False)	*variable = SMOOTH::True;
				else				*variable = SMOOTH::False;

				state = *variable;

				Paint(SP_PAINT);

				Process(SM_MOUSEMOVE, 0, 0);

				for (SMOOTHInt i = 0; i < SMOOTHObject::objectCount; i++)
				{
					object = mainObjectManager->RequestObject(i);

					if (object != NIL)
					{
						if (object->GetObjectType() == OBJ_CHECKBOX) object->Process(SM_CHECKCHECKBOXES, 0, 0);
					}
				}

				SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

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

#endif
