 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/divider.h>
#include <smooth/objectmanager.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/binary.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Divider::classID = S::Object::RequestClassID();

S::GUI::Divider::Divider(Int pos, Int iOrientation)
{
	type				= classID;
	flags				= iOrientation;
	objectProperties->orientation	= OR_FREE;

	if (pos == 0)	pos = Math::Round(120 * Setup::FontSize);
	else		pos = Math::Round(pos * Setup::FontSize);

	possibleContainers.AddEntry(Window::classID);
	possibleContainers.AddEntry(Layer::classID);

	if (Binary::IsFlagSet(flags, OR_HORZ))		{ objectProperties->pos.x = 0; objectProperties->pos.y = pos; }
	else if (Binary::IsFlagSet(flags, OR_VERT))	{ objectProperties->pos.x = pos; objectProperties->pos.y = 0; }
}

S::GUI::Divider::~Divider()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Divider::SetPos(Int pos)
{
	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	if (Binary::IsFlagSet(flags, OR_HORZ))		objectProperties->pos.y = pos;
	else if (Binary::IsFlagSet(flags, OR_VERT))	objectProperties->pos.x = pos;

	if (registered && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Divider::GetPos()
{
	if (Binary::IsFlagSet(flags, OR_HORZ))		return objectProperties->pos.y;
	else if (Binary::IsFlagSet(flags, OR_VERT))	return objectProperties->pos.x;

	return 0;
}

S::Int S::GUI::Divider::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Object	*container = NIL;
	Window	*wnd = NIL;
	Layer	*layer = NIL;

	if (myContainer->GetContainerObject()->GetObjectType() == Window::classID)
	{
		wnd = (Window *) myContainer->GetContainerObject();

		container = wnd;
	}
	else if (myContainer->GetContainerObject()->GetObjectType() == Layer::classID)
	{
		layer = (Layer *) myContainer->GetContainerObject();
		wnd = (Window *) layer->GetContainer()->GetContainerObject();

		container = layer;
	}

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Object	*object;
	Divider	*operat;
	Point	 doubleBar1;
	Point	 doubleBar2;
	Bool	 afterMe = False;

	if (Binary::IsFlagSet(flags, OR_HORZ))
	{
		if (container->GetObjectType() == Window::classID)
		{
			doubleBar1.x = 4;

			if (Binary::IsFlagSet(flags, OR_TOP))		doubleBar1.y = objectProperties->pos.y;
			else if	(Binary::IsFlagSet(flags, OR_BOTTOM))	doubleBar1.y = wnd->GetObjectProperties()->size.cy - objectProperties->pos.y;

			doubleBar2.x = wnd->GetObjectProperties()->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (Int i = mainObjectManager->GetNOfObjects() - 1; i >= 0; i--)
			{
				object = mainObjectManager->GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID && object->GetContainer() == myContainer)
					{
						operat = (Divider *) object;

						if (afterMe && Binary::IsFlagSet(operat->flags, OR_VERT))
						{
							if (Binary::IsFlagSet(operat->flags, OR_LEFT))
							{
								if (operat->GetObjectProperties()->pos.x >= doubleBar1.x - 2) doubleBar1.x = operat->GetObjectProperties()->pos.x + 3;
							}
							else
							{
								if (operat->GetObjectProperties()->pos.x <= doubleBar2.x+1) doubleBar2.x = wnd->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x - 2;
							}
						}

						if (operat == this) afterMe = True;
					}
				}
			}
		}
		else if (container->GetObjectType() == Layer::classID)
		{
			doubleBar1.x = layer->GetRealPosition().x + 4;

			if (Binary::IsFlagSet(flags, OR_TOP))		doubleBar1.y = layer->GetRealPosition().y + objectProperties->pos.y;
			else if	(Binary::IsFlagSet(flags, OR_BOTTOM))	doubleBar1.y = layer->GetRealPosition().y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;

			doubleBar2.x = layer->GetRealPosition().x + layer->GetObjectProperties()->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (Int i = mainObjectManager->GetNOfObjects() - 1; i >= 0; i--)
			{
				object = mainObjectManager->GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID && object->GetContainer() == myContainer)
					{
						operat = (Divider *) object;

						if (afterMe && Binary::IsFlagSet(operat->flags, OR_VERT))
						{
							if (Binary::IsFlagSet(operat->flags, OR_LEFT))
							{
								if (layer->GetRealPosition().x + operat->GetObjectProperties()->pos.x >= doubleBar1.x-2) doubleBar1.x = layer->GetRealPosition().x + operat->GetObjectProperties()->pos.x + 3;
							}
							else
							{
								if (layer->GetRealPosition().x + layer->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x <= doubleBar2.x+1) doubleBar2.x = layer->GetRealPosition().x + layer->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x - 2;
							}
						}

						if (operat == this) afterMe = True;
					}
				}
			}
		}

		objectProperties->size.cx = doubleBar2.x - doubleBar1.x + 4;
		objectProperties->size.cy = 2;

		surface->Bar(doubleBar1, doubleBar2, OR_HORZ);
	}
	else if (Binary::IsFlagSet(flags, OR_VERT))
	{
		if (container->GetObjectType() == Window::classID)
		{
			if (Binary::IsFlagSet(flags, OR_LEFT))		doubleBar1.x = objectProperties->pos.x;
			else if	(Binary::IsFlagSet(flags, OR_RIGHT))	doubleBar1.x = wnd->GetObjectProperties()->size.cx - objectProperties->pos.x;

			doubleBar1.y = wnd->GetClientRect().top;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = wnd->GetObjectProperties()->size.cy - wnd->GetClientRect().bottom - 2;

			for (Int i = mainObjectManager->GetNOfObjects() - 1; i >= 0; i--)
			{
				object = mainObjectManager->GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID && object->GetContainer() == myContainer)
					{
						operat = (Divider *) object;

						if (afterMe && Binary::IsFlagSet(operat->flags, OR_HORZ))
						{
							if (Binary::IsFlagSet(operat->flags, OR_TOP))
							{
								if (operat->GetObjectProperties()->pos.y >= doubleBar1.y-2) doubleBar1.y = operat->GetObjectProperties()->pos.y + 3;
							}
							else
							{
								if (operat->GetObjectProperties()->pos.y <= doubleBar2.y+1) doubleBar2.y = wnd->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y - 2;
							}
						}

						if (operat == this) afterMe = True;
					}
				}
			}
		}
		else if (container->GetObjectType() == Layer::classID)
		{
			if (Binary::IsFlagSet(flags, OR_LEFT))		doubleBar1.x = layer->GetRealPosition().x + objectProperties->pos.x;
			else if (Binary::IsFlagSet(flags, OR_RIGHT))	doubleBar1.x = layer->GetRealPosition().x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;

			doubleBar1.y = layer->GetRealPosition().y + 4;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = layer->GetRealPosition().y + layer->GetObjectProperties()->size.cy - 4;

			for (Int i = mainObjectManager->GetNOfObjects() - 1; i >= 0; i--)
			{
				object = mainObjectManager->GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID && object->GetContainer() == myContainer)
					{
						operat = (Divider *) object;

						if (afterMe && Binary::IsFlagSet(operat->flags, OR_HORZ))
						{
							if (Binary::IsFlagSet(operat->flags, OR_TOP))
							{
								if (layer->GetRealPosition().y + operat->GetObjectProperties()->pos.y >= doubleBar1.y-2) doubleBar1.y = layer->GetRealPosition().y + operat->GetObjectProperties()->pos.y + 3;
							}
							else
							{
								if (layer->GetRealPosition().y + layer->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y <= doubleBar2.y+1) doubleBar2.y = layer->GetRealPosition().y + layer->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y - 2;
							}
						}

						if (operat == this) afterMe = True;
					}
				}
			}
		}

		objectProperties->size.cy = doubleBar2.y - doubleBar1.y + 4;
		objectProperties->size.cx = 2;

		surface->Bar(doubleBar1, doubleBar2, OR_VERT);
	}

	return Success;
}
