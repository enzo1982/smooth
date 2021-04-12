 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_EVENTCOCOA
#define H_OBJSMOOTH_EVENTCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace System
	{
		class EventCocoa;
	};
};

#include "../eventbackend.h"

namespace smooth
{
	namespace System
	{
		const Short	 EVENT_COCOA = 3;

		class EventCocoa : public EventBackend
		{
			private:
				static Bool	 quitRequested;
			public:
				static Void	 RequestApplicationQuit();

						 EventCocoa();
						~EventCocoa();

				Bool		 ProcessNextEvent();
		};
	};
};

#endif
