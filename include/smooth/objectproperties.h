 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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
#include "point.h"
#include "size.h"

namespace smooth
{
	class SMOOTHAPI ObjectProperties
	{
		public:
			Point		 pos;
			Size		 size;

			Int		 orientation;

			String		 text;
			String		 tooltip;

			Size		 textSize;
			Size		 tooltipSize;

			String		 font;
			Int		 fontSize;
			Int		 fontWeight;
			Int		 fontColor;

			Bool		 clicked;
			Bool		 checked;

					 ObjectProperties();
	};
};

#endif
