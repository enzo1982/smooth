 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTENTRY_
#define _H_OBJSMOOTH_LISTENTRY_

#include "object.h"
#include "primitives/rect.h"
#include "graphics/font.h"

namespace smooth
{
	class SMOOTHAPI ListEntry : public Object
	{
		public:
			Int		 id;
			String		 name;

			Bool		 checked;
			Bool		 clicked;
			Bool		 selected;
			Int		 size;
			Rect		 rect;

			GUI::Font	 font;

					 ListEntry(Int);
					~ListEntry();
		signals:
			Signal0<Void>	 onMouseOver;
			Signal0<Void>	 onMouseOut;
			Signal0<Void>	 onClick;
	};
};

#endif
