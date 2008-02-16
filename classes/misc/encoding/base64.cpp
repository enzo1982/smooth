 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/encoding/base64.h>

S::Encoding::Base64::Base64(Buffer<UnsignedByte> &iBuffer) : buffer(iBuffer)
{
}

S::Encoding::Base64::~Base64()
{
}

S::String S::Encoding::Base64::Encode(Int bytes) const
{
	if (bytes > buffer.Size()) return NIL;
	if (bytes == -1) bytes = buffer.Size();

	String	 b64;

	for (Int i = 0; i < bytes; i += 3)
	{
		Int	 val[4] = { 64, 64, 64, 64 };

				   val[0] = 						      buffer[i + 0]      >> 2;
				   val[1] = ((buffer[i + 0] &  3) << 4) | (((bytes > i + 1) ? buffer[i + 1] : 0) >> 4);
		if (bytes > i + 1) val[2] = ((buffer[i + 1] & 15) << 2) | (((bytes > i + 2) ? buffer[i + 2] : 0) >> 6);
		if (bytes > i + 2) val[3] =   buffer[i + 2] & 63;

		for (Int j = 0; j < 4; j++)
		{
			if (val[j] >= 0 && val[j] <= 25)	b64[i / 3 * 4 + j] = 'A' + val[j] - 0;
			else if (val[j] >= 26 && val[j] <= 51)	b64[i / 3 * 4 + j] = 'a' + val[j] - 26;
			else if (val[j] >= 52 && val[j] <= 61)	b64[i / 3 * 4 + j] = '0' + val[j] - 52;
			else if (val[j] == 62)			b64[i / 3 * 4 + j] = '+';
			else if (val[j] == 63)			b64[i / 3 * 4 + j] = '/';
			else if (val[j] == 64)			b64[i / 3 * 4 + j] = '=';
		}
	}

	return b64;
}

S::Int S::Encoding::Base64::Decode(const String &string)
{
	buffer.Resize(string.Length() / 4 * 3 + 1);

	Int	 bytes = 0;

	for (Int i = 0; i < string.Length(); i += 4)
	{
		Int	 val[4];

		for (Int j = 0; j < 4; j++)
		{
			if (string[i + j] >= 'A' && string[i + j] <= 'Z')	val[j] = string[i + j] - 'A' + 0;
			else if (string[i + j] >= 'a' && string[i + j] <= 'z')	val[j] = string[i + j] - 'a' + 26;
			else if (string[i + j] >= '0' && string[i + j] <= '9')	val[j] = string[i + j] - '0' + 52;
			else if (string[i + j] == '+')				val[j] = 62;
			else if (string[i + j] == '/')				val[j] = 63;
			else if (string[i + j] == '=')				val[j] = 64;
		}

				  buffer[bytes++] = ( val[0]       << 2) | (val[1] >> 4);
		if (val[2] != 64) buffer[bytes++] = ((val[1] & 15) << 4) | (val[2] >> 2);
		if (val[3] != 64) buffer[bytes++] = ((val[2] &  3) << 6) | (val[3]);

		buffer[bytes] = 0;
	}

	return bytes;
}
