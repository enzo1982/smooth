 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SIZE_
#define _H_OBJSMOOTH_SIZE_

namespace smooth
{
	class Size;
};

#include "primitive.h"

namespace smooth
{
	class SMOOTHAPI Size : public Primitive
	{
		public:
#ifdef __WIN32__
			operator	 SIZE();
			Size &operator	 =(const SIZE);
#endif
			Int		 cx;
			Int		 cy;
					 Size();
					 Size(Int, Int);
	};
};

#endif
