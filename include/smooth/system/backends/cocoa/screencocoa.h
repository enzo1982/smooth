 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SCREENCOCOA
#define H_OBJSMOOTH_SCREENCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace System
	{
		class ScreenCocoa;
	};
};

#include "../screenbackend.h"

namespace smooth
{
	namespace System
	{
		const Short	 SCREEN_COCOA = 3;

		class ScreenCocoa : public ScreenBackend
		{
			private:
				static NSScreen	*ScreenFromPoint(const GUI::Point &);
			public:
						 ScreenCocoa();
						~ScreenCocoa();

				GUI::Rect	 GetActiveScreenMetrics();
				GUI::Rect	 GetActiveScreenWorkArea();

				GUI::Rect	 GetVirtualScreenMetrics();
		};
	};
};

#endif
