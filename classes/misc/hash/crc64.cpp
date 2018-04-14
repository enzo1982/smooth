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

S::UnsignedInt64	 S::Hash::CRC64::table[8][256];
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
		UnsignedInt64	 value = i;

		for (Int j = 0; j < 8; j++) value = (value >> 1) ^ (value & 1 ? polynomial : 0);

		table[0][i] = value;
	}

	for (Int i = 0; i <= 0xFF; i++)
	{
		for (Int j = 1; j < 8; j++) table[j][i] = table[0][table[j - 1][i] & 0xFF] ^ (table[j - 1][i] >> 8);
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
	while (size >= 8)
	{
		crc   =	table[7][data[0] ^ ( crc	& 0xFF)] ^ table[6][data[1] ^ ((crc >>  8) & 0xFF)] ^
			table[5][data[2] ^ ((crc >> 16) & 0xFF)] ^ table[4][data[3] ^ ((crc >> 24) & 0xFF)] ^
			table[3][data[4] ^ ((crc >> 32) & 0xFF)] ^ table[2][data[5] ^ ((crc >> 40) & 0xFF)] ^
			table[1][data[6] ^ ((crc >> 48) & 0xFF)] ^ table[0][data[7] ^  (crc >> 56)	  ];

		data += 8;
		size -= 8;
	}

	while (size--) crc = (crc >> 8) ^ table[0][(crc & 0xFF) ^ *data++];

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
