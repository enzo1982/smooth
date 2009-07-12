 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Divider::classID = S::Object::RequestClassID();

S::GUI::Divider::Divider(Int iPosition, Int iOrientation) : Widget(Point(), Size())
{
	type		= classID;
	flags		= iOrientation;
	orientation	= OR_FREE;
	position	= iPosition;

	if (position == 0) position = 120;
}

S::GUI::Divider::~Divider()
{
}

S::Int S::GUI::Divider::SetPos(Int nPosition)
{
	Bool	 prevVisible = IsVisible();

	if (IsRegistered() && prevVisible) Hide();

	position = nPosition;

	if (IsRegistered() && prevVisible) Show();

	return Success();
}

S::Int S::GUI::Divider::GetPos() const
{
	return position;
}

S::Int S::GUI::Divider::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 rect = GetDividerRect();

				if (Binary::IsFlagSet(flags, OR_VERT))	surface->Bar(Point(rect.left, rect.top), Point(rect.right - 1, rect.bottom), OR_VERT);
				else					surface->Bar(Point(rect.left, rect.top), Point(rect.right, rect.bottom - 1), OR_HORZ);
			}

			break;
	}

	return Success();
}

S::Bool S::GUI::Divider::IsAffected(const Rect &uRect) const
{
	return Rect::DoRectsOverlap(uRect, GetDividerRect());
}

S::GUI::Rect S::GUI::Divider::GetDividerRect() const
{
	Window	*wnd = GetContainerWindow();
	Rect	 rect;

	if (wnd == NIL) return rect;

	if (Binary::IsFlagSet(flags, OR_VERT))
	{
		Bool	 afterMe = False;

		if (container->GetObjectType() == Window::classID)
		{
			if (Binary::IsFlagSet(flags, OR_RIGHT))	rect.left = ((Window *) container)->GetClientRect().right - position;
			else					rect.left = ((Window *) container)->GetClientRect().left + position;

			rect.top	= wnd->GetClientRect().top;
			rect.bottom	= wnd->GetClientRect().bottom;
		}
		else
		{
			if (Binary::IsFlagSet(flags, OR_RIGHT))	rect.left = container->GetRealPosition().x + container->GetWidth() - position;
			else					rect.left = container->GetRealPosition().x + position;

			rect.top	= container->GetRealPosition().y + 3;
			rect.bottom	= container->GetRealPosition().y + container->GetHeight() - 3;
		}

		rect.right = rect.left + 1;

		for (Int i = container->GetNOfObjects() - 1; i >= 0; i--)
		{
			if (container->GetNthObject(i)->GetObjectType() != classID) continue;

			Divider	*divider = (Divider *) container->GetNthObject(i);

			if (afterMe && !Binary::IsFlagSet(divider->flags, OR_VERT))
			{
				if (Binary::IsFlagSet(divider->flags, OR_BOTTOM))
				{
					if (container->GetRealPosition().y + container->GetHeight() - divider->position <= rect.bottom + 1) rect.bottom = container->GetRealPosition().y + container->GetHeight() - divider->position - 2;
				}
				else
				{
					if (container->GetRealPosition().y + divider->position >= rect.top - 2) rect.top = container->GetRealPosition().y + divider->position + 3;
				}
			}

			if (divider == this) afterMe = True;
		}
	}
	else
	{
		Bool	 afterMe = False;

		if (container->GetObjectType() == Window::classID)
		{
			if (Binary::IsFlagSet(flags, OR_BOTTOM)) rect.top = ((Window *) container)->GetClientRect().bottom - position;
			else					 rect.top = ((Window *) container)->GetClientRect().top + position;

			rect.left	= wnd->GetClientRect().left;
			rect.right	= wnd->GetClientRect().right;
		}
		else
		{
			if (Binary::IsFlagSet(flags, OR_BOTTOM)) rect.top = container->GetRealPosition().y + container->GetHeight() - position;
			else					 rect.top = container->GetRealPosition().y + position;

			rect.left	= container->GetRealPosition().x + 3;
			rect.right	= container->GetRealPosition().x + container->GetWidth() - 3;
		}

		rect.bottom = rect.top + 1;

		for (Int i = container->GetNOfObjects() - 1; i >= 0; i--)
		{
			if (container->GetNthObject(i)->GetObjectType() != classID) continue;

			Divider	*divider = (Divider *) container->GetNthObject(i);

			if (afterMe && Binary::IsFlagSet(divider->flags, OR_VERT))
			{
				if (Binary::IsFlagSet(divider->flags, OR_RIGHT))
				{
					if (container->GetRealPosition().x + container->GetWidth() - divider->position <= rect.right + 1) rect.right = container->GetRealPosition().x + container->GetWidth() - divider->position - 2;
				}
				else
				{
					if (container->GetRealPosition().x + divider->position >= rect.left - 2) rect.left = container->GetRealPosition().x + divider->position + 3;
				}
			}

			if (divider == this) afterMe = True;
		}
	}

	return rect;
}
