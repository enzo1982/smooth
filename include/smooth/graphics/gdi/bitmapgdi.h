 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAPGDI_
#define _H_OBJSMOOTH_BITMAPGDI_

namespace smooth
{
	namespace GUI
	{
		class BitmapGDI;
	};
};

#include "../bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 BITMAP_GDI = 1;

		class SMOOTHAPI BitmapGDI : public Bitmap
		{
			private:
				HBITMAP			 bitmap;
			public:
				static const Int	 classID;

							 BitmapGDI(HBITMAP = NIL);
							~BitmapGDI();

				Bool			 SetBitmap(HBITMAP);
				HBITMAP			 GetBitmap();
		};
	};
};

#endif
