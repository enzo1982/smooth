 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAP_
#define _H_OBJSMOOTH_BITMAP_

namespace smooth
{
	namespace GUI
	{
		class Surface;
		class Bitmap;
		class BitmapBackend;
	};
};

#include "../definitions.h"
#include "../primitives/size.h"
#include "../primitives/rect.h"
#include "../string.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Bitmap
		{
			private:
				BitmapBackend		*backend;
			public:
							 Bitmap(Void * = NIL);
							 Bitmap(Int, Int, Int);
							 Bitmap(const int);
							 Bitmap(const Bitmap &);

							~Bitmap();

				Int			 GetBitmapType();

				const Size		 GetSize();
				Int			 GetDepth();

				UnsignedByte		*GetBytes();
				Int			 GetLineAlignment();

				Bool			 CreateBitmap(Int, Int, Int);
				Bool			 DeleteBitmap();

				Bool			 SetSystemBitmap(Void *);
				Void			*GetSystemBitmap();

				Int			 GrayscaleBitmap();
				Int			 ReplaceColor(Int, Int);

				Int			 BlitFromSurface(Surface *, Rect, Rect);
				Int			 BlitToSurface(Rect, Surface *, Rect);

				Bool			 SetPixel(Int, Int, UnsignedLong);
				UnsignedLong		 GetPixel(Int, Int);

				Bitmap &operator	 =(const int);
				Bitmap &operator	 =(const Bitmap &);

				Bool operator		 ==(const int);
				Bool operator		 !=(const int);

				static Bitmap		 LoadBitmap(String, Int, String);
		};

		SMOOTHVAR S::GUI::Bitmap SI_DEFAULT;
	};
};

#endif
