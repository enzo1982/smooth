 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_SIZE_
#define _H_OBJSMOOTH_SIZE_

namespace smooth
{
	namespace GUI
	{
		class Size;
	};
};

#include "form.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Size : public Form
		{
			public:
#ifdef __WIN32__
				operator	 SIZE();
				Size &operator	 =(const SIZE &);
#endif
				Int		 cx;
				Int		 cy;

						 Size();
						 Size(Int, Int);

				Size operator	 +(const Size &);
				Size operator	 -(const Size &);

				Size operator	 *(const Float);
				Size operator	 /(const Float);

				Bool operator	 ==(const Size &);
				Bool operator	 !=(const Size &);
		};
	};
};

#endif
