 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_IMAGE_
#define _H_OBJSMOOTH_IMAGE_

namespace smooth
{
	namespace GUI
	{
		class Image;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Image : public Widget
		{
			protected:
				HBITMAP			 bitmap;
			public:
				static const Int	 classID;

							 Image(HBITMAP, Point, Size);
							~Image();

				virtual Int		 Paint(Int);

				Int			 SetBitmap(HBITMAP);
				HBITMAP			 GetBitmap();
		};
	};
};

#endif
