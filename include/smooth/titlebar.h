 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TITLEBAR_
#define _H_OBJSMOOTH_TITLEBAR_

namespace smooth
{
	namespace GUI
	{
		class Titlebar;
		class Menubar;
		class Window;
	};
};

#include "widget.h"
#include "rect.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Titlebar : public Widget
		{
			friend class Menubar;
			friend class Window;
			private:
				Bool		 minchk;
				Bool		 minclk;
				Bool		 maxchk;
				Bool		 maxclk;
				Bool		 closechk;
				Bool		 closeclk;
				Rect		 nonmaxrect;
				Bool		 min;
				Bool		 max;
				Bool		 close;
				Int		 origwndstyle;
			public:
						 Titlebar(Bool, Bool, Bool);
			 			~Titlebar();
				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);
		};
	};

	SMOOTHVAR Int OBJ_TITLEBAR;
};

#endif
