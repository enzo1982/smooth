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

S::GUI::ListEntry::ListEntry(Int entryID)
{
	id		= entryID;

	selected	= False;

	tipTimer	= NIL;
	tooltip		= NIL;

	font.SetColor(Setup::ClientTextColor);

	onMouseOver.SetParentObject(this);
	onMouseOut.SetParentObject(this);
}

S::GUI::ListEntry::~ListEntry()
{
	if (tipTimer != NIL)
	{
		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (tooltip != NIL)
	{
		Window	*wnd = tooltip->GetContainer()->GetContainerWindow();

		tooltip->Hide();

		wnd->UnregisterObject(tooltip);

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Void S::GUI::ListEntry::ActivateTooltip()
{
	if (tooltip != NIL) return;

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer	= NIL;
	tooltip		= new Tooltip();

	tooltip->SetText(tooltipText);
	tooltip->SetTimeout(3000);

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object->GetObjectType() == Window::classID)
		{
			tooltip->SetMetrics(Point(((Window *) object)->MouseX(), ((Window *) object)->MouseY()), Size(0, 0));

			((Window *) object)->RegisterObject(tooltip);

			break;
		}
	}
}
