 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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

ActiveAreaPlugin::ActiveAreaPlugin(Int color, Point iPos, Size iSize)
{
	type		= classID;
	areaColor	= color;
	pos		= iPos;
	size		= iSize;

	possibleContainers.AddEntry(Layer::classID);

	pos.x	= Math::Round(pos.x * Setup::FontSize);
	pos.y	= Math::Round(pos.y * Setup::FontSize);
	size.cx	= Math::Round(size.cx * Setup::FontSize);
	size.cy	= Math::Round(size.cy * Setup::FontSize);
}

ActiveAreaPlugin::~ActiveAreaPlugin()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

Int ActiveAreaPlugin::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = wnd->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - 1;
	frame.bottom	= realPos.y + size.cy - 1;

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

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if ((wnd->MouseX() > realPos.x) && (wnd->MouseX() < (realPos.x + size.cx - 1)) && (wnd->MouseY() > realPos.y) && (wnd->MouseY() < (realPos.y + size.cy - 1)))
			{
				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
	}

	return retVal;
}
