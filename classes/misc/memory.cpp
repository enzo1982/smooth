 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/misc/memory.h>

S::Memory::Memory(Int size)
{
	memory = new UnsignedByte [size];
}

S::Memory::~Memory()
{
	delete [] memory;
}

S::Bool S::Memory::Resize(Int size)
{
	delete [] memory;

	memory = new UnsignedByte [size];

	return True;
}

S::Memory::operator void *() const
{
	return memory;
}
