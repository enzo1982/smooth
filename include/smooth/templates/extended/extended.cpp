 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __EXTENDED_
#define __EXTENDED_

#include "extended.h"

template <class t> S::Extended<t>::Extended(const t &iValue)
{
	beforeWrite.Emit(iValue);

	var = iValue;

	onWrite.Emit(iValue);
}

template <class t> t &S::Extended<t>::operator =(t nValue)
{
	beforeWrite.Emit(nValue);

	var = nValue;

	onWrite.Emit(nValue);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator ++()
{
	beforeWrite.Emit(var++);
	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t S::Extended<t>::operator ++(int)
{
	t	 oValue = var;

	beforeWrite.Emit(var++);
	onWrite.Emit(var);

	onRead.Emit(var);

	return oValue;
}

template <class t> t &S::Extended<t>::operator --()
{
	beforeWrite.Emit(var--);
	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t S::Extended<t>::operator --(int)
{
	t	 oValue = var;

	beforeWrite.Emit(var--);
	onWrite.Emit(var);

	onRead.Emit(var);

	return oValue;
}

template <class t> S::Bool S::Extended<t>::operator ==(t cValue)
{
	onRead.Emit(var);

	return var == cValue;
}

template <class t> S::Bool S::Extended<t>::operator !=(t cValue)
{
	onRead.Emit(var);

	return var != cValue;
}

template <class t> S::Bool S::Extended<t>::operator <(t cValue)
{
	onRead.Emit(var);

	return var < cValue;
}

template <class t> S::Bool S::Extended<t>::operator >(t cValue)
{
	onRead.Emit(var);

	return var > cValue;
}

template <class t> S::Bool S::Extended<t>::operator <=(t cValue)
{
	onRead.Emit(var);

	return var <= cValue;
}

template <class t> S::Bool S::Extended<t>::operator >=(t cValue)
{
	onRead.Emit(var);

	return var >= cValue;
}

template <class t> t &S::Extended<t>::operator +=(t nValue)
{
	beforeWrite.Emit(var + nValue);

	var += nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator -=(t nValue)
{
	beforeWrite.Emit(var - nValue);

	var -= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator *=(t nValue)
{
	beforeWrite.Emit(var * nValue);

	var *= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator /=(t nValue)
{
	beforeWrite.Emit(var / nValue);

	var /= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator %=(t nValue)
{
	beforeWrite.Emit(var % nValue);

	var %= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator &=(t nValue)
{
	beforeWrite.Emit(var & nValue);

	var &= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator ^=(t nValue)
{
	beforeWrite.Emit(var ^ nValue);

	var ^= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator |=(t nValue)
{
	beforeWrite.Emit(var | nValue);

	var |= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator <<=(t nValue)
{
	beforeWrite.Emit(var << nValue);

	var <<= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> t &S::Extended<t>::operator >>=(t nValue)
{
	beforeWrite.Emit(var >> nValue);

	var >>= nValue;

	onWrite.Emit(var);

	onRead.Emit(var);

	return var;
}

template <class t> S::Extended<t>::operator t()
{
	onRead.Emit(var);

	return var;
}

#endif
