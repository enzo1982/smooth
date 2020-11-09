 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/generic/container.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::Container::classID = S::Object::RequestClassID();

S::GUI::Container::Container() : Widget(Point(), Size())
{
	type = classID;

	onChangeEntryOrder.SetParentObject(this);
}

S::GUI::Container::~Container()
{
	onChangeEntryOrder.DisconnectAll();

	RemoveAllEntries();
}

S::Int S::GUI::Container::EnableLocking(Bool enable)
{
	if (enable)
	{
		createdEntry.EnableLocking();
		elementOrder.EnableLocking();
	}
	else
	{
		createdEntry.DisableLocking();
		elementOrder.DisableLocking();
	}

	return Widget::EnableLocking(enable);
}

S::Int S::GUI::Container::Add(Widget *widget)
{
	if (widget == NIL) return Error();

	if (!widget->IsRegistered())
	{
		if (Widget::Add(widget) == Success() && widget->GetObjectType() == Entry::classID)
		{
			elementOrder.Add((Entry *) widget, widget->GetHandle());
			createdEntry.Add(False, widget->GetHandle());

			Paint(SP_UPDATE);
			Process(SM_MOUSEMOVE, 0, 0);

			onChangeEntryOrder.Emit();

			return Success();
		}
	}

	return Error();
}

S::Int S::GUI::Container::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	if (Widget::Remove(widget) == Success() && widget->GetObjectType() == Entry::classID)
	{
		Int	 entryHandle = widget->GetHandle();

		if (createdEntry.Get(entryHandle)) DeleteObject(widget);

		elementOrder.Remove(entryHandle);
		createdEntry.Remove(entryHandle);

		Paint(SP_UPDATE);
		Process(SM_MOUSEMOVE, 0, 0);

		onChangeEntryOrder.Emit();

		return Success();
	}

	return Error();
}

S::Int S::GUI::Container::RemoveAllEntries()
{
	Surface	*surface = GetDrawSurface();
	Bool	 visible = IsVisible();

	if (visible)
	{
		Rect	 frame = Rect(GetRealPosition(), GetRealSize());

		surface->StartPaint(frame);

		Hide();
	}

	while (Length() > 0) Remove(GetNthEntry(Length() - 1));

	if (visible)
	{
		Show();

		surface->EndPaint();
	}

	return Success();
}

S::Int S::GUI::Container::MoveEntry(Int n, Int m)
{
	if (n == m)			    return Success();
	if (n >= Length() || m >= Length()) return Error();
	if (n <  0	  || m <  0	  ) return Error();

	elementOrder.MoveNth(n, m);

	onChangeEntryOrder.Emit();

	return Success();
}
