 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SEMAPHOREBACKEND_
#define _H_OBJSMOOTH_SEMAPHOREBACKEND_

namespace smooth
{
	namespace Threads
	{
		class SemaphoreBackend;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace Threads
	{
		const Int	 SEMAPHORE_NONE = 0;

		class SemaphoreBackend
		{
			private:
				static Array<SemaphoreBackend *(*)(Void *)>	*backend_creators;
			protected:
				Int						 type;
			public:
				static Int					 AddBackend(SemaphoreBackend *(*)(Void *));

				static SemaphoreBackend				*CreateBackendInstance(Void * = NIL);

										 SemaphoreBackend(Void * = NIL);
				virtual						~SemaphoreBackend();

				Int						 GetSemaphoreType();

				virtual Void					*GetSystemSemaphore();

				virtual Int					 Wait();
				virtual Int					 Release();
		};
	};
};

#endif
