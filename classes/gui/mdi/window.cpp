 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/mdi/window.h>

const S::Int	 S::GUI::MDI::Window::classID = S::Object::RequestClassID();

S::GUI::MDI::Window::Window(const String &title, const Point &iPos, const Size &iSize) : GUI::Window(title, iPos, iSize)
{
	type = classID;
}

S::GUI::MDI::Window::~Window()
{
}
