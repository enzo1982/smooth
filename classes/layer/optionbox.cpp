 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/optionbox.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_OPTIONBOX = S::Object::RequestObjectID();

S::OptionBox::OptionBox(String text, Point pos, Size size, Int *var, Int iCode, ProcParam, Void *procParam)
{
	type				= OBJ_OPTIONBOX;
	objectProperties->text		= text;
	variable			= var;
	code				= iCode;
	objectProperties->proc		= (ProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;

	if (*variable == code)	state = True;
	else			state = False;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(16 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);

	GetTextSize();
}

S::OptionBox::~OptionBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::OptionBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Point	 realPos = GetRealPosition();
	Rect	 textRect;
	Point	 lineStart;
	Point	 lineEnd;
	Int	 lightColor;

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
	PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

	lineStart.x--;
	lineEnd.x++;

	for (Int i = 0; i < 5; i++)
	{
		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

		PaintPixel(dc, lineStart, SMOOTH::Setup::DividerDarkColor);
		PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);
	}

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerLightColor);
	PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	Line(dc, lineStart, lineEnd, lightColor, PS_SOLID, 1);

	PaintPixel(dc, lineStart, SMOOTH::Setup::DividerLightColor);
	PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerLightColor);

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

		for (Int i = 0; i < 3; i++)
		{
			lineStart.y++;
			lineEnd.y++;

			Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);
		}

		PaintPixel(dc, Point(lineEnd.x, lineEnd.y - 1), SMOOTH::Setup::DividerDarkColor);
		PaintPixel(dc, lineEnd, SMOOTH::Setup::DividerDarkColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);

		PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y), SMOOTH::Setup::DividerDarkColor);
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

	return Success;
}

S::Int S::OptionBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered) return Error;
	if ((!active && message != SM_CHECKOPTIONBOXES) || !visible) return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Object	*object;
	Rect	 frame;
	HDC	 dc;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	switch (message)
	{
		case SM_CHECKOPTIONBOXES:
			if (state == True && *variable != code)
			{
				state = False;

				Paint(SP_PAINT);

				retVal = Break;
			}
			else if (state == False && *variable == code)
			{
				state = True;

				Paint(SP_PAINT);

				retVal = Break;
			}

			break;
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			if (objectProperties->checked)
			{
				objectProperties->clicked = True;

				Frame(dc, frame, FRAME_DOWN);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;
				objectProperties->checked = False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				if (*variable != code)
				{
					*variable = code;

					state = True;

					Paint(SP_PAINT);

					for (Int i = 0; i < Object::objectCount; i++)
					{
						object = mainObjectManager->RequestObject(i);

						if (object != NIL)
						{
							if (object->GetObjectType() == OBJ_OPTIONBOX) object->Process(SM_CHECKOPTIONBOXES, 0, 0);
						}
					}

					ProcCall(objectProperties->proc, objectProperties->procParam);
				}

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

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
				objectProperties->checked = True;
				Frame(dc, frame, FRAME_UP);
			}
			else if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

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

S::Int S::OptionBox::SetText(String newText)
{
	if (!registered || !visible)
	{
		objectProperties->text = newText;
		return Success;
	}

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Rect	 textRect;
	HDC	 dc = GetContext(wnd);

	textRect.left	= realPos.x + METRIC_OPTBOXOFFSETXY + 14;
	textRect.top	= realPos.y + METRIC_OPTBOXOFFSETXY - METRIC_OPTBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::BackgroundColor, objectProperties->fontWeight);

	objectProperties->text = newText;

	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return Success;
}
