 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/primitives/primitive.h>

S::Primitive::Primitive()
{
	color = 0;
}

S::Primitive::~Primitive()
{
}

S::Int S::Primitive::SetColor(UnsignedLong nColor)
{
	color = nColor;

	return Success;
}

S::UnsignedLong S::Primitive::GetColor()
{
	return color;
}

S::Int S::Primitive::Draw(GUI::Surface *surface)
{
	return Success;
}
