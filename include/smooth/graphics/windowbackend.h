 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOWBACKEND_
#define _H_OBJSMOOTH_WINDOWBACKEND_

namespace smooth
{
	namespace GUI
	{
		class WindowBackend;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 WINDOW_NULL = 0;

		class WindowBackend
		{
			protected:
				Int		 type;
			public:
						 WindowBackend(Void * = NIL);
				virtual		~WindowBackend();

				Int		 GetWindowType();

				virtual Void	*GetSystemWindow();
		};
	};
};

#endif
