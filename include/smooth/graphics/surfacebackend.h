 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SURFACEBACKEND_
#define _H_OBJSMOOTH_SURFACEBACKEND_

namespace smooth
{
	namespace GUI
	{
		class SurfaceBackend;
	};
};

#include "../definitions.h"
#include "rect.h"
#include "../misc/string.h"
#include "font.h"
#include "bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_NONE = 0;

		class SurfaceBackend
		{
			private:
				static Array<SurfaceBackend *(*)(Void *)>	*backend_creators;
			protected:
				Int						 type;

				Size						 size;
				Rect						 paintRect;

				Int						 painting;

				Int						 TranslateX(Int);
				Int						 TranslateY(Int);
				Point						 TranslatePoint(Point);
				Rect						 TranslateRect(Rect);
			public:
				static Int					 AddBackend(SurfaceBackend *(*)(Void *));

				static SurfaceBackend				*CreateBackendInstance(Void * = NIL);

										 SurfaceBackend(Void * = NIL);
				virtual						~SurfaceBackend();

				Int						 GetSurfaceType();

				Int						 SetSize(Size);
				const Size					 GetSize();

				virtual Int					 PaintRect(Rect);

				virtual Int					 StartPaint(Rect);
				virtual Int					 EndPaint();

				virtual Void					*GetSystemSurface();

				virtual Int					 SetPixel(Int, Int, Int);
				virtual Int					 GetPixel(Int, Int);

				virtual Int					 Line(Point, Point, Int);
				virtual Int					 Frame(Rect, Int);
				virtual Int					 Box(Rect, Int, Int);

				virtual Int					 SetText(String, Rect, Font, Bool);

				Int						 Gradient(Rect, Int, Int, Int);
				Int						 Bar(Point, Point, Int);

				virtual Int					 BlitFromBitmap(const Bitmap &, Rect, Rect);
				virtual Int					 BlitToBitmap(Rect, const Bitmap &, Rect);
		};
	};
};

#endif
