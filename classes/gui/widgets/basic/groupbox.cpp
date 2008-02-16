 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/groupbox.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::GroupBox::classID = S::Object::RequestClassID();

S::GUI::GroupBox::GroupBox(const String &iText, const Point &iPos, const Size &iSize) : Layer(iText)
{
	type		= classID;
	orientation	= OR_UPPERLEFT;

	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(80);

	ComputeTextSize();
}

S::GUI::GroupBox::~GroupBox()
{
}

S::Int S::GUI::GroupBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = container->GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetSize());

				surface->Frame(frame, FRAME_DOWN);
				surface->Frame(frame + Point(1, 1) - Size(2, 2), FRAME_UP);

				Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6), Size(textSize.cx + 3, Math::Round(textSize.cy * 1.2)));

				surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

				Font	 nFont		= font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect + Point(1, 0), nFont);
			}

			break;
	}

	return Layer::Paint(message);
}

S::Int S::GUI::GroupBox::Activate()
{
	active = True;

	if (!IsRegistered())	return Success();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6), Size(textSize.cx + 3, Math::Round(textSize.cy * 1.2)));

	surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

	surface->SetText(text, textRect + Point(1, 0), font);

	return Success();
}

S::Int S::GUI::GroupBox::Deactivate()
{
	active = False;

	if (!IsRegistered())	return Success();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6), Size(textSize.cx + 3, Math::Round(textSize.cy * 1.2)));

	surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

	Font	 nFont		= font;

	nFont.SetColor(Setup::GrayTextColor);

	surface->SetText(text, textRect + Point(1, 0), nFont);

	return Success();
}

S::Int S::GUI::GroupBox::Show()
{
	Int	 retVal = Layer::Show();

	Paint(SP_SHOW);

	return retVal;
}

S::Int S::GUI::GroupBox::Hide()
{
	if (IsRegistered() && IsVisible())
	{
		Surface	*surface = container->GetDrawSurface();

		surface->Box(Rect(GetRealPosition() - Point(0, 6), GetSize() + Size(0, 6)), Setup::BackgroundColor, Rect::Filled);
	}

	return Layer::Hide();
}
