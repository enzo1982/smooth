 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/tree.h>
#include <smooth/definitions.h>

S::Tree::Entry::Entry(Int et, Int ec)
{
	type	= et;
	id	= ec;
	sub	= NIL;

	chk	= False;
	clk	= False;

	open	= False;
	last	= False;
}

S::Tree::Entry::~Entry()
{
}
