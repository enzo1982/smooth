 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
#include "graphics/font.h"
#include "graphics/bitmap.h"
#include "shortcut.h"

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

			GUI::Bitmap	 bitmap;
			GUI::Bitmap	 graymap;

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

			Int		 scKey;
			Int		 scFlags;

			GUI::Shortcut	*shortcut;

			GUI::Font	 font;

					 MenuEntry(Int, Int);
					~MenuEntry();

			Int		 SetText(String);
			Int		 SetTooltip(String);
			Int		 SetStatusText(String);
			Int		 SetShortcut(Int, Int);

			Int		 SetBitmap(const GUI::Bitmap &);

			Int		 SetOrientation(Int);
		signals:
			Signal0<Void>	 onClick;
	};
};

#endif
