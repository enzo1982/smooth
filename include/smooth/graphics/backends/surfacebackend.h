 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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

#include "../font.h"
#include "../bitmap.h"
#include "../modifiers/fontsize.h"
#include "../modifiers/righttoleft.h"
#include "../../misc/string.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 SURFACE_NONE = 0;

		class SurfaceBackend
		{
			private:
				static Array<SurfaceBackend *(*)(Void *, const Size &), Void *>	*backend_creators;
			protected:
				Int								 type;

				Size								 size;
				Rect								 paintRect;

				Int								 painting;

				FontSizeModifier						 fontSize;
				RightToLeftModifier						 rightToLeft;
			public:
				static Int							 AddBackend(SurfaceBackend *(*)(Void *, const Size &));

				static SurfaceBackend						*CreateBackendInstance(Void * = NIL, const Size & = Size());

												 SurfaceBackend(Void * = NIL, const Size & = Size());
				virtual								~SurfaceBackend();

				Int								 GetSurfaceType() const;

				virtual Int							 SetSize(const Size &);
				virtual const Size						&GetSize() const;

				Int								 SetRightToLeft(Bool);

				virtual Int							 PaintRect(const Rect &);

				virtual Int							 StartPaint(const Rect &);
				virtual Int							 EndPaint();

				virtual Void							*GetSystemSurface() const;

				virtual Int							 SetPixel(Int, Int, Int);
				virtual Int							 GetPixel(Int, Int) const;

				virtual Int							 Line(const Point &, const Point &, Int);
				virtual Int							 Frame(const Rect &, Int);
				virtual Int							 Box(const Rect &, Int, Int, const Size &);

				virtual Int							 SetText(const String &, const Rect &, const Font &, Bool);

				Int								 Gradient(const Rect &, Int, Int, Int);
				Int								 Bar(const Point &, const Point &, Int);

				virtual Int							 BlitFromBitmap(const Bitmap &, const Rect &, const Rect &);
				virtual Int							 BlitToBitmap(const Rect &, const Bitmap &, const Rect &);
		};
	};
};

#endif
