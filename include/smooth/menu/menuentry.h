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
	namespace GUI
	{
		class Menu;
	};
};

#include "../object.h"
#include "../rect.h"
#include "../graphics/font.h"
#include "../graphics/bitmap.h"
#include "../shortcut.h"

namespace smooth
{
	namespace GUI
	{
		const Int SM_SEPARATOR	= 0;
		const Int SM_TEXT	= 1;
		const Int SM_BITMAP	= 2;

		class SMOOTHAPI MenuEntry : public Widget
		{
			public:
				Int		 id;
				Int		 type;

				String		 text;
				String		 tooltip;
				String		 description;

				Bitmap		 bitmap;
				Bitmap		 graymap;

				Int		 orientation;

				Bool		 checked;
				Bool		 clicked;

				Bool		*bVar;
				Int		*iVar;
				Int		 iCode;

				Menu		*popup;

				Int		 size;
				Bool		 sizeset;

				Int		 scKey;
				Int		 scFlags;

				Shortcut	*shortcut;

				Font		 font;

						 MenuEntry(Int, Int);
						~MenuEntry();

				Int		 Paint(Int);

				Int		 SetText(String);
				Int		 SetTooltip(String);
				Int		 SetStatusText(String);
				Int		 SetShortcut(Int, Int);

				Int		 SetBitmap(const Bitmap &);

				Int		 SetOrientation(Int);
			signals:
				Signal0<Void>	 onClick;
		};
	};
};

#endif
