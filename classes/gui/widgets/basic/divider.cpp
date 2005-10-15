 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/misc/binary.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Divider::classID = S::Object::RequestClassID();

S::GUI::Divider::Divider(Int position, Int iOrientation)
{
	type		= classID;
	flags		= iOrientation;
	orientation	= OR_FREE;

	if (position == 0) position = 120;

	possibleContainers.AddEntry(Window::classID);
	possibleContainers.AddEntry(Layer::classID);

	if (Binary::IsFlagSet(flags, OR_HORZ))		{ pos.x = 0; pos.y = position; }
	else if (Binary::IsFlagSet(flags, OR_VERT))	{ pos.x = position; pos.y = 0; }
}

S::GUI::Divider::~Divider()
{
}

S::Int S::GUI::Divider::SetPos(Int position)
{
	Bool	 prevVisible = IsVisible();

	if (IsRegistered() && IsVisible()) Hide();

	if (Binary::IsFlagSet(flags, OR_HORZ))		pos.y = position;
	else if (Binary::IsFlagSet(flags, OR_VERT))	pos.x = position;

	if (IsRegistered() && prevVisible) Show();

	return Success;
}

S::Int S::GUI::Divider::GetPos()
{
	if (Binary::IsFlagSet(flags, OR_HORZ))		return pos.y;
	else if (Binary::IsFlagSet(flags, OR_VERT))	return pos.x;

	return 0;
}

S::Int S::GUI::Divider::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	if (message != SP_PAINT) return Success;

	Surface	*surface = container->GetDrawSurface();
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

			if (Binary::IsFlagSet(flags, OR_TOP))		doubleBar1.y = pos.y;
			else if	(Binary::IsFlagSet(flags, OR_BOTTOM))	doubleBar1.y = wnd->size.cy - pos.y;

			doubleBar2.x = wnd->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
			{
				object = Object::GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID)
					{
						if (((Widget *) object)->GetContainer() == container)
						{
							operat = (Divider *) object;

							if (afterMe && Binary::IsFlagSet(operat->flags, OR_VERT))
							{
								if (Binary::IsFlagSet(operat->flags, OR_LEFT))
								{
									if (operat->pos.x >= doubleBar1.x - 2) doubleBar1.x = operat->pos.x + 3;
								}
								else
								{
									if (operat->pos.x <= doubleBar2.x+1) doubleBar2.x = wnd->size.cx - operat->pos.x - 2;
								}
							}

							if (operat == this) afterMe = True;
						}
					}
				}
			}
		}
		else if (container->GetObjectType() == Layer::classID)
		{
			doubleBar1.x = ((Layer *) container)->GetRealPosition().x + 4;

			if (Binary::IsFlagSet(flags, OR_TOP))		doubleBar1.y = ((Layer *) container)->GetRealPosition().y + pos.y;
			else if	(Binary::IsFlagSet(flags, OR_BOTTOM))	doubleBar1.y = ((Layer *) container)->GetRealPosition().y + ((Layer *) container)->size.cy - pos.y;

			doubleBar2.x = ((Layer *) container)->GetRealPosition().x + ((Layer *) container)->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
			{
				object = Object::GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID)
					{
						if (((Widget *) object)->GetContainer() == container)
						{
							operat = (Divider *) object;

							if (afterMe && Binary::IsFlagSet(operat->flags, OR_VERT))
							{
								if (Binary::IsFlagSet(operat->flags, OR_LEFT))
								{
									if (((Layer *) container)->GetRealPosition().x + operat->pos.x >= doubleBar1.x-2) doubleBar1.x = ((Layer *) container)->GetRealPosition().x + operat->pos.x + 3;
								}
								else
								{
									if (((Layer *) container)->GetRealPosition().x + ((Layer *) container)->size.cx - operat->pos.x <= doubleBar2.x+1) doubleBar2.x = ((Layer *) container)->GetRealPosition().x + ((Layer *) container)->size.cx - operat->pos.x - 2;
								}
							}

							if (operat == this) afterMe = True;
						}
					}
				}
			}
		}

		size.cx = doubleBar2.x - doubleBar1.x + 4;
		size.cy = 2;

		surface->Bar(doubleBar1, doubleBar2, OR_HORZ);
	}
	else if (Binary::IsFlagSet(flags, OR_VERT))
	{
		if (container->GetObjectType() == Window::classID)
		{
			if (Binary::IsFlagSet(flags, OR_LEFT))		doubleBar1.x = pos.x;
			else if	(Binary::IsFlagSet(flags, OR_RIGHT))	doubleBar1.x = wnd->size.cx - pos.x;

			doubleBar1.y = wnd->GetClientRect().top;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = wnd->size.cy - wnd->GetClientRect().bottom - 2;

			for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
			{
				object = Object::GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID)
					{
						if (((Widget *) object)->GetContainer() == container)
						{
							operat = (Divider *) object;

							if (afterMe && Binary::IsFlagSet(operat->flags, OR_HORZ))
							{
								if (Binary::IsFlagSet(operat->flags, OR_TOP))
								{
									if (operat->pos.y >= doubleBar1.y-2) doubleBar1.y = operat->pos.y + 3;
								}
								else
								{
									if (operat->pos.y <= doubleBar2.y+1) doubleBar2.y = wnd->size.cy - operat->pos.y - 2;
								}
							}

							if (operat == this) afterMe = True;
						}
					}
				}
			}
		}
		else if (container->GetObjectType() == Layer::classID)
		{
			if (Binary::IsFlagSet(flags, OR_LEFT))		doubleBar1.x = ((Layer *) container)->GetRealPosition().x + pos.x;
			else if (Binary::IsFlagSet(flags, OR_RIGHT))	doubleBar1.x = ((Layer *) container)->GetRealPosition().x + ((Layer *) container)->size.cx - pos.x;

			doubleBar1.y = ((Layer *) container)->GetRealPosition().y + 4;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = ((Layer *) container)->GetRealPosition().y + ((Layer *) container)->size.cy - 4;

			for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
			{
				object = Object::GetNthObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == classID)
					{
						if (((Widget *) object)->GetContainer() == container)
						{
							operat = (Divider *) object;

							if (afterMe && Binary::IsFlagSet(operat->flags, OR_HORZ))
							{
								if (Binary::IsFlagSet(operat->flags, OR_TOP))
								{
									if (((Layer *) container)->GetRealPosition().y + operat->pos.y >= doubleBar1.y-2) doubleBar1.y = ((Layer *) container)->GetRealPosition().y + operat->pos.y + 3;
								}
								else
								{
									if (((Layer *) container)->GetRealPosition().y + ((Layer *) container)->size.cy - operat->pos.y <= doubleBar2.y+1) doubleBar2.y = ((Layer *) container)->GetRealPosition().y + ((Layer *) container)->size.cy - operat->pos.y - 2;
								}
							}

							if (operat == this) afterMe = True;
						}
					}
				}
			}
		}

		size.cy = doubleBar2.y - doubleBar1.y + 4;
		size.cx = 2;

		surface->Bar(doubleBar1, doubleBar2, OR_VERT);
	}

	return Success;
}
