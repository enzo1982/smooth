 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_BITMAPNONE_
#define _H_OBJSMOOTH_BITMAPNONE_

namespace smooth
{
	namespace GUI
	{
		class BitmapNone;
	};
};

#include "../bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 BITMAP_NONE = 0;

		class SMOOTHAPI BitmapNone : public BitmapBase
		{
			public:
							 BitmapNone();
							 BitmapNone(const int);
							 BitmapNone(const BitmapNone &);

							~BitmapNone();

				BitmapBase &operator	 =(const int);
				BitmapBase &operator	 =(const BitmapBase &);

				Bool operator		 ==(const int);
				Bool operator		 !=(const int);
		};
	};
};

#endif
