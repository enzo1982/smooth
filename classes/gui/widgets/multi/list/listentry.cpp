 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listentry.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/window/window.h>
#include <smooth/system/timer.h>

S::ListEntry::ListEntry(Int entryID)
{
	id		= entryID;

	checked		= False;
	clicked		= False;

	selected	= False;

	size		= -1;

	tipTimer	= NIL;
	tooltip		= NIL;

	font.SetColor(Setup::ClientTextColor);

	onClick.SetParentObject(this);
	onMouseOver.SetParentObject(this);
	onMouseOut.SetParentObject(this);
}

S::ListEntry::~ListEntry()
{
	if (tipTimer != NIL)
	{
		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (tooltip != NIL)
	{
		GUI::Window	*wnd = tooltip->GetContainer()->GetContainerWindow();

		tooltip->Hide();

		wnd->UnregisterObject(tooltip);

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Int S::ListEntry::SetTooltipText(const String &nTooltipText)
{
	tooltipText = nTooltipText;

	return Success;
}

S::Void S::ListEntry::ActivateTooltip()
{
	if (tooltip != NIL) return;

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer	= NIL;
	tooltip		= new GUI::Tooltip();

	tooltip->SetText(tooltipText);
	tooltip->SetTimeout(3000);

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object->GetObjectType() == GUI::Window::classID)
		{
			tooltip->SetMetrics(GUI::Point(((GUI::Window *) object)->MouseX(), ((GUI::Window *) object)->MouseY()), GUI::Size(0, 0));

			((GUI::Window *) object)->RegisterObject(tooltip);

			break;
		}
	}
}
