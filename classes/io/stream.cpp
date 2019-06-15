 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/stream.h>

S::IO::Stream::Stream()
{
	streamType	 = STREAM_NONE;
	size		 = 0;

	currentFilePos	 = 0;
	currentBufferPos = 0;

	bitstreamActive	 = False;
	keepBits	 = False;
	bitLength	 = 0;

	closefile	 = True;
	crosslinked	 = False;

	driver		 = NIL;
	filter		 = NIL;

	allowpackset	 = True;
	packageSize	 = 1;
	stdpacksize	 = packageSize;
	origpacksize	 = packageSize;
	origsize	 = 0;
	origfilepos	 = 0;

	lastError	 = IO_ERROR_OK;

	memset(&bitBuffer, 0, sizeof(bitBuffer));
}

S::IO::Stream::~Stream()
{
}
