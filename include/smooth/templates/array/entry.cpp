 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ARRAY_ENTRY_
#define __OBJSMOOTH_ARRAY_ENTRY_

#include "entry.h"

template <class t> S::Array_Entry<t>::Array_Entry()
{
	Clear();

	nextEntry = ARRAY_NULLPOINTER;
	prevEntry = ARRAY_NULLPOINTER;
}

template <class t> S::Array_Entry<t>::~Array_Entry()
{
	Clear();
}

template <class t> S::Bool S::Array_Entry<t>::Fill(t entry)
{
	value	= entry;
	filled	= True;

	if (index == -1) index = 0;

	return True;
}

template <class t> S::Bool S::Array_Entry<t>::Clear()
{
	value	= ARRAY_NULLVALUE;
	filled	= False;

	index	= -1;

	return True;
}

template <class t> S::Bool S::Array_Entry<t>::SetIndex(Int nIndex)
{
	if (!filled) return False;

	index = nIndex;

	return True;
}

#endif
