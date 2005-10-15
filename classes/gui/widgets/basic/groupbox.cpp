 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/groupbox.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::GroupBox::classID = S::Object::RequestClassID();

S::GUI::GroupBox::GroupBox(String iText, Point iPos, Size iSize)
{
	type	= classID;
	text	= iText;

	possibleContainers.RemoveAll();
	possibleContainers.AddEntry(Layer::classID);

	pos	= iPos;
	size	= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 80;

	GetTextSize();
}

S::GUI::GroupBox::~GroupBox()
{
}

S::Int S::GUI::GroupBox::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), size);

	surface->Frame(frame, FRAME_DOWN);

	frame.left++;
	frame.top++;
	frame.right--;
	frame.bottom--;

	surface->Frame(frame, FRAME_UP);

	Rect	 textRect;

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

	if (!IsRegistered())	return Success;
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

	if (!IsRegistered())	return Success;
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
	if (IsRegistered() && IsVisible())
	{
		Surface	*surface = container->GetDrawSurface();

		surface->Box(Rect(GetRealPosition() + Point(10, -6), Size(textSize.cx + 2, (Int) (textSize.cy * 1.2))), Setup::BackgroundColor, FILLED);
	}

	return Layer::Hide();
}
