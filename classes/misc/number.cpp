 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/number.h>
#include <smooth/misc/math.h>

static const S::String	 kZero	= "0";
static const S::String	 kMinus	= "-";
static const S::String	 kPoint	= ".";

S::Number::Number(const Int64 iNumber)
{
	intValue   = iNumber;
	floatValue = iNumber;
}

S::Number::Number(const Float iNumber)
{
	intValue   = (Int64) iNumber;
	floatValue = iNumber;
}

S::Number::Number(const Number &iNumber)
{
	*this = iNumber;
}

S::Number::~Number()
{
}

S::Number::operator S::Int64 () const
{
	return intValue;
}

S::Number::operator S::Float () const
{
	return floatValue;
}

S::Number &S::Number::operator =(const Int64 number)
{
	return *this = Number(number);
}

S::Number &S::Number::operator =(const Float number)
{
	return *this = Number(number);
}

S::Number &S::Number::operator =(const Number &number)
{
	intValue   = number.intValue;
	floatValue = number.floatValue;

	return *this;
}

S::Bool S::Number::operator ==(const Int64 number) const
{
	return *this == Number(number);
}

S::Bool S::Number::operator ==(const Float number) const
{
	return *this == Number(number);
}

S::Bool S::Number::operator ==(const Number &number) const
{
	if (intValue   != number.intValue ||
	    floatValue != number.floatValue) return False;
	else				     return True;
}

S::Bool S::Number::operator !=(const Int64 number) const
{
	return *this != Number(number);
}

S::Bool S::Number::operator !=(const Float number) const
{
	return *this == Number(number);
}

S::Bool S::Number::operator !=(const Number &number) const
{
	return !(*this == number);
}

S::Number S::Number::FromIntString(const String &string)
{
	Int	 first = string[0];
	Int	 sign  = (first == '-' ? -1 : 1);
	Int	 skip  = 0;

	if (first == '-' || first == '+') skip = 1;

	Int	 length = string.Length();
	Int	 digits = 0;

	for (Int i = skip; i < length; i++)
	{
		Int	 digit = string[i];

		if ((digit >= '0' && digit <= '9')) digits++;
		else				    break;
	}

	Int64	 value = 0;

	for (Int i = 0; i < digits; i++)
	{
		value += Int64(string[skip + i] - '0') * (Int64) Math::Pow(10, digits - i - 1);
	}

	return value * sign;
}

S::Number S::Number::FromFloatString(const String &string)
{
	Int	 first = string[0];
	Int	 sign  = (first == '-' ? -1 : 1);
	Int	 skip  = 0;

	if (first == '-' || first == '+') skip = 1;

	Int	 length = string.Length();
	Int	 digits = 0;

	for (Int i = skip; i < length; i++)
	{
		Int	 digit = string[i];

		if ((digit >= '0' && digit <= '9')) digits++;
		else				    break;
	}

	Int	 digitsafp = 0;

	if (string[skip + digits] == '.')
	{
		for (Int i = skip + digits + 1; i < length; i++)
		{
			Int	 digit = string[i];

			if ((digit >= '0' && digit <= '9')) digitsafp++;
			else				    break;
		}
	}

	Float	 value = Number::FromIntString(string);

	for (Int i = 0; i < digitsafp; i++)
	{
		value += Float(string[skip + digits + 1 + i] - '0') * Math::Pow(10, 0 - i - 1);
	}

	return value * sign;
}

S::Number S::Number::FromHexString(const String &string)
{
	Int	 first  = string[0];
	Int	 second = string[1];
	Int	 skip	= 0;

	if (first == '0' && (second == 'x' || second == 'X')) skip = 2;

	Int	 length = string.Length();
	Int	 digits = 0;

	for (Int i = skip; i < length; i++)
	{
		Int	 digit = string[i];

		if ((digit >= '0' && digit <= '9') ||
		    (digit >= 'A' && digit <= 'F') ||
		    (digit >= 'a' && digit <= 'f')) digits++;
		else				    break;
	}

	Int64	 value = 0;

	for (Int i = 0; i < digits; i++)
	{
		Int	 digit = string[skip + i];

		if	(digit >= '0' && digit <= '9') value += (Int64(digit - '0')	 << ((digits - i - 1) * 4));
		else if (digit >= 'A' && digit <= 'F') value += (Int64(digit - 'A' + 10) << ((digits - i - 1) * 4));
		else if (digit >= 'a' && digit <= 'f') value += (Int64(digit - 'a' + 10) << ((digits - i - 1) * 4));
	}

	return value;
}

S::String S::Number::ToIntString() const
{
	if (intValue == 0) return kZero;

	String	 string;
	Int	 length = (Int) Math::Log10(Math::Abs(intValue)) + 1;

	for (Int i = 0; i < length; i++)
	{
		string[i] = '0' + Int64(Math::Abs(intValue) / Math::Pow(10, length - i - 1)) % 10;
	}

	if (intValue < 0) string = String(kMinus).Append(string);

	return string;
}

S::String S::Number::ToFloatString() const
{
	Int	 digits = 10;
	Int64	 fract	= (Int64) (Math::Fract(Math::Abs(floatValue)) * Math::Pow(10, digits));
	Int	 lead	= digits - Math::Floor((fract > 0 ? Math::Log10(fract) : -1) + 1.000000001);

	Int	 nOfNull = 0;

	for (Int i = 0; i < digits; i++)
	{
		if (Int64(fract / Math::Pow(10, i)) % 10 == 0)	nOfNull++;
		else						break;
	}

	Int	 nOfNine = 0;

	for (Int i = 0; i < digits; i++)
	{
		if (Int64(fract / Math::Pow(10, i)) % 10 == 9)	nOfNine++;
		else						break;
	}

	if	(nOfNull > 0) fract = (Int64) (fract / Math::Pow(10, nOfNull));
	else if (nOfNine > 0) fract = (Int64) (fract / Math::Pow(10, nOfNine)) + 1;

	String	 string = Number(Math::Floor(Math::Abs(floatValue))).ToIntString();

	if (nOfNine == digits)
	{
		string = Number(Math::Floor(Math::Abs(floatValue)) + 1).ToIntString();
	}
	else if (fract > 0)
	{
		string.Append(kPoint);

		for (Int i = 0; i < lead; i++) string.Append(kZero);

		string.Append(Number(fract).ToIntString());
	}

	if (floatValue < 0) string = String(kMinus).Append(string);

	return string;
}

S::String S::Number::ToHexString(Int length) const
{
	String	 string;
	Int	 stringPos = 0;

	for (Int i = 0; i < 16; i++)
	{
		if	(((intValue >> (4 * (15 - i))) & 15) >= 10) string[stringPos++] = 'a' + ((intValue >> (4 * (15 - i))) & 15) - 10;
		else if (((intValue >> (4 * (15 - i))) & 15) >=  1) string[stringPos++] = '0' + ((intValue >> (4 * (15 - i))) & 15);
		else if (stringPos			     >   0) string[stringPos++] = '0';
	}

	if (length != -1)
	{
		/* Add leading zeros if necessary.
		 */
		if (string.Length() < length) string = String().FillN('0', length - string.Length()).Append(string);

		/* Cut string if too long.
		 */
		if (string.Length() > length) string = string.Tail(length);
	}

	return string;
}
