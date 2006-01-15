 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPMENU_ENTRY_
#define _H_OBJSMOOTH_POPUPMENU_ENTRY_

namespace smooth
{
	namespace GUI
	{
		class PopupMenuEntry;

		class Hotspot;
	};
};

#include "menuentry.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI PopupMenuEntry : public MenuEntry
		{
			protected:
				Hotspot			*hotspot;
			public:
				static const Int	 classID;

							 PopupMenuEntry(const String & = NIL, const Bitmap & = NIL, Menu * = NIL, Bool * = NIL, Int * = NIL, Int = 0);
				virtual			~PopupMenuEntry();

				virtual Int		 Paint(Int);

				virtual Int		 Show();
				virtual Int		 Hide();
			slots:
				Void			 OnClickEntry();

				Void			 TogglePopupMenu();
		};
	};
};

#endif
