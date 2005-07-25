 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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

template <class t> S::Buffer<t>::Buffer()
{
	memory		= NIL;

	size		= 0;
	allocated	= 0;
}

template <class t> S::Buffer<t>::Buffer(Int iSize)
{
	memory		= new t [iSize];

	size		= iSize;
	allocated	= iSize;
}

template <class t> S::Buffer<t>::Buffer(const Buffer<t> &oBuffer)
{
	memory		= oBuffer.memory;

	size		= oBuffer.size;
	allocated	= oBuffer.allocated;
}

template <class t> S::Buffer<t>::~Buffer()
{
	Free();
}

template <class t> S::Int S::Buffer<t>::Size()
{
	return size;
}

template <class t> S::Int S::Buffer<t>::Resize(Int nSize)
{
	if (nSize > allocated) Free();

	if (memory == NIL)
	{
		memory		= new t [nSize];

		size		= nSize;
		allocated	= nSize;
	}
	else
	{
		size		= nSize;
	}

	return Success;
}

template <class t> S::Int S::Buffer<t>::Zero()
{
	if (memory == NIL) return Success;

	memset(memory, 0, size * sizeof(t));

	return Success;
}

template <class t> S::Int S::Buffer<t>::Free()
{
	if (memory == NIL) return Success;

	delete [] memory;

	memory		= NIL;

	size		= 0;
	allocated	= 0;

	return Success;
}

template <class t> t &S::Buffer<t>::operator [](int n)
{
	return memory[n];
}

template <class t> t &S::Buffer<t>::operator [](Int n)
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

#endif
