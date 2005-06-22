 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BACKENDWIN32_
#define _H_OBJSMOOTH_BACKENDWIN32_

namespace smooth
{
	namespace Backends
	{
		class BackendWin32;
	};
};

#include "../backend.h"

namespace smooth
{
	namespace Backends
	{
		const Int	 BACKEND_WIN32 = 2;

		class SMOOTH_STATIC_EXPORT BackendWin32 : public Backend
		{
			public:
					 BackendWin32();
				virtual	~BackendWin32();

				Int	 Init();
				Int	 Deinit();
		};
	};
};

#endif
