 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/hash/crc64.h>

S::UnsignedInt64	 S::Hash::CRC64::table[256];
S::Bool			 S::Hash::CRC64::initialized = False;

S::Hash::CRC64::CRC64()
{
	Reset();
}

S::Hash::CRC64::~CRC64()
{
}

S::Void S::Hash::CRC64::InitTable()
{
	UnsignedInt64	 polynomial = 0x95AC9329AC4BC9B5ULL;

	for (Int i = 0; i <= 0xFF; i++)
	{
		table[i] = i;

		for (Int j = 0; j < 8; j++) table[i] = (table[i] >> 1) ^ (table[i] & 1 ? polynomial : 0);
	}

	initialized = True;
}

S::Bool S::Hash::CRC64::Reset()
{
	if (!initialized) InitTable();

	crc = 0xFFFFFFFFFFFFFFFFULL;

	return True;
}

S::Bool S::Hash::CRC64::Feed(const UnsignedByte *data, Int size)
{
	while (size--) crc = (crc >> 8) ^ table[(crc & 0xFF) ^ *data++];

	return True;
}

S::Bool S::Hash::CRC64::Feed(const Buffer<UnsignedByte> &data)
{
	return Feed(data, data.Size());
}

S::UnsignedInt64 S::Hash::CRC64::Finish()
{
	return crc ^ 0xFFFFFFFFFFFFFFFFULL;
}

S::UnsignedInt64 S::Hash::CRC64::Compute(const UnsignedByte *data, Int size)
{
	CRC64	 crc64;

	crc64.Feed(data, size);

	return crc64.Finish();
}

S::UnsignedInt64 S::Hash::CRC64::Compute(const Buffer<UnsignedByte> &data)
{
	return Compute(data, data.Size());
}
