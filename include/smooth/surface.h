 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACE_
#define _H_OBJSMOOTH_SURFACE_

namespace smooth
{
	class Surface;
};

#include "definitions.h"
#include "rect.h"
#include "string.h"

namespace smooth
{
	class SMOOTHAPI Surface
	{
		protected:
			Size		 size;
		public:
					 Surface();
			virtual		~Surface();

			Size		 GetSize();

			virtual Int	 GetPixel(Int, Int);

			virtual Int	 SetPixel(Int, Int, Int);
			virtual Int	 Line(Point, Point, Int);
			virtual Int	 Frame(Rect, Int);
			virtual Int	 Box(Rect, Int, Int);

			virtual Int	 SetText(String, Rect, String, Int, Int, Int);

			virtual Int	 BlitFromBitmap(HBITMAP, Rect, Rect);
			virtual Int	 BlitToBitmap(Rect, HBITMAP, Rect);
	};
};

#endif
