 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_POINTERWAYLAND
#define H_OBJSMOOTH_POINTERWAYLAND

namespace smooth
{
	namespace Input
	{
		class PointerWayland;
	};
};

#include "../pointerbackend.h"

namespace smooth
{
	namespace Input
	{
		const Short	 POINTER_WAYLAND = 5;

		class PointerWayland : public PointerBackend
		{
			public:
					 PointerWayland();
					~PointerWayland();

				Bool	 SetCursor(const GUI::Window *, Pointer::CursorType);
		};
	};
};

#endif
