 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

 /* This code is derived from the RSA Data Security,
  * Inc. MD5 Message-Digest Algorithm. */

#include <smooth/misc/hash/md5.h>
#include <smooth/misc/number.h>

/* Constants for MD5Transform routine.
 */
const S::Int	 S11 = 7, S12 = 12, S13 = 17, S14 = 22,
		 S21 = 5, S22 =  9, S23 = 14, S24 = 20,
		 S31 = 4, S32 = 11, S33 = 16, S34 = 23,
		 S41 = 6, S42 = 10, S43 = 15, S44 = 21;

/* MD5 padding bytes.
 */
static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & ( z)))
#define G(x, y, z) (((x) & (z)) | (( y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
   Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac)					\
	{								\
		(a) += F((b), (c), (d)) + (x) + (UnsignedInt32) (ac);	\
		(a) = ROTATE_LEFT((a), (s));				\
		(a) += (b);						\
	}

#define GG(a, b, c, d, x, s, ac)					\
	{								\
		(a) += G((b), (c), (d)) + (x) + (UnsignedInt32) (ac);	\
		(a) = ROTATE_LEFT((a), (s));				\
		(a) += (b);						\
	}

#define HH(a, b, c, d, x, s, ac)					\
	{								\
		(a) += H((b), (c), (d)) + (x) + (UnsignedInt32) (ac);	\
		(a) = ROTATE_LEFT((a), (s));				\
		(a) += (b);						\
	}

#define II(a, b, c, d, x, s, ac)					\
	{								\
		(a) += I((b), (c), (d)) + (x) + (UnsignedInt32) (ac);	\
		(a) = ROTATE_LEFT((a), (s));				\
		(a) += (b);						\
	}

/* GET_INT32 reads an integer from a buffer.
 */
#define GET_INT32(x, b) ((b)[4 * (x)	]	|			\
			 (b)[4 * (x) + 1] <<  8	|			\
			 (b)[4 * (x) + 2] << 16 |			\
			 (b)[4 * (x) + 3] << 24)

/* REVERSE_INT32 changes the byte order of an integer.
 */
#define REVERSE_INT32(x) (( (x)	       & 0xFF) << 24 |			\
			  (((x) >>  8) & 0xFF) << 16 |			\
			  (((x) >> 16) & 0xFF) <<  8 |			\
			   ((x) >> 24) & 0xFF )

S::Hash::MD5::MD5(const Buffer<UnsignedByte> &iBuffer) : buffer(iBuffer)
{
	memset(&state, 0, sizeof(state));
}

S::Hash::MD5::~MD5()
{
}

