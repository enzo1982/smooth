 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MDICLIENT_
#define _H_OBJSMOOTH_MDICLIENT_

namespace smooth
{
	namespace GUI
	{
		class MDIClient;
	};
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MDIClient : public Widget
		{
			public:
				static const Int	 classID;

							 MDIClient();
							~MDIClient();

				virtual Int		 Paint(Int);
		};
	};
};

#endif
