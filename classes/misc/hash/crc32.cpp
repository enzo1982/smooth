 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/hash/crc32.h>

S::UnsignedInt32	 S::Hash::CRC32::table[256];
S::Bool			 S::Hash::CRC32::initialized = False;

S::Hash::CRC32::CRC32()
{
	Reset();
}

S::Hash::CRC32::~CRC32()
{
}

S::Void S::Hash::CRC32::InitTable()
{
	UnsignedInt32	 polynomial = 0x04C11DB7;

	for (Int i = 0; i <= 0xFF; i++)
	{
		table[i] = Reflect(i, 8) << 24;

		for (Int j = 0; j < 8; j++) table[i] = (table[i] << 1) ^ (table[i] & (1 << 31) ? polynomial : 0);

		table[i] = Reflect(table[i], 32);
	}

	initialized = True;
}

S::UnsignedInt32 S::Hash::CRC32::Reflect(UnsignedInt32 ref, char ch)
{
	UnsignedInt32	 value(0);

	for (Int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1) value |= 1 << (ch - i);

		ref >>= 1;
	}

	return value;
}

S::Bool S::Hash::CRC32::Reset()
{
	if (!initialized) InitTable();

	crc = 0xFFFFFFFF;

	return True;
}

S::Bool S::Hash::CRC32::Feed(const UnsignedByte *data, Int size)
{
	while (size--) crc = (crc >> 8) ^ table[(crc & 0xFF) ^ *data++];

	return True;
}

S::Bool S::Hash::CRC32::Feed(const Buffer<UnsignedByte> &data)
{
	return Feed(data, data.Size());
}

S::UnsignedInt32 S::Hash::CRC32::Finish()
{
	return crc ^ 0xFFFFFFFF;
}

S::UnsignedInt32 S::Hash::CRC32::Compute(const UnsignedByte *data, Int size)
{
	CRC32	 crc32;

	crc32.Feed(data, size);

	return crc32.Finish();
}

S::UnsignedInt32 S::Hash::CRC32::Compute(const Buffer<UnsignedByte> &data)
{
	return Compute(data, data.Size());
}
