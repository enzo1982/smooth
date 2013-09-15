 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_SURFACECOCOA
#define H_OBJSMOOTH_SURFACECOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace GUI
	{
		class SurfaceCocoa;
	};
};

#include "../surfacebackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 SURFACE_COCOA	= 4;

		class SurfaceCocoa : public SurfaceBackend
		{
			private:
				static Short			 surfaceDPI;
			protected:
				Size				 allocSize;

				NSWindow			*window;

				NSImage				*paintBitmap;

				Array<Rect *, Void *>		 paintRects;
			public:
								 SurfaceCocoa(Void * = NIL, const Size & = Size());
								~SurfaceCocoa();

				Int				 SetSize(const Size &);
				const Size			&GetSize() const;

				Int				 PaintRect(const Rect &);

				Int				 StartPaint(const Rect &);
				Int				 EndPaint();

				Void				*GetSystemSurface() const;

				Short				 GetSurfaceDPI() const;

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
