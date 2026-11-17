 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SURFACECAIROBASE
#define H_OBJSMOOTH_SURFACECAIROBASE

namespace smooth
{
	namespace GUI
	{
		class SurfaceCairoBase;
	};
};

#include "../surfacebackend.h"

#include <cairo/cairo.h>

namespace smooth
{
	namespace GUI
	{
		/* Common base class for Cairo based surfaces. Drawing operations
		 * render into a backbuffer and changed areas are copied to the
		 * window using PaintRect.
		 */
		class SurfaceCairoBase : public SurfaceBackend
		{
			protected:
				Size				 allocSize;

				Array<Rect>			 paintRects;

				cairo_surface_t			*paintSurface;
				cairo_t				*paintContext;

				Void				 CreateBackbuffer(const Size &);
				Void				 DestroyBackbuffer();

				/* Create a Cairo surface to be used as the backbuffer.
				 */
				virtual cairo_surface_t		*CreateBackbufferSurface(const Size &) = 0;
			public:
								 SurfaceCairoBase();
				virtual				~SurfaceCairoBase();

				Int				 SetSize(const Size &);
				const Size			&GetSize() const;

				Int				 StartPaint(const Rect &);
				Int				 EndPaint();

				Int				 SetPixel(const Point &, const Color &);

				Int				 Line(const Point &, const Point &, const Color &);
				Int				 Box(const Rect &, const Color &, Int, const Size &);

				Int				 SetText(const String &, const Rect &, const Font &, Bool);

				Int				 Gradient(const Rect &, const Color &, const Color &, Int);

				Int				 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				Int				 BlitToBitmap(const Rect &, Bitmap &, const Rect &);
		};
	};
};

#endif
