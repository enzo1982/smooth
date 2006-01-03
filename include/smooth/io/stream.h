 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_STREAM_
#define _H_OBJSMOOTH_STREAM_

namespace smooth
{
	namespace IO
	{
		class Stream;
		class Driver;
		class Filter;
	};
};

#include "../definitions.h"
#include "io.h"

namespace smooth
{
	namespace IO
	{
		class SMOOTHAPI Stream
		{
			protected:
				static Int		 defaultPackageSize;

				Int			 streamType;
				Bool			 crosslinked;
				Bool			 closefile;

				Int64			 size;
				Int64			 origsize;

				UnsignedByte		*data;

				Int			 packageSize;
				Int			 stdpacksize;
				Int			 origpacksize;
				Bool			 allowpackset;

				Int64			 currentFilePos;
				Int			 currentBufferPos;
				Int64			 origfilepos;

				Bool			 pbdActive;
				Bool			 keepPbd;
				Int			 pbdLength;
				Bool			 pbdBuffer[128];

				Driver			*driver;
				Array<Filter *, Void *>	 filters;

				Int			 lastError;
			public:
				static Bool		 SetDefaultPackageSize	(Int);

				Int64			 Size			();

				Int64			 GetPos			();
				Int			 GetStreamType		();
				Int			 GetLastError		();

							 Stream			();
							~Stream			();
		};
	};
};

#endif
