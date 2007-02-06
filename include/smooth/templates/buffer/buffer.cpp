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

template <class t> S::Bool S::Buffer<t>::Resize(Int nSize)
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

	return True;
}

template <class t> S::Bool S::Buffer<t>::Zero()
{
	if (memory == NIL) return True;

	memset(memory, 0, size * sizeof(t));

	return True;
}

template <class t> S::Bool S::Buffer<t>::Free()
{
	if (memory == NIL) return True;

	delete memory_manager;

	memory_manager	= NIL;
	memory		= NIL;

	size		= 0;
	allocated	= 0;

	return True;
}

#endif
