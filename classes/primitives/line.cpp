 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/primitives/line.h>

S::Line::Line()
{
	sx = 0;
	sy = 0;
	ex = 0;
	ey = 0;
}

S::Line::Line(const Point &iStart, const Point &iEnd)
{
	sx = iStart.x;
	sy = iStart.y;
	ex = iEnd.x;
	ey = iEnd.y;
}

S::Bool S::Line::operator ==(const Line &line)
{
	if (sx == line.sx && sy == line.sy && ex == line.ex && ey == line.ey)	return True;
	else									return False;
}

S::Bool S::Line::operator !=(const Line &line)
{
	if (sx != line.sx || sy != line.sy || ex != line.ex || ey != line.ey)	return True;
	else									return False;
}
