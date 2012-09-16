 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPHAIKU
#define H_OBJSMOOTH_BITMAPHAIKU

namespace smooth
{
	namespace GUI
	{
		class BitmapHaiku;
	};
};

#include "../bitmapbackend.h"

#include <Bitmap.h>

namespace smooth
{
	namespace GUI
	{
		const Short	 BITMAP_HAIKU = 5;

		class BitmapHaiku : public BitmapBackend
		{
			private:
				BBitmap			*bitmap;
			public:
							 BitmapHaiku(Void * = NIL);
							 BitmapHaiku(Int, Int, Int);
							 BitmapHaiku(const int);
							 BitmapHaiku(const BitmapHaiku &);

							~BitmapHaiku();

				Bool			 CreateBitmap(Int, Int, Int);
				Bool			 DeleteBitmap();

				Bool			 SetSystemBitmap(Void *);
				Void			*GetSystemBitmap() const;

				BitmapBackend &operator	 =(const BitmapBackend &);

				Bool operator		 ==(const int) const;
				Bool operator		 !=(const int) const;
		};
	};
};

#endif
