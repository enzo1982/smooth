 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LISTENTRY_
#define __OBJSMOOTH_LISTENTRY_

#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>

SMOOTHList::Entry::Entry(SMOOTHInt entryCode)
{
	code		= entryCode;

	proc		= NIL;
	procParam	= NIL;

	chk		= SMOOTH::False;
	clk		= SMOOTH::False;
}

SMOOTHList::Entry::~Entry()
{
}

#endif
