 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOWXLIB_
#define _H_OBJSMOOTH_WINDOWXLIB_

namespace smooth
{
	namespace GUI
	{
		class WindowXLib;
	};
};

#include "../windowbackend.h"
#include "../../../graphics/rect.h"

#include <X11/Xlib.h>

namespace smooth
{
	namespace GUI
	{
		const Int	 WINDOW_XLIB	= 2;

		class SMOOTH_STATIC_EXPORT WindowXLib : public WindowBackend
		{
			private:
				Display	*display;
			protected:
				Window	 wnd;
			public:
					 WindowXLib(Void * = NIL);
					~WindowXLib();

				Void	*GetSystemWindow();

				Int	 Open(String, Point, Size, Int);
				Int	 Close();
		};
	};
};

#endif
