 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/toolwindow.h>

const S::Short	 S::GUI::ToolWindow::classID = S::Object::RequestClassID();

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
	if (IsRegistered())
	{
		Window	*containerWindow = container->GetContainerWindow();

		if (containerWindow != NIL)
		{
			if (IsRightToLeft()) Window::SetMetrics(Point(containerWindow->GetWidth() - (GetX() + GetWidth()), GetY()), GetSize());

			Window::SetMetrics(GetPosition() + containerWindow->GetPosition(), GetSize());
		}
	}

	return Window::Create();
}

S::Int S::GUI::ToolWindow::SetMetrics(const Point &nPos, const Size &nSize)
{
	Point	 position = nPos;

	if (IsRegistered() && created)
	{
		Window	*containerWindow = container->GetContainerWindow();

		if (containerWindow != NIL)
		{
			if (IsRightToLeft()) position.x = containerWindow->GetWidth() - (nPos.x + nSize.cx);

			position += containerWindow->GetPosition();
		}
	}

	return Window::SetMetrics(position, nSize);
}

S::Bool S::GUI::ToolWindow::IsRightToLeft() const
{
	if (IsRegistered() && layoutDirection == LD_DEFAULT)
	{
		Window	*containerWindow = container->GetContainerWindow();

		if (containerWindow != NIL) return containerWindow->IsRightToLeft();
	}

	return Window::IsRightToLeft();
}

S::Bool S::GUI::ToolWindow::IsTypeCompatible(Short compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == Window::classID)	return True;
	else												return False;
}
