 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SHORTCUT_
#define _H_OBJSMOOTH_SHORTCUT_

namespace smooth
{
	namespace GUI
	{
		class Shortcut;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SC_NONE	= 0;
		const Int	 SC_ALT		= 1;
		const Int	 SC_CTRL	= 2;
		const Int	 SC_SHIFT	= 4;

		class SMOOTHAPI Shortcut : public Widget
		{
			protected:
				Int			 key;
				Int			 param;
			public:
				static const Int	 classID;

							 Shortcut(Int, Int, Int = 0);
							~Shortcut();

				Int			 Process(Int, Int, Int);

				Int			 SetShortcut(Int, Int, Int = 0);
			signals:
				Signal1<Void, Int>	 onKeyDown;
		};
	};
};

#endif
