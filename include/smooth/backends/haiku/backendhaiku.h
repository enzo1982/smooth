 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDHAIKU
#define H_OBJSMOOTH_BACKENDHAIKU

namespace smooth
{
	namespace Backends
	{
		class BackendHaiku;
	};
};

#include "../backend.h"

#include <Application.h>

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_HAIKU = 5;

		class BackendHaiku : public Backend
		{
			public:
							 BackendHaiku();
				virtual			~BackendHaiku();

				Int			 Init();
				Int			 Deinit();

				static BApplication	*GetApplication();
		};
	};
};

#endif
