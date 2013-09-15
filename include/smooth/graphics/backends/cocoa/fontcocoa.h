 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FONTCOCOA
#define H_OBJSMOOTH_FONTCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace GUI
	{
		class FontCocoa;
	};
};

#include "../fontbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 FONT_COCOA	= 4;

		class FontCocoa : public FontBackend
		{
			public:
					 FontCocoa(const String &, Short, Short, Short, const Color &);
					~FontCocoa();

				Size	 GetTextSize(const String &, Bool) const;
		};
	};
};

#endif
