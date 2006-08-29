 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_FILTER_
#define _H_OBJSMOOTH_FILTER_

namespace smooth
{
	namespace IO
	{
		class Filter;
		class Driver;
	};
};

#include "../definitions.h"
#include "../templates/buffer.h"
#include "io.h"

namespace smooth
{
	namespace IO
	{
		class SMOOTHAPI Filter
		{
			protected:
				Int		 packageSize;

				Driver		*driver;
			public:
						 Filter();
				virtual		~Filter();

				Int		 GetPackageSize() const;

				Void		 SetDriver(Driver *);

				virtual Bool	 Activate();
				virtual Bool	 Deactivate();

				virtual Int	 WriteData(Buffer<UnsignedByte> &, Int);
				virtual Int	 ReadData(Buffer<UnsignedByte> &, Int);
		};
	};
};

#endif
