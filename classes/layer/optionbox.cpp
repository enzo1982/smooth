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
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_OPTIONBOX = S::Object::RequestObjectID();

S::GUI::OptionBox::OptionBox(String text, Point pos, Size size, Int *var, Int iCode)
{
	type				= OBJ_OPTIONBOX;
	objectProperties->text		= text;
	variable			= var;
	code				= iCode;
	objectProperties->fontColor	= Setup::ClientTextColor;

	if (*variable == code)	state = True;
	else			state = False;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(16 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);

	GetTextSize();
}

S::GUI::OptionBox::~OptionBox()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::OptionBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 textRect;
	Point	 lineStart;
	Point	 lineEnd;
	Int	 lightColor;

	if (active)	lightColor = Setup::ClientColor;
	else		lightColor = Setup::BackgroundColor;

	lineStart.x = realPos.x + METRIC_OPTBOXOFFSETXY + 3;
	lineStart.y = realPos.y + METRIC_OPTBOXOFFSETXY;
	lineEnd.x = lineStart.x + 5;
	lineEnd.y = lineStart.y;

	surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

	lineStart.x--;
	lineStart.y++;
	lineEnd.y++;

	surface->Line(lineStart, lineEnd, lightColor);

	surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);

	lineStart.x--;
	lineStart.y++;
	lineEnd.x += 2;
	lineEnd.y++;

	surface->Line(lineStart, lineEnd, lightColor);

	surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);
	surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

	lineStart.x--;
	lineEnd.x++;

	for (Int i = 0; i < 5; i++)
	{
		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, lightColor);

		surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);
		surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);
	}

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	surface->Line(lineStart, lineEnd, lightColor);

	surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerLightColor);
	surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	surface->Line(lineStart, lineEnd, lightColor);

	surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerLightColor);
	surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

	lineStart.x++;
	lineStart.y++;
	lineEnd.x--;
	lineEnd.y++;

	surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

	if (*variable == code)
	{
		lineStart.x++;
		lineStart.y -= 7;
		lineEnd.x--;
		lineEnd.y -= 7;

		surface->Line(lineStart, lineEnd, Setup::ClientTextColor);

		lineStart.x--;
		lineEnd.x++;

		for (Int i = 0; i < 3; i++)
		{
			lineStart.y++;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, Setup::ClientTextColor);
		}

		surface->SetPixel(lineEnd.x, lineEnd.y - 1, Setup::DividerDarkColor);
		surface->SetPixel(lineEnd.x, lineEnd.y, Setup::DividerDarkColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, Setup::ClientTextColor);

		surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerDarkColor);
		surface->SetPixel(lineEnd.x, lineEnd.y, Setup::DividerDarkColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);
	}

	textRect.left	= realPos.x + METRIC_OPTBOXOFFSETXY + 14;
	textRect.top	= realPos.y + METRIC_OPTBOXOFFSETXY - METRIC_OPTBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	return Success;
}

S::Int S::GUI::OptionBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered) return Error;
	if ((!active && message != SM_CHECKOPTIONBOXES) || !visible) return Success;

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
							if (object->GetObjectType() == OBJ_OPTIONBOX) ((OptionBox *) object)->Process(SM_CHECKOPTIONBOXES, 0, 0);
						}
					}

					onClick.Emit(wnd->MouseX(), wnd->MouseY());
				}

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
		case SM_MOUSELEAVE:
			if (message == SM_MOUSEMOVE && !objectProperties->checked && wnd->IsMouseOn(frame))
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

S::Int S::GUI::OptionBox::SetText(String newText)
{
	if (!registered || !visible)
	{
		objectProperties->text = newText;
		return Success;
	}

	Surface	*surface = myContainer->GetDrawSurface();

	Point	 realPos = GetRealPosition();
	Rect	 textRect;

	textRect.left	= realPos.x + METRIC_OPTBOXOFFSETXY + 14;
	textRect.top	= realPos.y + METRIC_OPTBOXOFFSETXY - METRIC_OPTBOXTEXTOFFSETY;
	textRect.right	= textRect.left + objectProperties->size.cx;
	textRect.bottom	= textRect.top + 20;

	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, Setup::BackgroundColor, objectProperties->fontWeight);

	objectProperties->text = newText;

	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	return Success;
}
