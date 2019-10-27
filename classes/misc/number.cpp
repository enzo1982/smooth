 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/number.h>
#include <smooth/misc/math.h>

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

S::Number S::Number::FromIntString(const String &iString)
{
	String	 string = iString.ToLower();
	Int	 sign	= (string[0] == '-' ? -1 : 1);

	if (string[0] == '-' || string[0] == '+') string = string.Tail(string.Length() - 1);

	Int	 length = 0;

	for (Int i = 0; i < string.Length(); i++)
	{
		if	((string[i] >= '0' && string[i] <= '9')) length++;
		else						 break;
	}

	Int64	 value = 0;

	for (Int i = 0; i < length; i++)
	{
		value += Int64(string[i] - '0') * (Int64) Math::Pow(10, length - i - 1);
	}

	return value * sign;
}

S::Number S::Number::FromFloatString(const String &iString)
{
	String	 string = iString.ToLower();
	Int	 sign	= (string[0] == '-' ? -1 : 1);

	if (string[0] == '-' || string[0] == '+') string = string.Tail(string.Length() - 1);

	Int	 length = 0;

	for (Int i = 0; i < string.Length(); i++)
	{
		if	((string[i] >= '0' && string[i] <= '9')) length++;
		else						 break;
	}

	Int	 lengthafp = 0;

	if (string[length] == '.')
	{
		for (Int i = length + 1; i < string.Length(); i++)
		{
			if	((string[i] >= '0' && string[i] <= '9')) lengthafp++;
			else						 break;
		}
	}

	Float	 value = Number::FromIntString(string);

	for (Int i = 0; i < lengthafp; i++)
	{
		value += Float(string[i + length + 1] - '0') * Math::Pow(10, 0 - i - 1);
	}

	return value * sign;
}

S::Number S::Number::FromHexString(const String &iString)
{
	String	 string = iString.ToLower();

	if (string.StartsWith("0x")) string = string.Tail(string.Length() - 2);

	Int	 length = 0;

	for (Int i = 0; i < string.Length(); i++)
	{
		if	((string[i] >= '0' && string[i] <= '9') ||
			 (string[i] >= 'a' && string[i] <= 'f')) length++;
		else						 break;
	}

	Int64	 value = 0;

	for (Int i = 0; i < length; i++)
	{
		if	(string[i] >= '0' && string[i] <= '9') value += (Int64(string[i] - '0')	     << ((length - i - 1) * 4));
		else if (string[i] >= 'a' && string[i] <= 'f') value += (Int64(string[i] - 'a' + 10) << ((length - i - 1) * 4));
	}

	return value;
}

S::String S::Number::ToIntString() const
{
	if (intValue == 0) return "0";

	String	 string;
	Int	 length = (Int) Math::Log10(Math::Abs(intValue)) + 1;

	for (Int i = 0; i < length; i++)
	{
		string[string.Length()] = '0' + Int64(Math::Abs(intValue) / Math::Pow(10, length - i - 1)) % 10;
	}

	if (intValue < 0) string = String("-").Append(string);

	return string;
}

S::String S::Number::ToFloatString() const
{
	Int	 digits = 10;
	Int64	 fract	= (Int64) (Math::Fract(Math::Abs(floatValue)) * Math::Pow(10, digits));
	Int	 lead	= digits - Math::Floor((Math::Log10(fract) + 1.000000001));

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
		string.Append(".");

		for (Int i = 0; i < lead; i++) string.Append("0");

		string.Append(Number(fract).ToIntString());
	}

	if (floatValue < 0) string = String("-").Append(string);

	return string;
}

S::String S::Number::ToHexString(Int length) const
{
	String	 string;

	for (Int i = 0; i < 16; i++)
	{
		if	(((intValue >> (4 * (15 - i))) & 15) >= 10) string[string.Length()] = 'a' + ((intValue >> (4 * (15 - i))) & 15) - 10;
		else if (((intValue >> (4 * (15 - i))) & 15) >=  1) string[string.Length()] = '0' + ((intValue >> (4 * (15 - i))) & 15);
		else if (string.Length()		     >   0) string[string.Length()] = '0';
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
