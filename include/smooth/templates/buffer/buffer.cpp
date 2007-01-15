 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BUFFER_
#define __OBJSMOOTH_BUFFER_

#include "buffer.h"
#include "../../misc/memory.h"

template <class t> S::Buffer<t>::Buffer()
{
	memory_manager	= NIL;
	memory		= NIL;

	size		= 0;
	allocated	= 0;
}

template <class t> S::Buffer<t>::Buffer(Int iSize)
{
	memory_manager	= new Memory(iSize * sizeof(t));
	memory		= (t *) (void *) *memory_manager;

	size		= iSize;
	allocated	= iSize;
}

template <class t> S::Buffer<t>::Buffer(const Buffer<t> &oBuffer)
{
	memory_manager	= oBuffer.memory_manager;
	memory		= oBuffer.memory;

	size		= oBuffer.size;
	allocated	= oBuffer.allocated;
}

template <class t> S::Buffer<t>::~Buffer()
{
	Free();
}

template <class t> S::Int S::Buffer<t>::Size() const
{
	return size;
}

template <class t> S::Int S::Buffer<t>::Resize(Int nSize)
{
	if (nSize > allocated)
	{
		if (memory != NIL) memory_manager->Resize(nSize * sizeof(t));
		else		   memory_manager = new Memory(nSize * sizeof(t));

		memory		= (t *) (void *) *memory_manager;

		size		= nSize;
		allocated	= nSize;
	}
	else
	{
		size		= nSize;
	}

	return Success();
}

template <class t> S::Int S::Buffer<t>::Zero()
{
	if (memory == NIL) return Success();

	memset(memory, 0, size * sizeof(t));

	return Success();
}

template <class t> S::Int S::Buffer<t>::Free()
{
	if (memory == NIL) return Success();

	delete memory_manager;

	memory_manager	= NIL;
	memory		= NIL;

	size		= 0;
	allocated	= 0;

	return Success();
}

template <class t> t &S::Buffer<t>::operator [](int n)
{
	return memory[n];
}

template <class t> t &S::Buffer<t>::operator [](Int n)
{
	return (*this)[(int) n];
}

template <class t> t S::Buffer<t>::operator [](int n) const
{
	return memory[n];
}

template <class t> t S::Buffer<t>::operator [](Int n) const
{
	return (*this)[(int) n];
}

template <class t> t *S::Buffer<t>::operator +(const int offset)
{
	return memory + offset;
}

template <class t> S::Buffer<t>::operator t *()
{
	return memory;
}

template <class t> S::Buffer<t>::operator const t *() const
{
	return memory;
}

template <class t> S::String S::Buffer<t>::EncodeBase64(Int bytes) const
{
	if (bytes > size) return NIL;

	String	 b64;

	for (Int i = 0; i < bytes; i += 3)
	{
		Int	 val[4] = { 64, 64, 64, 64 };

				   val[0] = 									  ((unsigned char *) memory)[i + 0]	 >> 2;
				   val[1] = ((((unsigned char *) memory)[i + 0] &  3) << 4) | (((bytes > i + 1) ? ((unsigned char *) memory)[i + 1] : 0) >> 4);
		if (bytes > i + 1) val[2] = ((((unsigned char *) memory)[i + 1] & 15) << 2) | (((bytes > i + 2) ? ((unsigned char *) memory)[i + 2] : 0) >> 6);
		if (bytes > i + 2) val[3] =   ((unsigned char *) memory)[i + 2] & 63;

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

template <class t> S::Int S::Buffer<t>::DecodeBase64(const String &string)
{
	Resize(string.Length() / 4 * 3 + 1);

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

				  ((unsigned char *) memory)[bytes++] = ( val[0]       << 2) | (val[1] >> 4);
		if (val[2] != 64) ((unsigned char *) memory)[bytes++] = ((val[1] & 15) << 4) | (val[2] >> 2);
		if (val[3] != 64) ((unsigned char *) memory)[bytes++] = ((val[2] &  3) << 6) | (val[3]);

		((unsigned char *) memory)[bytes] = 0;
	}

	return bytes;
}

#endif
