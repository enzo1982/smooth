 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FONTCAIRO
#define H_OBJSMOOTH_FONTCAIRO

namespace smooth
{
	namespace GUI
	{
		class FontCairo;
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
		const Short	 FONT_CAIRO	= 3;

		class FontCairo : public FontBackend
		{
			public:
				static Int	 Initialize();

						 FontCairo(const String &, Short, Short, Short, const Color &);
						~FontCairo();

				Size		 GetTextSize(const String &) const;
		};
	};
};

#endif
