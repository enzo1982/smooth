 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OPTIONBOX_
#define _H_OBJSMOOTH_OPTIONBOX_

namespace smooth
{
	namespace GUI
	{
		class OptionBox;
	};
};

#include "../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI OptionBox : public Widget
		{
			protected:
				Int			*variable;
				Int			 code;
				Bool			 state;
			public:
				static const Int	 classID;

							 OptionBox(String, Point, Size, Int *, Int);
							~OptionBox();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetText(const String &);
		};
	};
};

#endif
