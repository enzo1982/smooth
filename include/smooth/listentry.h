 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTENTRY_
#define _H_OBJSMOOTH_LISTENTRY_

#include "object.h"
#include "rect.h"

namespace smooth
{
	class SMOOTHAPI ListEntry
	{
		public:
			Int		 id;
			String		 name;

			Bool		 chk;
			Bool		 clk;
			Int		 size;
			Bool		 sizeset;
			Rect		 rect;

					 ListEntry(Int);
					~ListEntry();
		signals:
			Signal0<Void>	 onClick;
	};
};

#endif
