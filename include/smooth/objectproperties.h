 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTPROPERTIES_
#define _H_OBJSMOOTH_OBJECTPROPERTIES_

namespace smooth
{
	class ObjectProperties;
};

#include "definitions.h"
#include "graphics/point.h"
#include "graphics/size.h"
#include "misc/font.h"

namespace smooth
{
	class SMOOTHAPI ObjectProperties
	{
		public:
			GUI::Point	 pos;
			GUI::Size	 size;

			Int		 orientation;

			String		 text;
			String		 tooltip;

			GUI::Size	 textSize;
			GUI::Size	 tooltipSize;

			GUI::Font	 font;

			Bool		 clicked;
			Bool		 checked;

					 ObjectProperties();
	};
};

#endif
