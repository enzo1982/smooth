 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SLIDER_
#define _H_OBJSMOOTH_SLIDER_

namespace smooth
{
	namespace GUI
	{
		class Slider;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Slider : public Widget
		{
			private:
				Int		 mouseBias;
				Int		 prevValue;
			protected:
				Int		*variable;

				Int		 startValue;
				Int		 endValue;
			public:
						 Slider(Point, Size, Int, Int *, Int, Int);
						~Slider();

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 SetRange(Int, Int);

				Int		 SetValue(Int);
				Int		 GetValue();
		};
	};

	SMOOTHVAR Int OBJ_SLIDER;
};

#endif
