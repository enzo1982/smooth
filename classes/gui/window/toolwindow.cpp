 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/toolwindow.h>

const S::Int	 S::GUI::ToolWindow::classID = S::Object::RequestClassID();

S::GUI::ToolWindow::ToolWindow(const Point &iPos, const Size &iSize) : Window("smooth ToolWindow", iPos, iSize)
{
	type		= classID;
	orientation	= OR_FREE;
	visible		= True;

	frameWidth	= 0;

	SetFlags(WF_TOPMOST | WF_NOTASKBUTTON | WF_THINBORDER);
}

S::GUI::ToolWindow::~ToolWindow()
{
}

S::Bool S::GUI::ToolWindow::Create()
{
	Window	*containerWindow = container->GetContainerWindow();

	if (containerWindow != NIL)
	{
		if (Setup::rightToLeft) SetPosition(Point(containerWindow->GetWidth() - (GetX() + GetWidth()), GetY()));

		SetPosition(GetPosition() + containerWindow->GetPosition());
	}

	return Window::Create();
}

S::Bool S::GUI::ToolWindow::IsTypeCompatible(Int compType)
{
	if (compType == Object::classID || compType == Widget::classID || compType == Window::classID)	return True;
	else												return False;
}
