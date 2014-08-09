 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/modifiers/upsidedown.h>

S::GUI::UpsideDownModifier::UpsideDownModifier()
{
	surfaceSize = Size(0, 0);
}

S::GUI::UpsideDownModifier::~UpsideDownModifier()
{
}

S::Int S::GUI::UpsideDownModifier::TranslateX(Int x) const
{
	return x;
}

S::Int S::GUI::UpsideDownModifier::TranslateY(Int y) const
{
	return surfaceSize.cy - y - 1;
}

S::GUI::Point S::GUI::UpsideDownModifier::TranslatePoint(const Point &p) const
{
	return Point(p.x, surfaceSize.cy - p.y - 1);
}

S::GUI::Rect S::GUI::UpsideDownModifier::TranslateRect(const Rect &r) const
{
	return Rect(Point(r.left, surfaceSize.cy - r.bottom), r.GetSize());
}

S::Void S::GUI::UpsideDownModifier::SetSurfaceSize(const Size &nSurfaceSize)
{
	surfaceSize = nSurfaceSize;
}

const S::GUI::Size &S::GUI::UpsideDownModifier::GetSurfaceSize() const
{
	return surfaceSize;
}
