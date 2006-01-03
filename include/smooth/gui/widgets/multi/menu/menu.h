 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

#include "menuentry.h"

namespace smooth
{
	namespace GUI
	{
		abstract class SMOOTHAPI Menu : public Widget
		{
			public:
				static const Int	 classID;

							 Menu();
				virtual			~Menu();

				Int			 RemoveEntry(MenuEntry *);

				Int			 Clear();
		};
	};
};

#endif
