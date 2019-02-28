 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/filter.h>
#include <smooth/io/driver.h>

S::IO::Filter::Filter()
{
	driver	    = NIL;

	packageSize = 0;
}

S::IO::Filter::~Filter()
{
}

S::Bool S::IO::Filter::Activate()
{
	return True;
}

S::Bool S::IO::Filter::Deactivate()
{
	return True;
}

S::Int S::IO::Filter::WriteData(const Buffer<UnsignedByte> &data)
{
	if (driver == NIL) return -1;

	return driver->WriteData(data, data.Size());
}

S::Int S::IO::Filter::ReadData(Buffer<UnsignedByte> &data)
{
	if (driver == NIL) return -1;

	if (data.Size() > (driver->GetSize() - driver->GetPos())) data.Resize(driver->GetSize() - driver->GetPos());

	return driver->ReadData(data, data.Size());
}