S::Void S::Hash::MD5::Init()
{
	/* Load magic initialization constants.
	 */
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

S::Void S::Hash::MD5::Transform(UnsignedByte *buffer)
{
	UnsignedInt32	 a = state[0];
	UnsignedInt32	 b = state[1];
	UnsignedInt32	 c = state[2];
	UnsignedInt32	 d = state[3];

	/* Round 1 */
	FF(a, b, c, d, GET_INT32( 0, buffer), S11, 0xd76aa478); /*  1 */
	FF(d, a, b, c, GET_INT32( 1, buffer), S12, 0xe8c7b756); /*  2 */
	FF(c, d, a, b, GET_INT32( 2, buffer), S13, 0x242070db); /*  3 */
	FF(b, c, d, a, GET_INT32( 3, buffer), S14, 0xc1bdceee); /*  4 */
	FF(a, b, c, d, GET_INT32( 4, buffer), S11, 0xf57c0faf); /*  5 */
	FF(d, a, b, c, GET_INT32( 5, buffer), S12, 0x4787c62a); /*  6 */
	FF(c, d, a, b, GET_INT32( 6, buffer), S13, 0xa8304613); /*  7 */
	FF(b, c, d, a, GET_INT32( 7, buffer), S14, 0xfd469501); /*  8 */
	FF(a, b, c, d, GET_INT32( 8, buffer), S11, 0x698098d8); /*  9 */
	FF(d, a, b, c, GET_INT32( 9, buffer), S12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, GET_INT32(10, buffer), S13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, GET_INT32(11, buffer), S14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, GET_INT32(12, buffer), S11, 0x6b901122); /* 13 */
	FF(d, a, b, c, GET_INT32(13, buffer), S12, 0xfd987193); /* 14 */
	FF(c, d, a, b, GET_INT32(14, buffer), S13, 0xa679438e); /* 15 */
	FF(b, c, d, a, GET_INT32(15, buffer), S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, GET_INT32( 1, buffer), S21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, GET_INT32( 6, buffer), S22, 0xc040b340); /* 18 */
	GG(c, d, a, b, GET_INT32(11, buffer), S23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, GET_INT32( 0, buffer), S24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, GET_INT32( 5, buffer), S21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, GET_INT32(10, buffer), S22, 0x02441453); /* 22 */
	GG(c, d, a, b, GET_INT32(15, buffer), S23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, GET_INT32( 4, buffer), S24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, GET_INT32( 9, buffer), S21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, GET_INT32(14, buffer), S22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, GET_INT32( 3, buffer), S23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, GET_INT32( 8, buffer), S24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, GET_INT32(13, buffer), S21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, GET_INT32( 2, buffer), S22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, GET_INT32( 7, buffer), S23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, GET_INT32(12, buffer), S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, GET_INT32( 5, buffer), S31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, GET_INT32( 8, buffer), S32, 0x8771f681); /* 34 */
	HH(c, d, a, b, GET_INT32(11, buffer), S33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, GET_INT32(14, buffer), S34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, GET_INT32( 1, buffer), S31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, GET_INT32( 4, buffer), S32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, GET_INT32( 7, buffer), S33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, GET_INT32(10, buffer), S34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, GET_INT32(13, buffer), S31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, GET_INT32( 0, buffer), S32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, GET_INT32( 3, buffer), S33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, GET_INT32( 6, buffer), S34, 0x04881d05); /* 44 */
	HH(a, b, c, d, GET_INT32( 9, buffer), S31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, GET_INT32(12, buffer), S32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, GET_INT32(15, buffer), S33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, GET_INT32( 2, buffer), S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, GET_INT32( 0, buffer), S41, 0xf4292244); /* 49 */
	II(d, a, b, c, GET_INT32( 7, buffer), S42, 0x432aff97); /* 50 */
	II(c, d, a, b, GET_INT32(14, buffer), S43, 0xab9423a7); /* 51 */
	II(b, c, d, a, GET_INT32( 5, buffer), S44, 0xfc93a039); /* 52 */
	II(a, b, c, d, GET_INT32(12, buffer), S41, 0x655b59c3); /* 53 */
	II(d, a, b, c, GET_INT32( 3, buffer), S42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, GET_INT32(10, buffer), S43, 0xffeff47d); /* 55 */
	II(b, c, d, a, GET_INT32( 1, buffer), S44, 0x85845dd1); /* 56 */
	II(a, b, c, d, GET_INT32( 8, buffer), S41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, GET_INT32(15, buffer), S42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, GET_INT32( 6, buffer), S43, 0xa3014314); /* 59 */
	II(b, c, d, a, GET_INT32(13, buffer), S44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, GET_INT32( 4, buffer), S41, 0xf7537e82); /* 61 */
	II(d, a, b, c, GET_INT32(11, buffer), S42, 0xbd3af235); /* 62 */
	II(c, d, a, b, GET_INT32( 2, buffer), S43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, GET_INT32( 9, buffer), S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

S::Void S::Hash::MD5::Final()
{
	UnsignedInt64	 bits = buffer.Size() * 8;
	Int		 index = buffer.Size() % 64;

	/* Pad out to 56 mod 64.
	 */
	Int		 padLen	= (index < 56) ? (56 - index) : (120 - index);

	UnsignedByte	*end = new UnsignedByte [128];

	memcpy(end, buffer + buffer.Size() - index, index);
	memcpy(end + index, PADDING, padLen);

	end[index + padLen    ] = bits	     & 0xFF;
	end[index + padLen + 1] = bits >>  8 & 0xFF;
	end[index + padLen + 2] = bits >> 16 & 0xFF;
	end[index + padLen + 3] = bits >> 24 & 0xFF;
	end[index + padLen + 4] = bits >> 32 & 0xFF;
	end[index + padLen + 5] = bits >> 40 & 0xFF;
	end[index + padLen + 6] = bits >> 48 & 0xFF;
	end[index + padLen + 7] = bits >> 56 & 0xFF;

	Transform(end);

	if (padLen > 56) Transform(end + 64);

	delete [] end;
}

S::String S::Hash::MD5::Compute()
{
	Init();

	for (Int i = 0; i + 63 < buffer.Size(); i += 64) Transform(buffer + i);

	Final();

	String	 string;

	for (Int i = 0; i < 4; i++) string.Append(Number((Int64) REVERSE_INT32(state[i])).ToHexString(8));

	return string;
}
