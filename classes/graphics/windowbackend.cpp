 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/windowbackend.h>

S::GUI::WindowBackend::WindowBackend(Void *iWindow)
{
	type = WINDOW_NONE;
}

S::GUI::WindowBackend::~WindowBackend()
{
}

S::Int S::GUI::WindowBackend::GetWindowType()
{
	return type;
}

S::Void *S::GUI::WindowBackend::GetSystemWindow()
{
	return NIL;
}
