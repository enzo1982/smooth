 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/basic/input.h>

S::GUI::Point	 S::Input::mousePosition = S::GUI::Point();

S::Input::Input()
{
}

S::Input::Input(const Input &)
{
}

const S::GUI::Point &S::Input::GetMousePosition()
{
#ifdef __WIN32__
	POINT	 point;

	GetCursorPos(&point);

	mousePosition.x = point.x;
	mousePosition.y = point.y;

	return mousePosition;
#else
	return 0;
#endif
}
