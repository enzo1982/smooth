 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENU_
#define _H_OBJSMOOTH_MENU_

namespace smooth
{
	class Menu;
};

#include "object.h"
#include "rect.h"

namespace smooth
{
	const Int SM_SEPARATOR	= 0;
	const Int SM_TEXT	= 1;
	const Int SM_BITMAP	= 2;

	class SMOOTHAPI Menu
	{
		public: class SMOOTHAPI Entry
		{
			public:
				Int		 type;
				Int		 id;

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

						 Entry(Int, Int);
						~Entry();

				Int		 SetText(String);
				Int		 SetTooltip(String);
				Int		 SetStatusText(String);

				Int		 SetBitmap(HBITMAP);

				Int		 SetOrientation(Int);
			signals:
				Signal0<Void>	 onClick;
		};

		protected:
			Int		 nOfEntries;

			Int		 GetSizeY();
			Int		 GetSizeX();
			Void		 GetMenuEntriesSize();
		public:
			Bool		 sizeset;
			Bool		 entrysizesset;
			Array<Entry *>	 entries;
			Size		 popupsize;

					 Menu();
					~Menu();

			Entry		*AddEntry(String text = NIL, HBITMAP bitmap = NIL, Menu *popupMenu = NIL, Bool *bVar = NIL, Int *iVar = NIL, Int iCode = 0, Int orientation = OR_LEFT);
			Int		 RemoveEntry(Entry *);
			Int		 RemoveEntry(Int);

			Void		 GetSize();
			Int		 GetNOfEntries();

			Int		 Clear();
	};
};

#endif
