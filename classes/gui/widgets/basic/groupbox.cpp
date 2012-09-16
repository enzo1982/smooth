 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
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

const S::Short	 S::GUI::GroupBox::classID = S::Object::RequestClassID();

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
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->Frame(frame, FRAME_DOWN);
				surface->Frame(frame + Point(1, 1) - Size(2, 2), FRAME_UP);

				Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6) * surface->GetSurfaceDPI() / 96.0, Size(scaledTextSize.cx, Math::Round(scaledTextSize.cy * 1.2)) + Size(3, 0) * surface->GetSurfaceDPI() / 96.0);

				surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

				Font	 nFont		= font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect + Point(1, 0) * surface->GetSurfaceDPI() / 96.0, nFont);
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

	Surface	*surface	= GetDrawSurface();
	Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6) * surface->GetSurfaceDPI() / 96.0, Size(scaledTextSize.cx, Math::Round(scaledTextSize.cy * 1.2)) + Size(3, 0) * surface->GetSurfaceDPI() / 96.0);

	surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

	surface->SetText(text, textRect + Point(1, 0) * surface->GetSurfaceDPI() / 96.0, font);

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		widget->Paint(SP_PAINT);
	}

	return Success();
}

S::Int S::GUI::GroupBox::Deactivate()
{
	active = False;

	if (!IsRegistered())	return Success();
	if (!IsVisible())	return Success();

	Surface	*surface	= GetDrawSurface();
	Rect	 textRect	= Rect(GetRealPosition() + Point(10, -6) * surface->GetSurfaceDPI() / 96.0, Size(scaledTextSize.cx, Math::Round(scaledTextSize.cy * 1.2)) + Size(3, 0) * surface->GetSurfaceDPI() / 96.0);

	surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

	Font	 nFont		= font;

	nFont.SetColor(Setup::GrayTextColor);

	surface->SetText(text, textRect + Point(1, 0) * surface->GetSurfaceDPI() / 96.0, nFont);

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		widget->Paint(SP_PAINT);
	}

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
		Surface	*surface = GetDrawSurface();

		surface->Box(Rect(GetRealPosition() - Point(0, 6) * surface->GetSurfaceDPI() / 96.0, GetRealSize() + Size(0, 6) * surface->GetSurfaceDPI() / 96.0), Setup::BackgroundColor, Rect::Filled);
	}

	return Layer::Hide();
}
