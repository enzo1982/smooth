 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CRC16
#define H_OBJSMOOTH_CRC16

namespace smooth
{
	namespace Hash
	{
		class CRC16;
	};
};

#include "../string.h"
#include "../../templates/buffer.h"

namespace smooth
{
	namespace Hash
	{
		class SMOOTHAPI CRC16
		{
			private:
				static UnsignedInt16	 table[256];
				static Bool		 initialized;

				UnsignedInt16		 crc;

				static Void		 InitTable();
				static UnsignedInt16	 Reflect(UnsignedInt16, char);
			public:
				static UnsignedInt16	 Compute(const UnsignedByte *, Int);
				static UnsignedInt16	 Compute(const Buffer<UnsignedByte> &);

							 CRC16();
							~CRC16();

				Bool			 Reset();

				Bool			 Feed(const UnsignedByte *, Int);
				Bool			 Feed(const Buffer<UnsignedByte> &);

				UnsignedInt16		 Finish();
		};
	};
};

#endif
