 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIVIDER_
#define _H_OBJSMOOTH_DIVIDER_

namespace smooth
{
	namespace GUI
	{
		class Divider;
		class Client;
		class MDIClient;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Divider : public Widget
		{
			public:
				static const Int	 classID;

							 Divider(Int, Int);
							~Divider();

				Int			 SetPos(Int);
				Int			 GetPos();

				virtual Int		 Paint(Int);
		};
	};
};

#endif
