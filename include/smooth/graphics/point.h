 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POINT_
#define _H_OBJSMOOTH_POINT_

namespace smooth
{
	namespace GUI
	{
		class Point;
	};
};

#include "form.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Point : public Form
		{
			public:
#ifdef __WIN32__
				operator	 POINT();
				Point &operator	 =(const POINT &);
#endif
				Int		 x;
				Int		 y;

						 Point();
						 Point(Int, Int);

				Point operator	 +(const Point &);
				Point operator	 -(const Point &);

				Point operator	 *(const Float);
				Point operator	 /(const Float);

				Bool operator	 ==(const Point &);
				Bool operator	 !=(const Point &);
		};
	};
};

#endif
