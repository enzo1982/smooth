 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_HYPERLINK_
#define _H_OBJSMOOTH_HYPERLINK_

namespace smooth
{
	namespace GUI
	{
		class Hyperlink;
	};
};

#include "widget.h"
#include "graphics/bitmap.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Hyperlink : public Widget
		{
			protected:
				String			 linkURL;
				Bitmap			 linkBitmap;
			public:
				static const Int	 classID;

							 Hyperlink();
							 Hyperlink(String, const Bitmap &, String, Point, Size size = Size(0, 0));
							~Hyperlink();

				Int			 Hide();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Bitmap			&GetBitmap();
				String			 GetURL();

				Int			 SetText(String);
				Int			 SetBitmap(const Bitmap &);
				Int			 SetURL(String);
		};
	};
};

#endif
