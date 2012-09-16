 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDPHOTON
#define H_OBJSMOOTH_BACKENDPHOTON

namespace smooth
{
	namespace Backends
	{
		class BackendPhoton;
	};
};

#include "../backend.h"

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_PHOTON = 5;

		class BackendPhoton : public Backend
		{
			public:
						 BackendPhoton();
				virtual		~BackendPhoton();

				Int		 Init();
		};
	};
};

#endif
