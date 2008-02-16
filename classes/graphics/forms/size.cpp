 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/forms/size.h>
#include <smooth/misc/math.h>

S::GUI::Size S::GUI::Size::operator +(const Size &size) const
{
	Size	 retS(cx + size.cx, cy + size.cy);

	return retS;
}

S::GUI::Size S::GUI::Size::operator -(const Size &size) const
{
	Size	 retS(cx - size.cx, cy - size.cy);

	return retS;
}

S::GUI::Size S::GUI::Size::operator *(const Float factor) const
{
	Size	 retS(Math::Round(cx * factor), Math::Round(cy * factor));

	return retS;
}

S::GUI::Size S::GUI::Size::operator /(const Float divisor) const
{
	Size	 retS(Math::Round(cx / divisor), Math::Round(cy / divisor));

	return retS;
}

S::Bool S::GUI::Size::operator ==(const Size &size) const
{
	if (cx == size.cx && cy == size.cy)	return True;
	else					return False;
}

S::Bool S::GUI::Size::operator !=(const Size &size) const
{
	if (cx != size.cx || cy != size.cy)	return True;
	else					return False;
}

#ifdef __WIN32__
S::GUI::Size::operator SIZE() const
{
	SIZE	 size;

	size.cx = cx;
	size.cy = cy;

	return size;
}

S::GUI::Size &S::GUI::Size::operator =(const SIZE &size)
{
	cx = size.cx;
	cy = size.cy;

	return *this;
}
#endif
