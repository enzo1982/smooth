 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SCREENWIN32
#define H_OBJSMOOTH_SCREENWIN32

#include <windows.h>

namespace smooth
{
	namespace System
	{
		class ScreenWin32;
	};
};

#include "../screenbackend.h"

namespace smooth
{
	namespace System
	{
		const Short	 SCREEN_WIN32 = 1;

		class ScreenWin32 : public ScreenBackend
		{
			public:
				static Int	 Initialize();
				static Int	 Free();

						 ScreenWin32();
						~ScreenWin32();

				GUI::Rect	 GetActiveScreenMetrics();
				GUI::Rect	 GetActiveScreenWorkArea();

				GUI::Rect	 GetVirtualScreenMetrics();
		};
	};
};

#endif
