 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __SMOOTHARRAY_ENTRY_
#define __SMOOTHARRAY_ENTRY_

#include "entry.h"

template <class t> SMOOTHArray_Entry<t>::SMOOTHArray_Entry()
{
	gotNext = false;
	gotPrev = false;
	nextEntry = ARRAY_NULLPOINTER;
	prevEntry = ARRAY_NULLPOINTER;
}

template <class t> SMOOTHArray_Entry<t>::~SMOOTHArray_Entry()
{
}

template <class t> bool SMOOTHArray_Entry<t>::SetNext(SMOOTHArray_Entry<t> *entry)
{
	if (entry != ARRAY_NULLPOINTER)
	{
		gotNext = true;
		nextEntry = entry;
		return true;
	}
	else
	{
		gotNext = false;
		nextEntry = ARRAY_NULLPOINTER;
		return true;
	}
}

template <class t> bool SMOOTHArray_Entry<t>::SetPrev(SMOOTHArray_Entry<t> *entry)
{
	if (entry != ARRAY_NULLPOINTER)
	{
		gotPrev = true;
		prevEntry = entry;
		return true;
	}
	else
	{
		gotPrev = false;
		prevEntry = ARRAY_NULLPOINTER;
		return true;
	}
}

template <class t> inline SMOOTHArray_Entry<t> *SMOOTHArray_Entry<t>::GetNext()
{
	if (gotNext)	return nextEntry;
	else		return ARRAY_NULLPOINTER;
}

template <class t> inline SMOOTHArray_Entry<t> *SMOOTHArray_Entry<t>::GetPrev()
{
	if (gotPrev)	return prevEntry;
	else		return ARRAY_NULLPOINTER;
}

#endif
