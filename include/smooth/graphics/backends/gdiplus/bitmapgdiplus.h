 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BITMAPGDIPLUS
#define H_OBJSMOOTH_BITMAPGDIPLUS

#include <windows.h>
#include <gdiplus.h>

namespace smooth
{
	namespace GUI
	{
		class BitmapGDIPlus;
	};
};

#include "../bitmapbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 BITMAP_GDIPLUS = 5;

		class BitmapGDIPlus : public BitmapBackend
		{
			private:
				Gdiplus::Bitmap		*bitmap;
				mutable HBITMAP		 hBitmap;
			public:
							 BitmapGDIPlus(Void * = NIL);
							 BitmapGDIPlus(const Size &, Int);
							 BitmapGDIPlus(const int);
							 BitmapGDIPlus(const BitmapGDIPlus &);

							~BitmapGDIPlus();

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
