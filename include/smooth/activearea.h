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
	class ActiveArea;
};

#include "object.h"

namespace smooth
{
	class SMOOTHAPI ActiveArea : public Object
	{
		protected:
			Int		 areaColor;
		public:
					 ActiveArea(Int, Point, Size, ProcParam, Void *);
					~ActiveArea();

			virtual Int	 Paint(Int);
			Int		 Process(Int, Int, Int);

			Int		 SetColor(Int);
			Int		 GetColor();
	};

	SMOOTHVAR Int OBJ_ACTIVEAREA;
};

#endif
