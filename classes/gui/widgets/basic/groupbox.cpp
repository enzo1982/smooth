 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/groupbox.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::GroupBox::classID = S::Object::RequestClassID();

S::GUI::GroupBox::GroupBox(String iText, Point iPos, Size iSize)
{
	type	= classID;
	text	= iText;

	possibleContainers.AddEntry(Layer::classID);

	pos	= iPos;
	size	= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 80;

	GetTextSize();
}

S::GUI::GroupBox::~GroupBox()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::GroupBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - 1;
	frame.bottom	= realPos.y + size.cy - 1;

	surface->Frame(frame, FRAME_DOWN);

	frame.left++;
	frame.top++;
	frame.right--;
	frame.bottom--;

	surface->Frame(frame, FRAME_UP);

	textRect.left	= frame.left + 9;
	textRect.top	= frame.top - 7;
	textRect.right	= textRect.left + textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	Font	 nFont = font;

	if (!active) nFont.SetColor(Setup::GrayTextColor);

	surface->SetText(text, textRect, nFont);

	return Success;
}

S::Int S::GUI::GroupBox::Activate()
{
	active = True;

	if (!registered)	return Success;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - 6;
	textRect.right	= textRect.left + textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	surface->SetText(text, textRect, font);

	return Success;
}

S::Int S::GUI::GroupBox::Deactivate()
{
	active = False;

	if (!registered)	return Success;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	textRect.left	= realPos.x + 10;
	textRect.top	= realPos.y - 6;
	textRect.right	= textRect.left + textSize.cx + 3;
	textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	textRect.left++;

	Font	 nFont = font;

	nFont.SetColor(Setup::GrayTextColor);

	surface->SetText(text, textRect, nFont);

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
		Surface	*surface = container->GetDrawSurface();

		rect.left	= realPos.x + 10;
		rect.top	= realPos.y - 6;
		rect.right	= rect.left + textSize.cx + 3;
		rect.bottom	= rect.top + Math::Round(textSize.cy * 1.2);

		surface->Box(rect, Setup::BackgroundColor, FILLED);

		rect.left	= realPos.x;
		rect.top	= realPos.y;
		rect.right	= realPos.x + size.cx + 1;
		rect.bottom	= realPos.y + size.cy + 1;

		surface->Box(rect, Setup::BackgroundColor, FILLED);
	}

	return Success;
}
