 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/modifiers/righttoleft.h>

S::GUI::RightToLeftModifier::RightToLeftModifier()
{
	surfaceSize = Size(0, 0);

	rightToLeft = False;
}

S::GUI::RightToLeftModifier::~RightToLeftModifier()
{
}

S::Int S::GUI::RightToLeftModifier::TranslateX(Int x) const
{
	if (rightToLeft) return surfaceSize.cx - x;
	else		 return x;
}

S::Int S::GUI::RightToLeftModifier::TranslateY(Int y) const
{
	return y;
}

S::GUI::Point S::GUI::RightToLeftModifier::TranslatePoint(const Point &p) const
{
	if (rightToLeft) return Point(surfaceSize.cx - p.x, p.y);
	else		 return p;
}

S::GUI::Rect S::GUI::RightToLeftModifier::TranslateRect(const Rect &r) const
{
	if (rightToLeft) return Rect(Point(surfaceSize.cx - r.right, r.top), r.GetSize());
	else		 return Rect(Point(		    r.left,  r.top), r.GetSize());
}

S::Void S::GUI::RightToLeftModifier::SetSurfaceSize(const Size &nSurfaceSize)
{
	surfaceSize = nSurfaceSize;
}

const S::GUI::Size &S::GUI::RightToLeftModifier::GetSurfaceSize() const
{
	return surfaceSize;
}

S::Void S::GUI::RightToLeftModifier::SetRightToLeft(Bool nRightToLeft)
{
	rightToLeft = nRightToLeft;
}

S::Bool S::GUI::RightToLeftModifier::GetRightToLeft() const
{
	return rightToLeft;
}
