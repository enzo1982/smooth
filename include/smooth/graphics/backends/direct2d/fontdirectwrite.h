 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FONTDIRECTWRITE
#define H_OBJSMOOTH_FONTDIRECTWRITE

namespace smooth
{
	namespace GUI
	{
		class FontDirectWrite;
	};
};

#include "../fontbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 FONT_DIRECTWRITE = 8;

		class FontDirectWrite : public FontBackend
		{
			public:
						 FontDirectWrite(const String &, Short, Short, Short, const Color &);
						~FontDirectWrite();

				Size		 GetTextSize(const String &) const;
		};
	};
};

#endif
