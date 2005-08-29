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
}

Int ActiveAreaPlugin::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return Success;

	Surface	*surface	= window->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), size);

	surface->Box(frame, areaColor, FILLED);
	surface->Frame(frame, FRAME_DOWN);

	return Success;
}

Int ActiveAreaPlugin::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Failure;
	if (!active || !visible)	return Success;

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return Success;

	Point	 realPos	= GetRealPosition();
	Int	 retVal		= Success;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if ((window->MouseX() > realPos.x) && (window->MouseX() < (realPos.x + size.cx - 1)) && (window->MouseY() > realPos.y) && (window->MouseY() < (realPos.y + size.cy - 1)))
			{
				onClick.Emit(window->MouseX(), window->MouseY());

				retVal = Break;
			}

			break;
	}

	return retVal;
}
