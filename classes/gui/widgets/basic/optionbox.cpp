 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::OptionBox::classID = S::Object::RequestClassID();

S::GUI::OptionBox::OptionBox(String iText, Point iPos, Size iSize, Int *var, Int iCode)
{
	type		= classID;
	text		= iText;
	variable	= var;
	code		= iCode;

	font.SetColor(Setup::ClientTextColor);

	if (*variable == code)	state = True;
	else			state = False;

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 16;

	GetTextSize();
}

S::GUI::OptionBox::~OptionBox()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::OptionBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 textRect;
	Point	 lineStart;
	Point	 lineEnd;
	Int	 lightColor;

	if (active)	lightColor = Setup::ClientColor;
	else		lightColor = Setup::BackgroundColor;

	lineStart.x = realPos.x + 6;
	lineStart.y = realPos.y + 3;
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

	textRect.left	= realPos.x + 17;
	textRect.top	= realPos.y + 2;
	textRect.right	= textRect.left + size.cx;
	textRect.bottom	= textRect.top + 20;

	surface->SetText(text, textRect, font);

	return Success;
}

S::Int S::GUI::OptionBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered) return Failure;
	if ((!active && message != SM_CHECKOPTIONBOXES) || !visible) return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Object	*object;
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx;
	frame.bottom	= realPos.y + size.cy;

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
			if (checked)
			{
				clicked = True;

				surface->Frame(frame, FRAME_DOWN);

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (clicked)
			{
				clicked = False;
				checked = False;

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

					for (Int i = 0; i < Object::GetNOfObjects(); i++)
					{
						object = Object::GetNthObject(i);

						if (object != NIL)
						{
							if (object->GetObjectType() == classID) ((OptionBox *) object)->Process(SM_CHECKOPTIONBOXES, 0, 0);
						}
					}

					onClick.Emit(wnd->MouseX(), wnd->MouseY());
				}

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			if (!checked && wnd->IsMouseOn(frame))
			{
				checked = True;
				surface->Frame(frame, FRAME_UP);
			}
			else if (checked && !wnd->IsMouseOn(frame))
			{
				checked = False;
				clicked = False;

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

S::Int S::GUI::OptionBox::SetText(const String &newText)
{
	if (!registered || !visible)
	{
		text = newText;

		return Success;
	}

	Surface	*surface = container->GetDrawSurface();

	Point	 realPos = GetRealPosition();
	Rect	 textRect;

	textRect.left	= realPos.x + 17;
	textRect.top	= realPos.y + 4;
	textRect.right	= textRect.left + size.cx;
	textRect.bottom	= textRect.top + 20;

	Font	 nFont = font;

	nFont.SetColor(Setup::BackgroundColor);

	surface->SetText(text, textRect, nFont);

	text = newText;

	surface->SetText(text, textRect, font);

	return Success;
}
