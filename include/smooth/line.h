 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LINE_
#define _H_OBJSMOOTH_LINE_

namespace smooth
{
	class Line;
};

#include "primitive.h"
#include "point.h"

namespace smooth
{
	class SMOOTHAPI Line : public Primitive
	{
		public:
			Int	 sx;
			Int	 sy;
			Int	 ex;
			Int	 ey;
				 Line();
				 Line(Point, Point);
	};
};

#endif
