 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SEMAPHOREWIN32_
#define _H_OBJSMOOTH_SEMAPHOREWIN32_

namespace smooth
{
	namespace Threads
	{
		class SemaphoreWin32;
	};
};

#include "../semaphorebackend.h"

#include <winnt.h>

namespace smooth
{
	namespace Threads
	{
		const Int	 SEMAPHORE_WIN32	= 2;

		class SMOOTH_STATIC_EXPORT SemaphoreWin32 : public SemaphoreBackend
		{
			protected:
				HANDLE	 semaphore;
				Bool	 mySemaphore;
			public:
					 SemaphoreWin32(Void * = NIL);
					~SemaphoreWin32();

				Void	*GetSystemSemaphore();

				Int	 Wait();
				Int	 Release();
		};
	};
};

#endif
