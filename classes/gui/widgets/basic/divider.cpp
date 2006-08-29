 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

S::Int S::GUI::Divider::GetPos()
{
	return position;
}

S::Int S::GUI::Divider::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*wnd	 = container->GetContainerWindow();
	Surface	*surface = container->GetDrawSurface();
	Point	 doubleBar1;
	Point	 doubleBar2;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (Binary::IsFlagSet(flags, OR_VERT))
			{
				Bool	 afterMe = False;

				if (Binary::IsFlagSet(flags, OR_RIGHT))	doubleBar1.x = container->GetRealPosition().x + container->GetWidth() - position;
				else					doubleBar1.x = container->GetRealPosition().x + position;

				doubleBar2.x = doubleBar1.x;

				if (container->GetObjectType() == Window::classID)
				{
					doubleBar1.y = wnd->GetClientRect().top;
					doubleBar2.y = wnd->GetClientRect().bottom;

				}
				else
				{
					doubleBar1.y = container->GetRealPosition().y + 4;
					doubleBar2.y = container->GetRealPosition().y + container->GetHeight() - 4;
				}

				for (Int i = container->GetNOfObjects() - 1; i >= 0; i--)
				{
					if (container->GetNthObject(i)->GetObjectType() != classID) continue;

					Divider	*divider = (Divider *) container->GetNthObject(i);

					if (afterMe && !Binary::IsFlagSet(divider->flags, OR_VERT))
					{
						if (Binary::IsFlagSet(divider->flags, OR_BOTTOM))
						{
							if (container->GetRealPosition().y + container->GetHeight() - divider->position <= doubleBar2.y + 1) doubleBar2.y = container->GetRealPosition().y + container->GetHeight() - divider->position - 2;
						}
						else
						{
							if (container->GetRealPosition().y + divider->position >= doubleBar1.y - 2) doubleBar1.y = container->GetRealPosition().y + divider->position + 3;
						}
					}

					if (divider == this) afterMe = True;
				}
			}
			else
			{
				Bool	 afterMe = False;

				if (Binary::IsFlagSet(flags, OR_BOTTOM)) doubleBar1.y = container->GetRealPosition().y + container->GetHeight() - position;
				else					 doubleBar1.y = container->GetRealPosition().y + position;

				doubleBar2.y = doubleBar1.y;

				if (container->GetObjectType() == Window::classID)
				{
					doubleBar1.x = wnd->GetClientRect().left + 1;
					doubleBar2.x = wnd->GetClientRect().right - 1;
				}
				else
				{
					doubleBar1.x = container->GetRealPosition().x + 4;
					doubleBar2.x = container->GetRealPosition().x + container->GetWidth() - 4;
				}

				for (Int i = container->GetNOfObjects() - 1; i >= 0; i--)
				{
					if (container->GetNthObject(i)->GetObjectType() != classID) continue;

					Divider	*divider = (Divider *) container->GetNthObject(i);

					if (afterMe && Binary::IsFlagSet(divider->flags, OR_VERT))
					{
						if (Binary::IsFlagSet(divider->flags, OR_RIGHT))
						{
							if (container->GetRealPosition().x + container->GetWidth() - divider->position <= doubleBar2.x + 1) doubleBar2.x = container->GetRealPosition().x + container->GetWidth() - divider->position - 2;
						}
						else
						{
							if (container->GetRealPosition().x + divider->position >= doubleBar1.x - 2) doubleBar1.x = container->GetRealPosition().x + divider->position + 3;
						}
					}

					if (divider == this) afterMe = True;
				}
			}

			surface->Bar(doubleBar1, doubleBar2, Binary::IsFlagSet(flags, OR_VERT) ? OR_VERT : OR_HORZ);

			break;
	}

	return Success();
}

S::Bool S::GUI::Divider::IsAffected(const Rect &uRect) const
{
	return True;
}
