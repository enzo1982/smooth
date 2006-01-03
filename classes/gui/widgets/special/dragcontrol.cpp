 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/dragcontrol.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::DragControl::classID = S::Object::RequestClassID();

S::GUI::DragControl::DragControl() : Widget(Point(), Size(4096, 4096))
{
	type		= classID;
	orientation	= OR_CENTER;

	dragHotspot	= new Hotspot(GetPosition(), GetSize());

	dragHotspot->onMouseDragStart.Connect(&DragControl::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&DragControl::OnMouseDrag, this);

	RegisterObject(dragHotspot);
}

S::GUI::DragControl::~DragControl()
{
	DeleteObject(dragHotspot);
}

S::Void S::GUI::DragControl::OnMouseDragStart(const Point &mousePos)
{
	startMousePos = mousePos;
}

S::Void S::GUI::DragControl::OnMouseDrag(const Point &mousePos)
{
	Window	*window	= container->GetContainerWindow();

	if (!window->IsMaximized()) window->SetPosition(window->GetPosition() - (startMousePos - mousePos));
}
