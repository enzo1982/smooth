 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACE_
#define _H_OBJSMOOTH_SURFACE_

namespace smooth
{
	namespace GUI
	{
		class Surface;
	};
};

#include "../definitions.h"
#include "../rect.h"
#include "../string.h"
#include "font.h"
#include "bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_NULL = 0;

		class SMOOTHAPI Surface
		{
			protected:
				Int		 type;

				Size		 size;
				Rect		 paintRect;

				Int		 painting;

				Int		 TranslateX(Int);
				Point		 TranslatePoint(Point);
				Rect		 TranslateRect(Rect);
			public:
						 Surface();
				virtual		~Surface();

				Int		 GetSurfaceType();

				Int		 SetSize(Size);
				Size		 GetSize();

				virtual Int	 PaintRect(Rect);

				virtual Int	 StartPaint(Rect);
				virtual Int	 EndPaint();

				virtual Int	 SetPixel(Int, Int, Int);
				virtual Int	 GetPixel(Int, Int);

				virtual Int	 Line(Point, Point, Int);
				virtual Int	 Frame(Rect, Int);
				virtual Int	 Box(Rect, Int, Int);

				virtual Int	 SetText(String, Rect, Font);
				Int		 SetShadowedText(String, Rect, Font);

				Int		 Gradient(Rect, Int, Int, Int);
				Int		 Bar(Point, Point, Int);

				virtual Int	 BlitFromBitmap(Bitmap *, Rect, Rect);
				virtual Int	 BlitToBitmap(Rect, Bitmap *, Rect);
		};

		const Int	 FRAME_UP	= 0;
		const Int	 FRAME_DOWN	= 1;
		const Int	 FILLED		= 0;
		const Int	 OUTLINED	= 1;
		const Int	 OUTLINEDOTS	= 2;
		const Int	 INVERT		= 3;
	};
};

#endif
