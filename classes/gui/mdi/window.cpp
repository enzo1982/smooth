 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/mdi/window.h>
#include <smooth/gui/window/window.h>
#include <smooth/definitions.h>

const S::Int	 S::GUI::MDI::Window::classID = S::Object::RequestClassID();

S::GUI::MDI::Window::Window(String title) : GUI::Window(title)
{
	type		= classID;
	containerType	= GUI::Window::classID;

	orientation	= OR_FREE;

	possibleContainers.RemoveAll();
	possibleContainers.AddEntry(GUI::Window::classID);
}

S::GUI::MDI::Window::~Window()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}
