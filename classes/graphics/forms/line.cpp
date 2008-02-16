 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/forms/line.h>

S::Bool S::GUI::Line::operator ==(const Line &line) const
{
	if (sx == line.sx && sy == line.sy && ex == line.ex && ey == line.ey)	return True;
	else									return False;
}

S::Bool S::GUI::Line::operator !=(const Line &line) const
{
	if (sx != line.sx || sy != line.sy || ex != line.ex || ey != line.ey)	return True;
	else									return False;
}
