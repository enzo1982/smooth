 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/foreach.h>

namespace smooth
{
	Array<Int>	 foreach_indices;
}

S::Void S::foreach_initIndex(const String &name)
{
	Int	 crc = name.ComputeCRC32();

	foreach_indices.Add(0, crc);
	foreach_indices.Set(crc, 0);
}

S::Int S::foreach_indexValue(const String &name)
{
	Int	 crc = name.ComputeCRC32();

	return foreach_indices.Get(crc);
}

S::Void S::foreach_incIndex(const String &name)
{
	Int	 crc = name.ComputeCRC32();

	foreach_indices.Set(crc, foreach_indices.Get(crc) + 1);
}
