 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/binary.h>
#include <smooth/misc/math.h>

S::Binary::Binary()
{
}

S::Binary::Binary(const Binary &)
{
}

S::Bool S::Binary::GetBit(Int n, UnsignedInt bit)
{
	return IsFlagSet(n, (Int) Math::Pow(2, (signed) bit));
}

S::Int S::Binary::SetBit(Int &n, UnsignedInt bit, Bool value)
{
	if (value)	return (n |= (Int) Math::Pow(2, (signed) bit));
	else		return (n = (n | (Int) Math::Pow(2, (signed) bit) ^ (Int) Math::Pow(2, (signed) bit)));
}

S::Int S::Binary::GetBits(Int n, UnsignedInt startBit, UnsignedInt endBit)
{
	Int	retVal = 0;

	if (startBit >= 64 || endBit >= 64) return -1;

	for (UnsignedInt i = startBit; i <= endBit; i++)
	{
		retVal += (Int) Math::Pow(2, (signed) i - (signed) startBit) * ((n >> i) & 1);
	}

	return retVal;
}

S::Int S::Binary::SetBits(Int &n, UnsignedInt startBit, UnsignedInt endBit, Int value)
{
	if (startBit >= 64 || endBit >= 64) return -1;

	for (UnsignedInt i = startBit; i <= endBit; i++)
	{
		SetBit(n, i, (value >> (i - startBit)) & 1);
	}

	return n;
}

S::Int S::Binary::And(Int a, Int b)
{
	return a & b;
}

S::Int S::Binary::Or(Int a, Int b)
{
	return a | b;
}

S::Int S::Binary::Xor(Int a, Int b)
{
	return a ^ b;
}

S::Int S::Binary::Not(Int a)
{
	return ~a;
}

S::Int S::Binary::ShiftL(Int n, Int s)
{
	return n << s;
}

S::Int S::Binary::ShiftR(Int n, Int s)
{
	return n >> s;
}

S::Bool S::Binary::IsFlagSet(Int n, Int flag)
{
	return ((n & flag) == flag);
}

S::Int S::Binary::SetFlag(Int &n, Int flag)
{
	return (n |= flag);
}
