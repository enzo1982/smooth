 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CRC64
#define H_OBJSMOOTH_CRC64

namespace smooth
{
	namespace Hash
	{
		class CRC64;
	};
};

#include "../string.h"
#include "../../templates/buffer.h"

namespace smooth
{
	namespace Hash
	{
		class SMOOTHAPI CRC64
		{
			private:
				static UnsignedInt64	 table[256];
				static Bool		 initialized;

				UnsignedInt64		 crc;

				static Void		 InitTable();
			public:
				static UnsignedInt64	 Compute(const UnsignedByte *, Int);
				static UnsignedInt64	 Compute(const Buffer<UnsignedByte> &);

							 CRC64();
							~CRC64();

				Bool			 Reset();

				Bool			 Feed(const UnsignedByte *, Int);
				Bool			 Feed(const Buffer<UnsignedByte> &);

				UnsignedInt64		 Finish();
		};
	};
};

#endif
