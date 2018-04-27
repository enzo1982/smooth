 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/hash/crc16.h>

S::UnsignedInt16	 S::Hash::CRC16::table[8][256];
S::Bool			 S::Hash::CRC16::initialized = InitTable();

S::Hash::CRC16::CRC16()
{
	Reset();
}

S::Hash::CRC16::~CRC16()
{
}

S::Bool S::Hash::CRC16::InitTable()
{
	UnsignedInt16	 polynomial = 0x8005;

	for (Int i = 0; i <= 0xFF; i++)
	{
		UnsignedInt16	 value = Reflect(i, 8) << 8;

		for (Int j = 0; j < 8; j++) value = (value << 1) ^ (value & (1 << 15) ? polynomial : 0);

		table[0][i] = Reflect(value, 16);
	}

	for (Int i = 0; i <= 0xFF; i++)
	{
		for (Int j = 1; j < 8; j++) table[j][i] = table[0][table[j - 1][i] & 0xFF] ^ (table[j - 1][i] >> 8);
	}

	return True;
}

S::UnsignedInt16 S::Hash::CRC16::Reflect(UnsignedInt16 ref, char ch)
{
	UnsignedInt16	 value(0);

	for (Int i = 1; i < (ch + 1); i++)
	{
		if (ref & 1) value |= 1 << (ch - i);

		ref >>= 1;
	}

	return value;
}

S::Bool S::Hash::CRC16::Reset()
{
	crc = 0x0000;

	return True;
}

S::Bool S::Hash::CRC16::Feed(const UnsignedByte *data, Int size)
{
	while (size >= 8)
	{
		crc  ^= data[1] << 8 | data[0];

		crc   =	table[7][crc & 0xFF] ^ table[6][crc >> 8] ^
			table[5][data[2]   ] ^ table[4][data[3]	] ^
			table[3][data[4]   ] ^ table[2][data[5]	] ^
			table[1][data[6]   ] ^ table[0][data[7]	];

		data += 8;
		size -= 8;
	}

	while (size--) crc = (crc >> 8) ^ table[0][(crc & 0xFF) ^ *data++];

	return True;
}

S::Bool S::Hash::CRC16::Feed(const Buffer<UnsignedByte> &data)
{
	return Feed(data, data.Size());
}

S::UnsignedInt16 S::Hash::CRC16::Finish()
{
	return crc;
}

S::UnsignedInt16 S::Hash::CRC16::Compute(const UnsignedByte *data, Int size)
{
	CRC16	 crc16;

	crc16.Feed(data, size);

	return crc16.Finish();
}

S::UnsignedInt16 S::Hash::CRC16::Compute(const Buffer<UnsignedByte> &data)
{
	return Compute(data, data.Size());
}
