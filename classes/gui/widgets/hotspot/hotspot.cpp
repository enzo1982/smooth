 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/hotspot/hotspot.h>

const S::Int	 S::GUI::Hotspot::classID = S::Object::RequestClassID();

S::GUI::Hotspot::Hotspot(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;
}

S::GUI::Hotspot::~Hotspot()
{
}

S::Int S::GUI::Hotspot::Show()
{
	if (visible) return Success();

	visible = True;

	onShow.Emit();

	return Success();
}

S::Int S::GUI::Hotspot::Hide()
{
	if (!visible) return Success();

	visible = False;

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Hotspot::Activate()
{
	active = True;

	Paint(SP_MOUSEOUT);

	onActivate.Emit();

	return Success();
}

S::Int S::GUI::Hotspot::Deactivate()
{
	active = False;

	mouseOver	= False;

	leftButtonDown	= False;
	rightButtonDown	= False;

	mouseDragging	= False;

	onDeactivate.Emit();

	return Success();
}
