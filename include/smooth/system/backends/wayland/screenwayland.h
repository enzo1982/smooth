 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SCREENWAYLAND
#define H_OBJSMOOTH_SCREENWAYLAND

namespace smooth
{
	namespace System
	{
		class ScreenWayland;
	};
};

#include "../screenbackend.h"

namespace smooth
{
	namespace System
	{
		const Short	 SCREEN_WAYLAND = 5;

		class ScreenWayland : public ScreenBackend
		{
			public:
						 ScreenWayland();
						~ScreenWayland();

				GUI::Rect	 GetActiveScreenMetrics();
				GUI::Rect	 GetActiveScreenWorkArea();

				GUI::Rect	 GetVirtualScreenMetrics();
		};
	};
};

#endif
