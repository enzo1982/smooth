 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LINE_
#define _H_OBJSMOOTH_LINE_

namespace smooth
{
	namespace GUI
	{
		class Line;
	};
};

#include "form.h"
#include "point.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Line : public Form
		{
			public:
				Int		 sx;
				Int		 sy;
				Int		 ex;
				Int		 ey;

						 Line();
						 Line(const Point &, const Point &);

				Bool operator	 ==(const Line &);
				Bool operator	 !=(const Line &);
		};
	};
};

#endif
