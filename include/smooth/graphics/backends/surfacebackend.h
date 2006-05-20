 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

#include "../../definitions.h"
#include "../forms/rect.h"
#include "../../misc/string.h"
#include "../font.h"
#include "../bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_NONE = 0;

		class SurfaceBackend
		{
			private:
				static Array<SurfaceBackend *(*)(Void *), Void *>	*backend_creators;
			protected:
				Int							 type;

				Size							 size;
				Rect							 paintRect;

				Bool							 rightToLeft;

				Int							 painting;

				Int							 TranslateX(Int);
				Int							 TranslateY(Int);
				Point							 TranslatePoint(const Point &);
				Rect							 TranslateRect(const Rect &);
			public:
				static Int						 AddBackend(SurfaceBackend *(*)(Void *));

				static SurfaceBackend					*CreateBackendInstance(Void * = NIL);

											 SurfaceBackend(Void * = NIL);
				virtual							~SurfaceBackend();

				Int							 GetSurfaceType();

				Int							 SetSize(const Size &);
				const Size						&GetSize();

				Int							 SetRightToLeft(Bool);

				virtual Int						 PaintRect(const Rect &);

				virtual Int						 StartPaint(const Rect &);
				virtual Int						 EndPaint();

				virtual Void						*GetSystemSurface();

				virtual Int						 SetPixel(Int, Int, Int);
				virtual Int						 GetPixel(Int, Int);

				virtual Int						 Line(const Point &, const Point &, Int);
				virtual Int						 Frame(const Rect &, Int);
				virtual Int						 Box(const Rect &, Int, Int);

				virtual Int						 SetText(const String &, const Rect &, const Font &, Bool);

				Int							 Gradient(const Rect &, Int, Int, Int);
				Int							 Bar(const Point &, const Point &, Int);

				virtual Int						 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				virtual Int						 BlitToBitmap(const Rect &, const Bitmap &, const Rect &);
		};
	};
};

#endif
