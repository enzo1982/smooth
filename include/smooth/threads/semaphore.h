 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SEMAPHORE_
#define _H_OBJSMOOTH_SEMAPHORE_

namespace smooth
{
	namespace Threads
	{
		class Semaphore;
		class SemaphoreBackend;
	};
};

#include "../object.h"

namespace smooth
{
	namespace Threads
	{
		class SMOOTHAPI Semaphore : public Object
		{
			private:
				SemaphoreBackend	*backend;
			public:
				static const Int	 classID;

							 Semaphore(Void * = NIL);
							~Semaphore();

				Int			 GetSemaphoreType();

				Void			*GetSystemSemaphore();

				Int			 Wait();
				Int			 Release();
		};
	};
};

#endif
