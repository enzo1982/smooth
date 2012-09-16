 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short		 S::GUI::CheckBox::classID = S::Object::RequestClassID();

S::Signal0<S::Void>	 S::GUI::CheckBox::internalCheckValues;

S::GUI::CheckBox::CheckBox(const String &iText, const Point &iPos, const Size &iSize, Bool *iVariable) : Widget(iPos, iSize)
{
	type		= classID;
	text		= iText;

	dummyVariable	= False;

	if (iVariable == NIL)	variable = &dummyVariable;
	else			variable = iVariable;

	state		= *variable;

	font.SetColor(Setup::ClientTextColor);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(17);

	ComputeTextSize();

	internalCheckValues.Connect(&CheckBox::InternalCheckValues, this);

	hotspot	= new HotspotSimpleButton(Point(0, 0), GetSize());
	hotspot->onLeftButtonClick.Connect(&CheckBox::OnLeftButtonClick, this);
	hotspot->onLeftButtonClick.Connect(&onAction);

	onChangeSize.Connect(&HotspotSimpleButton::SetSize, hotspot);

	Add(hotspot);
}

S::GUI::CheckBox::~CheckBox()
{
	DeleteObject(hotspot);

	internalCheckValues.Disconnect(&CheckBox::InternalCheckValues, this);
}

S::Int S::GUI::CheckBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Font	 nFont = font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->Box(frame, GetBackgroundColor(), Rect::Filled);
				surface->SetText(text, frame + Point(frame.GetHeight(), 2), nFont);
			}

			/* Fall through to SP_UPDATE here.
			 */

		case SP_UPDATE:
			{
				Rect	 valueFrame = Rect(GetRealPosition() + Point(3, 3) * surface->GetSurfaceDPI() / 96.0, Size(frame.GetHeight(), frame.GetHeight()) - (Size(3, 3) * surface->GetSurfaceDPI() / 96.0) * 2);

				if (IsActive())	surface->Box(valueFrame, Setup::ClientColor, Rect::Filled);
				else		surface->Box(valueFrame, Setup::BackgroundColor, Rect::Filled);

				surface->Frame(valueFrame, FRAME_DOWN);

				if (*variable == True)
				{
					Point p1 = Point(valueFrame.left + 3 - (IsRightToLeft() ? 1 : 0), valueFrame.top + 3);
					Point p2 = Point(valueFrame.right - 1 - (IsRightToLeft() ? 1 : 0), valueFrame.bottom - 1);

					for (Int i = 0; i < 2; i++)
					{
						Int	 color = IsActive() ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);

						if (i == 1)
						{
							color = IsActive() ? Setup::ClientTextColor : Setup::GrayTextColor;

							p1 -= Point((IsRightToLeft() ? -i : i), i);
							p2 -= Point((IsRightToLeft() ? -i : i), i);
						}

						surface->Line(p1 + Point(0, 0),				p2 - Point(0, 0),			  color);
						surface->Line(p1 + Point(1, 0),				p2 - Point(0, 1),			  color);
						surface->Line(p1 + Point(0, 1),				p2 - Point(1, 0),			  color);
						surface->Line(p1 + Point(valueFrame.GetWidth() - 5, 0), p2 - Point(valueFrame.GetWidth() - 3, 0), color);
						surface->Line(p1 + Point(valueFrame.GetWidth() - 5, 1), p2 - Point(valueFrame.GetWidth() - 4, 0), color);
						surface->Line(p1 + Point(valueFrame.GetWidth() - 6, 0), p2 - Point(valueFrame.GetWidth() - 3, 1), color);
					}
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::CheckBox::SetChecked(Bool newValue)
{
	*variable = newValue;

	if (*variable != state) internalCheckValues.Emit();

	return Success();
}

S::Bool S::GUI::CheckBox::IsChecked()
{
	return *variable;
}

S::Void S::GUI::CheckBox::OnLeftButtonClick()
{
	if (*variable == False)	*variable = True;
	else			*variable = False;

	internalCheckValues.Emit();
}

S::Void S::GUI::CheckBox::InternalCheckValues()
{
	if (state != *variable)
	{
		state = *variable;

		Paint(SP_UPDATE);
	}
}
