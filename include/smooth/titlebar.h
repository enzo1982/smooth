 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
#include "primitives/rect.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 TB_NONE	= 0;
		const Int	 TB_MINBUTTON	= 1;
		const Int	 TB_MAXBUTTON	= 2;
		const Int	 TB_CLOSEBUTTON	= 4;

		class SMOOTHAPI Titlebar : public Widget
		{
			private:
				Bool			 paintActive;
				Bool			 minchk;
				Bool			 minclk;
				Bool			 maxchk;
				Bool			 maxclk;
				Bool			 closechk;
				Bool			 closeclk;
			public:
				static const Int	 classID;

							 Titlebar(Int = TB_MINBUTTON | TB_MAXBUTTON | TB_CLOSEBUTTON);
			 				~Titlebar();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);
		};
	};
};

#endif
