 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENUENTRY_
#define _H_OBJSMOOTH_MENUENTRY_

namespace smooth
{
	class Menu;
};

#include "object.h"
#include "rect.h"
#include "font.h"

namespace smooth
{
	const Int SM_SEPARATOR	= 0;
	const Int SM_TEXT	= 1;
	const Int SM_BITMAP	= 2;

	class SMOOTHAPI MenuEntry : public Object
	{
		public:
			Int		 id;
			Int		 type;

			String		 text;
			String		 tooltip;
			String		 description;

			HBITMAP		 bitmap;
			HBITMAP		 graymap;

			Int		 orientation;

			Bool		 checked;
			Bool		 clicked;

			Bool		*bVar;
			Int		*iVar;
			Int		 iCode;

			Menu		*popup;

			Int		 size;
			Bool		 sizeset;
			Rect		 rect;

			GUI::Font	 font;

					 MenuEntry(Int, Int);
					~MenuEntry();

			Int		 SetText(String);
			Int		 SetTooltip(String);
			Int		 SetStatusText(String);

			Int		 SetBitmap(HBITMAP);

			Int		 SetOrientation(Int);
		signals:
			Signal0<Void>	 onClick;
	};
};

#endif
