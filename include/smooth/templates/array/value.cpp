 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __SMOOTHARRAY_VALUE_
#define __SMOOTHARRAY_VALUE_

#include "value.h"

template <class t> SMOOTHArray_Value<t>::SMOOTHArray_Value()
{
	filled = false;
	value = ARRAY_NULLVALUE;
	index = 0;
}

template <class t> SMOOTHArray_Value<t>::~SMOOTHArray_Value()
{
	Clear();
}

template <class t> bool SMOOTHArray_Value<t>::Fill(t entry)
{
	value = entry;
	filled = true;

	return true;
}

template <class t> bool SMOOTHArray_Value<t>::Clear()
{
	value = ARRAY_NULLVALUE;
	filled = false;

	return true;
}

template <class t> inline t SMOOTHArray_Value<t>::GetValue()
{
	if (filled)	return value;
	else		return ARRAY_NULLVALUE;
}

template <class t> inline long SMOOTHArray_Value<t>::GetIndex()
{
	if (filled)	return index;
	else		return -1;
}

template <class t> bool SMOOTHArray_Value<t>::SetIndex(long ind)
{
	if (filled)
	{
		index = ind;
		return true;
	}
	else
	{
		return false;
	}
}

#endif
