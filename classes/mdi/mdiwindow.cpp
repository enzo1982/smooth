 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/mdiwindow.h>
#include <smooth/window.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_MDIWINDOW = S::Object::RequestObjectID();

S::GUI::MDIWindow::MDIWindow(String title) : Window(title)
{
	type				= OBJ_MDIWINDOW;
	containerType			= OBJ_WINDOW;

	objectProperties->pos.x		= 0;
	objectProperties->pos.y		= 0;
	objectProperties->size.cx	= 100;
	objectProperties->size.cy	= 100;
	objectProperties->orientation	= OR_FREE;

	possibleContainers.DeleteAll();
	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::GUI::MDIWindow::~MDIWindow()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}
