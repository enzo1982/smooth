 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACEXLIB_
#define _H_OBJSMOOTH_SURFACEXLIB_

namespace smooth
{
	namespace GUI
	{
		class SurfaceXLib;
	};
};

#include "../surfacebackend.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_XLIB	= 2;

		class SMOOTH_STATIC_EXPORT SurfaceXLib : public SurfaceBackend
		{
			private:
				Display	*display;
			protected:
				Window	 window;
				Window	 rWindow;

				Pixmap	 bitmap;
			public:
					 SurfaceXLib(Void * = NIL);
					~SurfaceXLib();

				Int	 PaintRect(const Rect &);

				Int	 StartPaint(const Rect &);
				Int	 EndPaint();

				Void	*GetSystemSurface();

				Int	 SetPixel(Int, Int, Int);
				Int	 GetPixel(Int, Int);

				Int	 Line(const Point &, const Point &, Int);
				Int	 Box(const Rect &, Int, Int);

				Int	 SetText(String, const Rect &, const Font &, Bool);

				Int	 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				Int	 BlitToBitmap(const Rect &, const Bitmap &, const Rect &);
		};
	};
};

#endif
