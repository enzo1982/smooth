 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BINARY_
#define __OBJSMOOTH_BINARY_

#include <math.h>

#include <smooth/binary.h>

bool GetBit(int number, int bit)
{
	if (IsBitSet(number, (int) pow(2, bit)))	return true;
	else						return false;
}

int GetBits(int number, unsigned int startBit, unsigned int endBit)
{
	int	retVal = 0;

	if (startBit >= 64 || endBit >= 64) return -1;

	for (unsigned int i = startBit; i <= endBit; i++)
	{
		retVal += (int) pow(2, i - startBit) * ((number >> i) % 2);
	}

	return retVal;
}

bool IsBitSet(int number, int bit)
{
	if ((number & bit) > 0)	return true;
	else			return false;
}

#endif
