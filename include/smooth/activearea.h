 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ACTIVEAREA_
#define _H_OBJSMOOTH_ACTIVEAREA_

namespace smooth
{
	namespace GUI
	{
		class ActiveArea;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ActiveArea : public Widget
		{
			protected:
				Int			 areaColor;
			public:
				static const Int	 classID;

							 ActiveArea(Int, Point, Size);
							~ActiveArea();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetColor(Int);
				Int			 GetColor();
		};
	};
};

#endif
