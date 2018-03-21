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

S::UnsignedInt16	 S::Hash::CRC16::table[256];
S::Bool			 S::Hash::CRC16::initialized = False;

S::Hash::CRC16::CRC16()
{
	Reset();
}

S::Hash::CRC16::~CRC16()
{
}

S::Void S::Hash::CRC16::InitTable()
{
	UnsignedInt16	 polynomial = 0x8005;

	for (Int i = 0; i <= 0xFF; i++)
	{
		table[i] = Reflect(i, 8) << 8;

		for (Int j = 0; j < 8; j++) table[i] = (table[i] << 1) ^ (table[i] & (1 << 15) ? polynomial : 0);

		table[i] = Reflect(table[i], 16);
	}

	initialized = True;
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
	if (!initialized) InitTable();

	crc = 0x0000;

	return True;
}

S::Bool S::Hash::CRC16::Feed(const UnsignedByte *data, Int size)
{
	while (size--) crc = (crc >> 8) ^ table[(crc & 0xFF) ^ *data++];

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
