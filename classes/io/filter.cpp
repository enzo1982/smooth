 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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
	packageSize = 0;
}

S::IO::Filter::~Filter()
{
}

S::Int S::IO::Filter::GetPackageSize() const
{
	return packageSize;
}

S::Void S::IO::Filter::SetDriver(Driver *iDriver)
{
	driver = iDriver;
}

S::Bool S::IO::Filter::Activate()
{
	return true;
}

S::Bool S::IO::Filter::Deactivate()
{
	return true;
}

S::Int S::IO::Filter::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	return driver->WriteData(data, size);
}

S::Int S::IO::Filter::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size > (driver->GetSize() - driver->GetPos())) size = driver->GetSize() - driver->GetPos();

	data.Resize(size);

	return driver->ReadData(data, size);
}
