 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MUTEX_
#define _H_OBJSMOOTH_MUTEX_

namespace smooth
{
	namespace Threads
	{
		class Mutex;
		class MutexBackend;
	};
};

#include "../basic/object.h"

namespace smooth
{
	namespace Threads
	{
		class SMOOTHAPI Mutex : public Object
		{
			private:
				MutexBackend		*backend;
			public:
				static const Int	 classID;

							 Mutex(Void * = NIL);
							~Mutex();

				Int			 GetMutexType();

				Void			*GetSystemMutex();

				Int			 Lock();
				Int			 Release();
		};
	};
};

#endif
