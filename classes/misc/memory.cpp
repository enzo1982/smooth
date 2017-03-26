 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/misc/memory.h>

#include <stdlib.h>

S::Memory::Memory(Int size)
{
	memory = (UnsignedByte *) malloc(size);
}

S::Memory::~Memory()
{
	if (memory != NIL) free(memory);
}

S::Bool S::Memory::Resize(Int size)
{
	UnsignedByte	*oldMemory = memory;

	if ((memory = (UnsignedByte *) realloc(memory, size)) == NIL)
	{
		free(oldMemory);

		return False;
	}

	return True;
}
