 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_CHECKBOX_
#define _H_OBJSMOOTH_CHECKBOX_

namespace smooth
{
	namespace GUI
	{
		class CheckBox;
	};
};

#include "../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI CheckBox : public Widget
		{
			protected:
				Bool			*variable;
				Bool			 state;
			public:
				static const Int	 classID;

							 CheckBox(String, Point, Size, Bool *);
							~CheckBox();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);
		};
	};
};

#endif
