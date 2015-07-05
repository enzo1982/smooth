 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDCOCOA
#define H_OBJSMOOTH_BACKENDCOCOA

#ifdef __OBJC__
#	include <Cocoa/Cocoa.h>
#else
	class NSAutoReleasePool;
#endif

namespace smooth
{
	namespace Backends
	{
		class BackendCocoa;
	};
};

#include "../backend.h"

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_COCOA = 3;

		class BackendCocoa : public Backend
		{
			private:
				NSAutoreleasePool	*pool;
			public:
				static Bool		 IsOSXVersionAtLeast(Int = 10, Int = 5, Int = 0);

							 BackendCocoa();
				virtual			~BackendCocoa();

				Int			 Init();
				Int			 Deinit();
		};
	};
};

#endif
