 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_MENUBAR_COCOA
#define H_OBJSMOOTH_MENUBAR_COCOA

namespace smooth
{
	namespace GUI
	{
		class MenubarCocoa;
	};
};

#include "menubar.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MenubarCocoa : public Widget
		{
			private:
				const Menubar		*menubar;
				Void			*menubarCocoa;
			public:
				static const Short	 classID;

							 MenubarCocoa(Menubar *);
				virtual			~MenubarCocoa();

				Int			 Paint(Int);
		};
	};
};

#endif
