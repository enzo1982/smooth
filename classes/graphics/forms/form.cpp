 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/forms/form.h>

S::GUI::Form::Form()
{
	color = 0;
}

S::GUI::Form::~Form()
{
}

S::Int S::GUI::Form::SetColor(UnsignedLong nColor)
{
	color = nColor;

	return Success();
}

S::UnsignedLong S::GUI::Form::GetColor()
{
	return color;
}

S::Int S::GUI::Form::Draw(Surface *surface)
{
	return Success();
}
