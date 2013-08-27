 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_POINTERXLIB
#define H_OBJSMOOTH_POINTERXLIB

namespace smooth
{
	namespace System
	{
		classPointerXLib;
	};
};

#include "../pointerbackend.h"
#include "../backends/xlib/backendxlib.h"

#include <X11/cursorfont.h>

namespace smooth
{
	namespace Input
	{
		const Short	 POINTER_XLIB = 2;

		class PoinerXLib : public PoinerBackend
		{
			private:
				X11::Display	*display;
			public:
						 PoinerXLib();
						~PoinerXLib();

				Bool		 SetCursor(const GUI::Window *, Pointer::CursorType);
		};
	};
};

#endif
