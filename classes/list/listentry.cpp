 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/list.h>
#include <smooth/definitions.h>

S::List::Entry::Entry(Int entryCode)
{
	code		= entryCode;

	proc		= NIL;
	procParam	= NIL;

	chk		= False;
	clk		= False;
}

S::List::Entry::~Entry()
{
}
