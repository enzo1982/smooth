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
	namespace GUI
	{
		class Menu;
	};
};

#include "../object.h"
#include "../container.h"
#include "../array.h"
#include "../graphics/bitmap.h"
#include "menuentry.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Menu : public Widget, public Container
		{
			public:
				static const Int	 classID;

							 Menu();
							~Menu();

				MenuEntry		*AddEntry(String = NIL, Bitmap = NIL, Menu * = NIL, Bool * = NIL, Int * = NIL, Int = 0, Int = OR_LEFT);
				Int			 RemoveEntry(MenuEntry *);

				Int			 Clear();
		};
	};
};

#endif
