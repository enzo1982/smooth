 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TREEENTRY_
#define __OBJSMOOTH_TREEENTRY_

#include <smooth/tree.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>

SMOOTHTree::Entry::Entry(SMOOTHInt et, SMOOTHInt ec)
{
	type		= et;
	code		= ec;
	sub		= NIL;
	proc		= NIL;
	procParam	= NIL;
	chk		= SMOOTH::False;
	clk		= SMOOTH::False;
	open		= SMOOTH::False;
	last		= SMOOTH::False;
}

SMOOTHTree::Entry::~Entry()
{
}

#endif
