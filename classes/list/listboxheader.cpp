 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listboxheader.h>
#include <smooth/surface.h>
#include <smooth/listbox.h>
#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/objectproperties.h>
#include <smooth/toolkit.h>
#include <smooth/metrics.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_LISTBOXHEADER = S::Object::RequestObjectID();

S::GUI::ListBoxHeader::ListBoxHeader(ListBox *iListBox)
{
	listBox = iListBox;

	type	= OBJ_LISTBOXHEADER;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = listBox->GetObjectProperties()->pos.x + 1;
	objectProperties->pos.y = listBox->GetObjectProperties()->pos.y + 1;

	objectProperties->size.cx = listBox->GetObjectProperties()->size.cx - 3;
	objectProperties->size.cy = METRIC_LISTBOXENTRYHEIGHT;
}

S::GUI::ListBoxHeader::~ListBoxHeader()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::ListBoxHeader::AddTab(String tabName, Int iTabWidth)
{
	tabNames.AddEntry(tabName);
	tabWidths.AddEntry(iTabWidth);

	return Success;
}

S::Int S::GUI::ListBoxHeader::UpdateMetrics()
{
	objectProperties->pos.x = listBox->GetObjectProperties()->pos.x + 1;
	objectProperties->pos.y = listBox->GetObjectProperties()->pos.y + 1;

	objectProperties->size.cx = listBox->GetObjectProperties()->size.cx - 3;
	objectProperties->size.cy = METRIC_LISTBOXENTRYHEIGHT;

	return Success;
}

S::Int S::GUI::ListBoxHeader::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx;
			frame.bottom	= realPos.y + objectProperties->size.cy;

			surface->Box(frame, Setup::BackgroundColor, FILLED);
			surface->Frame(frame, FRAME_UP);

			frame.left += 3;
			frame.top += 1;

			if (active)	surface->SetText(tabNames.GetFirstEntry(), frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, FW_BOLD);
			else		surface->SetText(tabNames.GetFirstEntry(), frame, objectProperties->font, objectProperties->fontSize, Setup::GrayTextColor, FW_BOLD);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ListBoxHeader::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	frame.left	= realPos.x + 4;
	frame.top	= realPos.y + 4;
	frame.right	= realPos.x + objectProperties->size.cx - 5;
	frame.bottom	= realPos.y + objectProperties->size.cy - 5;

	switch (message)
	{
	}

	LeaveProtectedRegion();

	return retVal;
}
