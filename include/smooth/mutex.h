 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MUTEX_
#define _H_OBJSMOOTH_MUTEX_

namespace smooth
{
	class Mutex;
};

#include "object.h"

namespace smooth
{
	class SMOOTHAPI Mutex : public Object
	{
		private:
			LiSAMutex		*mutex;
		public:
			static const Int	 classID;

						 Mutex();
						~Mutex();

			Int			 Lock();
			Int			 Release();
	};
};

#endif
