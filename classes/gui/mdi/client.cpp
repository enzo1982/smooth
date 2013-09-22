 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/mdi/client.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/misc/binary.h>

const S::Short	 S::GUI::MDI::Client::classID = S::Object::RequestClassID();

S::GUI::MDI::Client::Client() : Widget(Point(), Size())
{
	type		= classID;
	orientation	= OR_CENTER;
}

S::GUI::MDI::Client::~Client()
{
}

S::Int S::GUI::MDI::Client::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface = GetDrawSurface();
	Rect	 client	 = Rect(GetRealPosition() + Point(2, 2), GetRealSize() - Size(3, 4));

	for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
	{
		Object	*object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Divider::classID)
			{
				if (((Widget *) object)->GetContainer() == container)
				{
					Divider	*db    = (Divider *) object;
					Int	 dbPos = Math::Round(db->GetPos() * surface->GetSurfaceDPI() / 96.0);

					if (Binary::IsFlagSet(db->GetFlags(), OR_VERT))
					{
						if	( Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && dbPos >= client.left   - 3) client.left	  = 			     dbPos + 5;
						else if (!Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && dbPos <= client.right  + 1) client.right  = container->GetWidth()  - dbPos - 2;
					}
					else if (Binary::IsFlagSet(db->GetFlags(), OR_HORZ))
					{
						if	( Binary::IsFlagSet(db->GetFlags(), OR_TOP)  && dbPos >= client.top    - 2) client.top	  = 			     dbPos + 5;
						else if (!Binary::IsFlagSet(db->GetFlags(), OR_TOP)  && dbPos <= client.bottom + 1) client.bottom = container->GetHeight() - dbPos - 2;
					}
				}
			}
		}
	}

	Rect	 updateRect = container->GetContainerWindow()->GetUpdateRect();

	switch (message)
	{
		case SP_PAINT:
			if (Rect::DoRectsOverlap(updateRect, client) || (updateRect.left == 0 && updateRect.top == 0 && updateRect.right == 0 && updateRect.bottom == 0))
			{
				updateRect.right  += 5;
				updateRect.bottom += 5;

				Rect	 intersectRect = Rect::OverlapRect(updateRect, client);

				surface->Box(intersectRect, Setup::DividerDarkColor, Rect::Filled);
				surface->Frame(client, FRAME_DOWN);
			}

			break;
	}

	return Success();
}
