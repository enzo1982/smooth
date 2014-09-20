 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPCOCOA
#define H_OBJSMOOTH_BITMAPCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace GUI
	{
		class BitmapCocoa;
	};
};

#include "../bitmapbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 BITMAP_COCOA = 4;

		class BitmapCocoa : public BitmapBackend
		{
			private:
				NSBitmapImageRep	*bitmap;

				mutable NSImage		*image;

				Void			 Initialize();
			public:
							 BitmapCocoa(Void * = NIL);
							 BitmapCocoa(const Size &, Int);
							 BitmapCocoa(const int);
							 BitmapCocoa(const BitmapCocoa &);

							~BitmapCocoa();

				Bool			 CreateBitmap(const Size &, Int);
				Bool			 DeleteBitmap();

				Bitmap			 Scale(const Size &) const;

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
