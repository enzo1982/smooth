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
		class Bitmap;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Bitmap : public Widget
		{
			protected:
				HBITMAP			 bitmap;
			public:
				static const Int	 classID;

							 Bitmap(HBITMAP, Point, Size);
							~Bitmap();

				virtual Int		 Paint(Int);

				Int			 SetBitmap(HBITMAP);
				HBITMAP			 GetBitmap();
		};
	};
};

#endif
