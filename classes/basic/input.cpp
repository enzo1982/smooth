 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input.h>

S::Input::Input()
{
}

S::Input::Input(const Input &)
{
}

S::Int S::Input::MouseX()
{
	POINT	 point;

	GetCursorPos(&point);

	return point.x;
}

S::Int S::Input::MouseY()
{
	POINT	 point;

	GetCursorPos(&point);

	return point.y;
}
