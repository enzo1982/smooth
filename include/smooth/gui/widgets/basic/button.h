 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BUTTON_
#define _H_OBJSMOOTH_BUTTON_

namespace smooth
{
	namespace GUI
	{
		class Button;
	};
};

#include "../widget.h"
#include "../../../graphics/bitmap.h"
#include "../../../graphics/color.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 BF_NORMAL		= 0;
		const Int	 BF_NOFRAME		= 1;
		const Int	 BF_SHOWHIGHLIGHT	= 2;

		class SMOOTHAPI Button : public Widget
		{
			private:
				Color			 backgroundColor;
			protected:
				Bitmap			 bitmap;
				Size			 bmpSize;
			public:
				static const Int	 classID;

							 Button(String, const Bitmap &, Point, Size);
							~Button();

				Int			 SetBackgroundColor(Int);

				virtual Int		 Paint(Int);
		};
	};
};

#endif
