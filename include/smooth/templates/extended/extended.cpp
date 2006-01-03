 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_EXTENDED_
#define __OBJSMOOTH_EXTENDED_

#include "extended.h"

template <class t> S::Extended<t>::Extended()
{
	onRead.Connect(&Extended<t>::DefaultRead, this);
	onWrite.Connect(&Extended<t>::DefaultWrite, this);
}

template <class t> S::Extended<t>::Extended(const t &iValue)
{
	onRead.Connect(&Extended::DefaultRead, this);
	onWrite.Connect(&Extended::DefaultWrite, this);

	t	 value = iValue;

	var = onWrite.CallUnprotected(value);
}

template <class t> t &S::Extended<t>::DefaultRead(t &value)
{
	return value;
}

template <class t> t &S::Extended<t>::DefaultWrite(t &value)
{
	return value;
}

template <class t> t &S::Extended<t>::operator =(t &nValue)
{
	var = onWrite.CallUnprotected(nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator ++()
{
	var = onWrite.CallUnprotected(++var);

	return onRead.CallUnprotected(var);
}

template <class t> t S::Extended<t>::operator ++(int)
{
	t	 oValue = onRead.CallUnprotected(var);

	var = onWrite.CallUnprotected(++var);

	return oValue;
}

template <class t> t &S::Extended<t>::operator --()
{
	var = onWrite.CallUnprotected(--var);

	return onRead.CallUnprotected(var);
}

template <class t> t S::Extended<t>::operator --(int)
{
	t	 oValue = onRead.CallUnprotected(var);

	var = onWrite.CallUnprotected(--var);

	return oValue;
}

template <class t> S::Bool S::Extended<t>::operator ==(t &cValue)
{
	return onRead.CallUnprotected(var) == onWrite.CallUnprotected(cValue);
}

template <class t> S::Bool S::Extended<t>::operator !=(t &cValue)
{
	return onRead.CallUnprotected(var) != onWrite.CallUnprotected(cValue);
}

template <class t> S::Bool S::Extended<t>::operator <(t &cValue)
{
	return onRead.CallUnprotected(var) < onWrite.CallUnprotected(cValue);
}

template <class t> S::Bool S::Extended<t>::operator >(t &cValue)
{
	return onRead.CallUnprotected(var) > onWrite.CallUnprotected(cValue);
}

template <class t> S::Bool S::Extended<t>::operator <=(t &cValue)
{
	return onRead.CallUnprotected(var) <= onWrite.CallUnprotected(cValue);
}

template <class t> S::Bool S::Extended<t>::operator >=(t &cValue)
{
	return onRead.CallUnprotected(var) >= onWrite.CallUnprotected(cValue);
}

template <class t> t &S::Extended<t>::operator +=(t &nValue)
{
	var = onWrite.CallUnprotected(var + nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator -=(t &nValue)
{
	var = onWrite.CallUnprotected(var - nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator *=(t &nValue)
{
	var = onWrite.CallUnprotected(var * nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator /=(t &nValue)
{
	var = onWrite.CallUnprotected(var / nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator %=(t &nValue)
{
	var = onWrite.CallUnprotected(var % nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator &=(t &nValue)
{
	var = onWrite.CallUnprotected(var & nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator ^=(t &nValue)
{
	var = onWrite.CallUnprotected(var ^ nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator |=(t &nValue)
{
	var = onWrite.CallUnprotected(var | nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator <<=(t &nValue)
{
	var = onWrite.CallUnprotected(var << nValue);

	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator >>=(t &nValue)
{
	var = onWrite.CallUnprotected(var >> nValue);

	return onRead.CallUnprotected(var);
}

template <class t> S::Extended<t>::operator t()
{
	return onRead.CallUnprotected(var);
}

template <class t> t &S::Extended<t>::operator *()
{
	return onRead.CallUnprotected(var);
}

template <class t> t *S::Extended<t>::operator ->()
{
	t	&value = onRead.CallUnprotected(var);

	return &value;
}

#endif
