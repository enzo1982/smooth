 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACEGDI_
#define _H_OBJSMOOTH_SURFACEGDI_

namespace smooth
{
	namespace GUI
	{
		class SurfaceGDI;
	};
};

#include "../surface.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_GDI	= 1;

		class SMOOTHAPI SurfaceGDI : public Surface
		{
			protected:
				HDC	 gdi_dc;
				HDC	 bmp_dc;
				HDC	 real_dc;

				HBITMAP	 cDc_bitmap;
			public:
					 SurfaceGDI(HDC);
					~SurfaceGDI();

				Int	 PaintRect(Rect);

				Int	 StartPaint(Rect);
				Int	 EndPaint();

				HDC	 GetContext();

				Int	 SetPixel(Int, Int, Int);
				Int	 GetPixel(Int, Int);

				Int	 Line(Point, Point, Int);
				Int	 Box(Rect, Int, Int);

				Int	 SetText(String, Rect, Font);

				Int	 BlitFromBitmap(const Bitmap &, Rect, Rect);
				Int	 BlitToBitmap(Rect, const Bitmap &, Rect);
		};
	};
};

#endif