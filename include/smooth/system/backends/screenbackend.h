 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SCREENBACKEND
#define H_OBJSMOOTH_SCREENBACKEND

namespace smooth
{
	namespace System
	{
		class Screen;
		class ScreenBackend;
	};
};

#include "../../definitions.h"
#include "../../graphics/forms/rect.h"

namespace smooth
{
	namespace System
	{
		const Short	 SCREEN_NONE = 0;

		class ScreenBackend
		{
			private:
				static ScreenBackend	*(*backend_creator)();
			protected:
				Short			 type;
			public:
				static Int		 SetBackend(ScreenBackend *(*)());
				static ScreenBackend	*CreateBackendInstance();

							 ScreenBackend();
				virtual			~ScreenBackend();

				Short			 GetScreenType() const;

				virtual GUI::Rect	 GetActiveScreenMetrics();
				virtual GUI::Rect	 GetActiveScreenWorkArea();

				virtual GUI::Rect	 GetVirtualScreenMetrics();
		};
	};
};

#endif
