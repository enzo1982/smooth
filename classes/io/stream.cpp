 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/stream.h>

S::Int	 S::IO::Stream::defaultPackageSize = 1048576;

S::IO::Stream::Stream()
{
	streamType		= STREAM_NONE;
	size			= 0;
	currentFilePos		= 0;
	currentBufferPos	= 0;
	pbdActive		= false;
	keepPbd			= false;
	pbdLength		= 0;
	closefile		= true;
	crosslinked		= false;
	driver			= NULL;
	allowpackset		= true;
	packageSize		= defaultPackageSize;
	stdpacksize		= packageSize;
	origpacksize		= packageSize;
	origsize		= 0;
	origfilepos		= 0;
	lastError		= IO_ERROR_OK;
}

S::IO::Stream::~Stream()
{
}

S::Bool S::IO::Stream::SetDefaultPackageSize(Int nDefaultPackageSize)
{
	if (nDefaultPackageSize > 0)
	{
		defaultPackageSize = nDefaultPackageSize;

		return true;
	}

	return false;
}
