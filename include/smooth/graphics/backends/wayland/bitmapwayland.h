 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPWAYLAND
#define H_OBJSMOOTH_BITMAPWAYLAND

namespace smooth
{
	namespace GUI
	{
		class BitmapWayland;
	};
};

#include "../bitmapbackend.h"

#include <cairo/cairo.h>

namespace smooth
{
	namespace GUI
	{
		const Short	 BITMAP_WAYLAND = 7;

		/* Memory bitmap backed by a Cairo image surface. The
		 * system bitmap is the cairo_surface_t pointer.
		 */
		class BitmapWayland : public BitmapBackend
		{
			private:
				cairo_surface_t		*bitmap;

				Void			 Initialize();
			public:
							 BitmapWayland(Void * = NIL);
							 BitmapWayland(const Size &, Int);
							 BitmapWayland(const int);
							 BitmapWayland(const BitmapWayland &);

							~BitmapWayland();

				Bool			 CreateBitmap(const Size &, Int);
				Bool			 DeleteBitmap();

				Bool			 SetSystemBitmap(Void *);
				Void			*GetSystemBitmap() const;

				Bool			 SetPixel(const Point &, const Color &);
				Color			 GetPixel(const Point &) const;

				BitmapBackend &operator	 =(const BitmapBackend &);

				Bool operator		 ==(const int) const;
				Bool operator		 !=(const int) const;
		};
	};
};

#endif
