 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
#include "array.h"
#include "menuentry.h"

namespace smooth
{
	class SMOOTHAPI Menu
	{
		protected:
			Int			 nOfEntries;

			Int			 GetSizeY();
			Int			 GetSizeX();
			Void			 GetMenuEntriesSize();
		public:
			Bool			 sizeset;
			Bool			 entrysizesset;
			Array<MenuEntry *>	 entries;
			Size			 popupsize;

						 Menu();
						~Menu();

			MenuEntry		*AddEntry(String text = NIL, HBITMAP bitmap = NIL, Menu *popupMenu = NIL, Bool *bVar = NIL, Int *iVar = NIL, Int iCode = 0, Int orientation = OR_LEFT);
			Int			 RemoveEntry(MenuEntry *);
			Int			 RemoveEntry(Int);

			Void			 GetSize();
			Int			 GetNOfEntries();

			Int			 Clear();
	};
};

#endif
