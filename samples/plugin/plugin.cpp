 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>
#include "plugin.h"

Void smooth::AttachDLL()
{
}

Void smooth::DetachDLL()
{
}

const Int	 ActiveAreaPlugin::classID = Object::RequestClassID();

ActiveAreaPlugin::ActiveAreaPlugin(Int color, Point pos, Size size)
{
	type			= classID;
	areaColor		= color;
	objectProperties->pos	= pos;
	objectProperties->size	= size;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x		= Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(pos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(size.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(size.cy * Setup::FontSize);
}

ActiveAreaPlugin::~ActiveAreaPlugin()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

Int ActiveAreaPlugin::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = wnd->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	surface->Frame(frame, FRAME_DOWN);

	frame.left++;
	frame.top++;

	surface->Box(frame, areaColor, FILLED);

	return Success;
}

Int ActiveAreaPlugin::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if ((wnd->MouseX() > realPos.x) && (wnd->MouseX() < (realPos.x + objectProperties->size.cx - 1)) && (wnd->MouseY() > realPos.y) && (wnd->MouseY() < (realPos.y + objectProperties->size.cy - 1)))
			{
				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
	}

	return retVal;
}
