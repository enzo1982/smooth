 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_INSTANCE_
#define _H_INSTANCE_

namespace smooth
{
	class Instance;
};

#include "method.h"

namespace smooth
{
	class Instance
	{
		public:
			virtual ~Instance()
			{
			}

			virtual Void		 Call(Method *) = 0;
			virtual Instance	*Copy() = 0;
	};
};

#endif
