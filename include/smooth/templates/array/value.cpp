 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __SMOOTHARRAY_VALUE_
#define __SMOOTHARRAY_VALUE_

#include "value.h"

template <class t> S::Array_Value<t>::Array_Value()
{
	filled = False;
	value = ARRAY_NULLVALUE;
	index = 0;
}

template <class t> S::Array_Value<t>::~Array_Value()
{
	Clear();
}

template <class t> S::Bool S::Array_Value<t>::Fill(t entry)
{
	value = entry;
	filled = True;

	return True;
}

template <class t> S::Bool S::Array_Value<t>::Clear()
{
	value = ARRAY_NULLVALUE;
	filled = False;

	return True;
}

template <class t> inline t S::Array_Value<t>::GetValue()
{
	if (filled)	return value;
	else		return ARRAY_NULLVALUE;
}

template <class t> inline S::Int S::Array_Value<t>::GetIndex()
{
	if (filled)	return index;
	else		return -1;
}

template <class t> S::Bool S::Array_Value<t>::SetIndex(Int ind)
{
	if (filled)
	{
		index = ind;
		return True;
	}
	else
	{
		return False;
	}
}

template <class t> t S::Array_Value<t>::GetValue(const Array_Value<t> *entry)
{
	if (entry->filled)	return entry->value;
	else			return ARRAY_NULLVALUE;
}

template <class t> S::Int S::Array_Value<t>::GetIndex(const Array_Value<t> *entry)
{
	if (entry->filled)	return entry->index;
	else			return -1;
}

#endif
