 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/checkbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_CHECKBOX = S::Object::RequestObjectID();

S::GUI::CheckBox::CheckBox(String text, Point pos, Size size, Bool *var)
{
	type				= OBJ_CHECKBOX;
	objectProperties->text		= text;
	variable			= var;
	objectProperties->fontColor	= Setup::ClientTextColor;
	state				= *variable;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(16 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);

	GetTextSize();
}

S::GUI::CheckBox::~CheckBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::CheckBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Rect	 textRect;
	Point	 lineStart;
	Point	 lineEnd;
	Int	 shadowColor;
	Int	 crossColor;

	frame.left	= realPos.x + METRIC_CHECKBOXOFFSETXY;
	frame.top	= realPos.y + METRIC_CHECKBOXOFFSETXY;
	frame.right	= frame.left + 10;
	frame.bottom	= frame.top + 10;

	if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
	else		surface->Box(frame, Setup::BackgroundColor, FILLED);

	surface->Frame(frame, FRAME_DOWN);

	if (active)
	{
		shadowColor = Setup::DividerDarkColor;
		crossColor = Setup::ClientTextColor;
	}
	else
	{
		shadowColor = Setup::DividerDarkColor;
		crossColor = Setup::GrayTextColor;
	}

	if (*variable == True)
	{
		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right;
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 4;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 1;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 1;
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.left + 3;
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 2;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.left + 3;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1;
		lineEnd.y = frame.bottom - 2;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.left + 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right - 2;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 2;
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2;
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 3;
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1;
		lineEnd.y = frame.bottom - 2;

		surface->Line(lineStart, lineEnd, crossColor);
	}

	textRect.left	= frame.right + 4;
	textRect.top	= frame.top - METRIC_CHECKBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	if (active)	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
	else		surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, Setup::GrayTextColor, objectProperties->fontWeight);

	if (objectProperties->checked) surface->Frame(frame, FRAME_UP);

	return Success;
}

S::Int S::GUI::CheckBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered) return Error;
	if ((!active && message != SM_CHECKCHECKBOXES) || !visible) return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Object	*object;
	Rect	 frame;

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

				retVal = Break;
			}

			break;
		case SM_LBUTTONDOWN:
			if (objectProperties->checked)
			{
				objectProperties->clicked = True;

				surface->Frame(frame, FRAME_DOWN);

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;
				objectProperties->checked = False;

				frame.right++;
				frame.bottom++;

				surface->Box(frame, Setup::BackgroundColor, OUTLINED);

				frame.right--;
				frame.bottom--;

				if (*variable == False)	*variable = True;
				else			*variable = False;

				state = *variable;

				Paint(SP_PAINT);

				Process(SM_MOUSEMOVE, 0, 0);

				for (Int i = 0; i < objectCount; i++)
				{
					object = mainObjectManager->RequestObject(i);

					if (object != NIL)
					{
						if (object->GetObjectType() == OBJ_CHECKBOX) ((CheckBox *) object)->Process(SM_CHECKCHECKBOXES, 0, 0);
					}
				}

				onClick.Emit();

				retVal = Break;
			}

			break;
		case SM_MOUSELEAVE:
			if (objectProperties->checked && !wnd->IsMouseOn(frame))
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;
			}

			break;
		case SM_MOUSEMOVE:
			if (!objectProperties->checked && wnd->IsMouseOn(frame))
			{
				objectProperties->checked = True;
				surface->Frame(frame, FRAME_UP);
			}
			else if (objectProperties->checked && !wnd->IsMouseOn(frame))
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;
			}

			break;
	}

	return retVal;
}
