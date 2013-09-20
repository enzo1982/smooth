 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/hash/sha1.h>
#include <smooth/misc/number.h>

/* SHA-1 padding bytes.
 */
static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

S::Hash::SHA1::SHA1(const Buffer<UnsignedByte> &iBuffer) : buffer(iBuffer)
{
	memset(&state, 0, sizeof(state));
}

S::Hash::SHA1::~SHA1()
{
}

S::Void S::Hash::SHA1::Init()
{
	/* Load magic initialization constants.
	 */
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	state[4] = 0xc3d2e1f0;
}

S::Void S::Hash::SHA1::Transform(UnsignedByte *buffer)
{
	UnsignedInt32	 word[80];

	for (Int j =  0; j < 16; j++) word[j] = buffer[j * 4 + 0] * 0x1000000 + buffer[j * 4 + 1] * 0x10000 + buffer[j * 4 + 2] * 0x100 + buffer[j * 4 + 3];
	for (Int j = 16; j < 80; j++) word[j] = ROTATE_LEFT((word[j - 3] ^ word[j - 8] ^ word[j - 14] ^ word[j - 16]), 1);

	UnsignedInt32	 a = state[0];
	UnsignedInt32	 b = state[1];
	UnsignedInt32	 c = state[2];
	UnsignedInt32	 d = state[3];
	UnsignedInt32	 e = state[4];

	for (int m = 0; m < 80; m++)
	{
		UnsignedInt32	 f = 0;
		UnsignedInt32	 k = 0;

		if	(m <= 19) { f = (b & c) | ((~b) & d);	     k = 0x5a827999; }
		else if (m <= 39) { f = b ^ c ^ d;		     k = 0x6ed9eba1; }
		else if (m <= 59) { f = (b & c) | (b & d) | (c & d); k = 0x8f1bbcdc; }
		else		  { f = b ^ c ^ d;		     k = 0xca62c1d6; }

		UnsignedInt32	 temp = (ROTATE_LEFT(a, 5) + f + e + k + word[m]) & 0xFFFFFFFF;

		e = d;
		d = c;
		c = ROTATE_LEFT(b, 30);
		b = a;

		a = temp;
	}

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
}

S::Void S::Hash::SHA1::Final()
{
	UnsignedInt64	 bits = buffer.Size() * 8;
	Int		 index = buffer.Size() % 64;

	/* Pad out to 56 mod 64.
	 */
	Int		 padLen	= (index < 56) ? (56 - index) : (120 - index);

	UnsignedByte	*end = new UnsignedByte [128];

	memcpy(end, buffer + buffer.Size() - index, index);
	memcpy(end + index, PADDING, padLen);

	for (Int i = 0; i < 8; i++) end[index + padLen + i] = bits >> 8 * (7 - i) & 0xFF;

	Transform(end);

	if (padLen > 56) Transform(end + 64);

	delete [] end;
}

S::String S::Hash::SHA1::Compute()
{
	Init();

	for (Int i = 0; i + 63 < buffer.Size(); i += 64) Transform(buffer + i);

	Final();

	String	 string;

	for (Int i = 0; i < 5; i++) string.Append(Number((Int64) ((UnsignedInt32 *) state)[i]).ToHexString(8));

	return string;
}
