 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POINT_
#define _H_OBJSMOOTH_POINT_

namespace smooth
{
	class Point;
};

#include "primitive.h"

namespace smooth
{
	class SMOOTHAPI Point : public Primitive
	{
		public:
#ifdef __WIN32__
			operator	 POINT();
			Point &operator	 =(const POINT);
#endif
			Int		 x;
			Int		 y;
					 Point();
					 Point(Int, Int);
	};
};

#endif
