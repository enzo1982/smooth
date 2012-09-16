 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/misc/binary.h>
#include <smooth/input/pointer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short	 S::GUI::Divider::classID = S::Object::RequestClassID();

S::GUI::Divider::Divider(Int iPosition, Int iOrientation) : Widget(Point(), Size())
{
	type		= classID;
	flags		= iOrientation;
	orientation	= OR_FREE;
	position	= iPosition;
	dragging	= False;

	if (position == 0) position = 120;

	dragHotspot = new Hotspot(Point(), Size());
	dragHotspot->SetIndependent(True);

	dragHotspot->onMouseOver.Connect(&Divider::OnMouseOver, this);
	dragHotspot->onMouseOut.Connect(&Divider::OnMouseOut, this);

	dragHotspot->onMouseDragStart.Connect(&Divider::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&Divider::OnMouseDrag, this);
	dragHotspot->onMouseDragEnd.Connect(&Divider::OnMouseDragEnd, this);

	Add(dragHotspot);
}

S::GUI::Divider::~Divider()
{
	DeleteObject(dragHotspot);
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
				UpdateMetrics();

				Surface	*surface = GetDrawSurface();
				Rect	 rect = Rect(GetRealPosition(), GetRealSize());

				if (Binary::IsFlagSet(flags, OR_VERT))	surface->Bar(rect.GetPosition(), Point(rect.left,      rect.bottom - 1), OR_VERT);
				else					surface->Bar(rect.GetPosition(), Point(rect.right - 1, rect.top       ), OR_HORZ);
			}

			break;
	}

	return Success();
}

S::Void S::GUI::Divider::UpdateMetrics()
{
	Window	*wnd = GetContainerWindow();
	Rect	 rect;

	if (wnd == NIL) return;

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
			if (Binary::IsFlagSet(flags, OR_RIGHT))	rect.left = container->GetWidth() - position;
			else					rect.left = position;

			rect.top	= 3;
			rect.bottom	= container->GetHeight() - 3;
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
					if (container->GetHeight() - divider->position <= rect.bottom + 1) rect.bottom = container->GetHeight() - divider->position - 2;
				}
				else
				{
					if (divider->position >= rect.top - 2) rect.top = divider->position + 3;
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
			else					 rect.top = ((Window *) container)->GetClientRect().top	   + position;

			rect.left	= wnd->GetClientRect().left;
			rect.right	= wnd->GetClientRect().right;
		}
		else
		{
			if (Binary::IsFlagSet(flags, OR_BOTTOM)) rect.top = container->GetHeight() - position;
			else					 rect.top = 			     position;

			rect.left	= 3;
			rect.right	= container->GetWidth() - 3;
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
					if (container->GetWidth() - divider->position <= rect.right + 1) rect.right = container->GetWidth() - divider->position - 2;
				}
				else
				{
					if (divider->position >= rect.left - 2) rect.left = divider->position + 3;
				}
			}

			if (divider == this) afterMe = True;
		}
	}

	SetMetrics(rect.GetPosition(), rect.GetSize() + Size(1, 1));

	if (Binary::IsFlagSet(flags, OR_VERT))	dragHotspot->SetMetrics(Point(-1, 0), rect.GetSize() + Size(3, 1));
	else					dragHotspot->SetMetrics(Point(0, -1), rect.GetSize() + Size(1, 3));
}

S::Void S::GUI::Divider::OnMouseOver()
{
	if (!Binary::IsFlagSet(flags, DIV_MOVABLE)) return;

	if (Binary::IsFlagSet(flags, OR_VERT))	Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorHSize);
	else					Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorVSize);
}

S::Void S::GUI::Divider::OnMouseOut()
{
	if (!Binary::IsFlagSet(flags, DIV_MOVABLE)) return;

	if (!dragging) Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorArrow);
}

S::Void S::GUI::Divider::OnMouseDragStart(const Point &mousePos)
{
	dragging = True;

	startPos = GetPos();
	startMousePos = mousePos;
}

S::Void S::GUI::Divider::OnMouseDrag(const Point &mousePos)
{
	Int	 pos = startPos;

	if (Binary::IsFlagSet(flags, OR_VERT))	pos -= (startMousePos.x - mousePos.x);
	else					pos -= (startMousePos.y - mousePos.y);

	if (pos != GetPos()) onDrag.Emit(pos);
}


S::Void S::GUI::Divider::OnMouseDragEnd(const Point &mousePos)
{
	dragging = False;

	if (!dragHotspot->IsMouseOver()) Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorArrow);
}
