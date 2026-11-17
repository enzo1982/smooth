 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SURFACEWAYLAND
#define H_OBJSMOOTH_SURFACEWAYLAND

namespace smooth
{
	namespace GUI
	{
		class SurfaceWayland;
		class WindowWayland;
	};
};

#include "../cairo/surfacecairobase.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 SURFACE_WAYLAND = 7;

		/* Cairo based surface that lets the window present
		 * damaged regions of the backbuffer.
		 */
		class SurfaceWayland : public SurfaceCairoBase
		{
			private:
				static Short		 surfaceDPI;
			protected:
				WindowWayland		*window;

				cairo_surface_t		*CreateBackbufferSurface(const Size &);
			public:
							 SurfaceWayland(Void * = NIL, const Size & = Size());
							~SurfaceWayland();

				Int			 PaintRect(const Rect &);

				Void			*GetSystemSurface() const;

				Short			 GetSurfaceDPI() const;
		};
	};
};

#endif
