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
		class SurfaceBackend;
	};
};

#include "../definitions.h"
#include "../primitives/rect.h"
#include "../string.h"
#include "font.h"
#include "bitmap.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Surface
		{
			private:
				SurfaceBackend	*backend;
			public:
						 Surface(Void * = NIL);
						~Surface();

				Int		 GetSurfaceType();

				Int		 SetSize(Size);
				const Size	 GetSize();

				Int		 PaintRect(Rect);

				Int		 StartPaint(Rect);
				Int		 EndPaint();

				Void		*GetSystemSurface();

				Int		 SetPixel(Int, Int, Int);
				Int		 GetPixel(Int, Int);

				Int		 Line(Point, Point, Int);
				Int		 Frame(Rect, Int);
				Int		 Box(Rect, Int, Int);

				Int		 SetText(String, Rect, Font, Bool = False);

				Int		 Gradient(Rect, Int, Int, Int);
				Int		 Bar(Point, Point, Int);

				Int		 BlitFromBitmap(const Bitmap &, Rect, Rect);
				Int		 BlitToBitmap(Rect, const Bitmap &, Rect);
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
