 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Hyperlink : public Widget
		{
			protected:
				String			 linkURL;
				HBITMAP			 linkBitmap;
			public:
				static const Int	 classID;

							 Hyperlink();
							 Hyperlink(String, HBITMAP, String, Point, Size size = Size(0, 0));
							~Hyperlink();

				Int			 Hide();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				HBITMAP			 GetBitmap();
				String			 GetURL();

				Int			 SetText(String);
				Int			 SetBitmap(HBITMAP);
				Int			 SetURL(String);
		};
	};
};

#endif
