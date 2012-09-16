 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FONTXLIB
#define H_OBJSMOOTH_FONTXLIB

namespace smooth
{
	namespace GUI
	{
		class FontXLib;
	};
};

#include "../fontbackend.h"

namespace X11
{
	extern "C"
	{
#		define True	1
#		define False	0
#		define Bool	int
#		define Success	0

#		include <X11/Xlib.h>

#		undef True
#		undef False
#		undef Bool
#		undef Success
	}
};

namespace smooth
{
	namespace GUI
	{
		const Short	 FONT_XLIB	= 2;

		class FontXLib : public FontBackend
		{
			public:
					 FontXLib(const String &, Short, Short, Short, const Color &);
					~FontXLib();

				Size	 GetTextSize(const String &, Bool) const;
		};
	};
};

#endif
