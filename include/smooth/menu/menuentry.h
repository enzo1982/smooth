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
#include "../primitives/rect.h"
#include "../misc/font.h"
#include "../graphics/bitmap.h"
#include "../window/shortcut.h"

namespace smooth
{
	namespace GUI
	{
		const Int SM_SEPARATOR	= 0;
		const Int SM_TEXT	= 1;
		const Int SM_BITMAP	= 2;

		class SMOOTHAPI MenuEntry : public Widget
		{
			private:
				Int		 popupHandle;

				Int		 CalcSize();
			public:
				Int		 type;

				String		 description;

				Bitmap		 bitmap;
				Bitmap		 graymap;

				Bool		*bVar;
				Int		*iVar;
				Int		 iCode;

				Menu		*popup;

				Int		 scKey;
				Int		 scFlags;

				Shortcut	*shortcut;

						 MenuEntry(String, Bitmap, Menu *, Bool *, Int *, Int, Int);
						~MenuEntry();

				Int		 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 SetText(String);
				Int		 SetStatusText(String);
				Int		 SetShortcut(Int, Int);

				Int		 SetBitmap(const Bitmap &);
			signals:
				Signal0<Void>	 onClick;
			slots:
				Void		 PopupProc();
		};
	};
};

#endif
