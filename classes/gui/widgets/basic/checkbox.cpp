 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::CheckBox::classID = S::Object::RequestClassID();

S::GUI::CheckBox::CheckBox(String iText, Point iPos, Size iSize, Bool *var)
{
	type		= classID;
	text		= iText;
	variable	= var;
	state		= *variable;

	font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 16;

	GetTextSize();
}

S::GUI::CheckBox::~CheckBox()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::CheckBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Rect	 textRect;
	Point	 lineStart;
	Point	 lineEnd;
	Int	 shadowColor;
	Int	 crossColor;

	frame.left	= realPos.x + 3;
	frame.top	= realPos.y + 3;
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
		lineStart.x = frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 4 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.right - 1 - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 1 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2 - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 1 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 4;
		lineEnd.x = frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.right - 2 - (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2 - (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, shadowColor);

		lineStart.x = frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.left + 3 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.right - 1 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 2;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.right - 2 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 2 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 2 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 3;
		lineEnd.x = frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 1;

		surface->Line(lineStart, lineEnd, crossColor);

		lineStart.x = frame.right - 3 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = frame.top + 2;
		lineEnd.x = frame.left + 1 + (Setup::rightToLeft ? 1 : 0);
		lineEnd.y = frame.bottom - 2;

		surface->Line(lineStart, lineEnd, crossColor);
	}

	textRect.left	= frame.right + 4;
	textRect.top	= frame.top - 1;
	textRect.right	= textRect.left + size.cx;
	textRect.bottom	= textRect.top + 20;

	Font	 nFont = font;

	if (!active) nFont.SetColor(Setup::GrayTextColor);

	surface->SetText(text, textRect, nFont);

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= frame.left + size.cx;
	frame.bottom	= frame.top + size.cy;

	if (checked) surface->Frame(frame, FRAME_UP);

	return Success;
}

S::Int S::GUI::CheckBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered) return Failure;
	if ((!active && message != SM_CHECKCHECKBOXES) || !visible) return Success;

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
		case SM_CHECKCHECKBOXES:
			if (state != *variable)
			{
				state = *variable;

				Paint(SP_PAINT);

				retVal = Break;
			}

			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
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

				if (*variable == False)	*variable = True;
				else			*variable = False;

				state = *variable;

				Paint(SP_PAINT);

				Process(SM_MOUSEMOVE, 0, 0);

				for (Int i = 0; i < Object::GetNOfObjects(); i++)
				{
					object = Object::GetNthObject(i);

					if (object != NIL)
					{
						if (object->GetObjectType() == classID) ((CheckBox *) object)->Process(SM_CHECKCHECKBOXES, 0, 0);
					}
				}

				onClick.Emit(wnd->MouseX(), wnd->MouseY());

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
