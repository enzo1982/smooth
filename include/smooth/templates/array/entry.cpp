 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __SMOOTHARRAY_ENTRY_
#define __SMOOTHARRAY_ENTRY_

#include "entry.h"

template <class t> S::Array_Entry<t>::Array_Entry()
{
	gotNext = False;
	gotPrev = False;
	nextEntry = ARRAY_NULLPOINTER;
	prevEntry = ARRAY_NULLPOINTER;
}

template <class t> S::Array_Entry<t>::~Array_Entry()
{
}

template <class t> S::Bool S::Array_Entry<t>::SetNext(Array_Entry<t> *entry)
{
	if (entry != ARRAY_NULLPOINTER)
	{
		gotNext = True;
		nextEntry = entry;
	}
	else
	{
		gotNext = False;
		nextEntry = ARRAY_NULLPOINTER;
	}

	return True;
}

template <class t> S::Bool S::Array_Entry<t>::SetPrev(Array_Entry<t> *entry)
{
	if (entry != ARRAY_NULLPOINTER)
	{
		gotPrev = True;
		prevEntry = entry;
	}
	else
	{
		gotPrev = False;
		prevEntry = ARRAY_NULLPOINTER;
	}

	return True;
}

template <class t> inline S::Array_Entry<t> *S::Array_Entry<t>::GetNext()
{
	if (gotNext)	return nextEntry;
	else		return ARRAY_NULLPOINTER;
}

template <class t> inline S::Array_Entry<t> *S::Array_Entry<t>::GetPrev()
{
	if (gotPrev)	return prevEntry;
	else		return ARRAY_NULLPOINTER;
}

template <class t> S::Array_Entry<t> *S::Array_Entry<t>::GetNext(const Array_Entry<t> *entry)
{
	if (entry->gotNext)	return entry->nextEntry;
	else			return ARRAY_NULLPOINTER;
}

template <class t> S::Array_Entry<t> *S::Array_Entry<t>::GetPrev(const Array_Entry<t> *entry)
{
	if (entry->gotPrev)	return entry->prevEntry;
	else			return ARRAY_NULLPOINTER;
}

#endif
