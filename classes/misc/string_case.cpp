 /* The smooth Class Library								
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>				
  *												
  * This library is free software; you can redistribute it and/or				
  * modify it under the terms of "The Artistic License, Version 2.0".			
  *												
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR				
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED				
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */			
												
#include <smooth/misc/string.h>								
												
S::String S::String::ToUpper() const							
{												
	String	 retVal = *this;								
												
	mutex.Lock();										
												
	for (Int i = 0; i < Length(); i++)							
	{
		if	((*this)[i] >= 0x61 && (*this)[i] <= 0x7a)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0xb5)							retVal[i] =		 0x39c;	// Single character
		else if	((*this)[i] >= 0xe0 && (*this)[i] <= 0xf6)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] >= 0xf8 && (*this)[i] <= 0xfe)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0xff)							retVal[i] =		 0x178;	// Single character
		else if	((*this)[i] >= 0x101 && (*this)[i] <= 0x12f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x131)							retVal[i] =		 0x49;	// Single character
		else if	((*this)[i] >= 0x133 && (*this)[i] <= 0x137 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x13a && (*this)[i] <= 0x148 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x14b && (*this)[i] <= 0x177 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x17a && (*this)[i] <= 0x17e && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x17f)							retVal[i] =		 0x53;	// Single character
		else if	((*this)[i] == 0x180)							retVal[i] =		 0x243;	// Single character
		else if	((*this)[i] >= 0x183 && (*this)[i] <= 0x185 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x188)							retVal[i] =		 0x187;	// Single character
		else if	((*this)[i] == 0x18c)							retVal[i] =		 0x18b;	// Single character
		else if	((*this)[i] == 0x192)							retVal[i] =		 0x191;	// Single character
		else if	((*this)[i] == 0x195)							retVal[i] =		 0x1f6;	// Single character
		else if	((*this)[i] == 0x199)							retVal[i] =		 0x198;	// Single character
		else if	((*this)[i] == 0x19a)							retVal[i] =		 0x23d;	// Single character
		else if	((*this)[i] == 0x19e)							retVal[i] =		 0x220;	// Single character
		else if	((*this)[i] >= 0x1a1 && (*this)[i] <= 0x1a5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1a8)							retVal[i] =		 0x1a7;	// Single character
		else if	((*this)[i] == 0x1ad)							retVal[i] =		 0x1ac;	// Single character
		else if	((*this)[i] == 0x1b0)							retVal[i] =		 0x1af;	// Single character
		else if	((*this)[i] >= 0x1b4 && (*this)[i] <= 0x1b6 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1b9)							retVal[i] =		 0x1b8;	// Single character
		else if	((*this)[i] == 0x1bd)							retVal[i] =		 0x1bc;	// Single character
		else if	((*this)[i] == 0x1bf)							retVal[i] =		 0x1f7;	// Single character
		else if	((*this)[i] == 0x1c5)							retVal[i] =		 0x1c4;	// Single character
		else if	((*this)[i] == 0x1c6)							retVal[i] =		 0x1c4;	// Single character
		else if	((*this)[i] == 0x1c8)							retVal[i] =		 0x1c7;	// Single character
		else if	((*this)[i] == 0x1c9)							retVal[i] =		 0x1c7;	// Single character
		else if	((*this)[i] == 0x1cb)							retVal[i] =		 0x1ca;	// Single character
		else if	((*this)[i] == 0x1cc)							retVal[i] =		 0x1ca;	// Single character
		else if	((*this)[i] >= 0x1ce && (*this)[i] <= 0x1dc && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1dd)							retVal[i] =		 0x18e;	// Single character
		else if	((*this)[i] >= 0x1df && (*this)[i] <= 0x1ef &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1f2)							retVal[i] =		 0x1f1;	// Single character
		else if	((*this)[i] == 0x1f3)							retVal[i] =		 0x1f1;	// Single character
		else if	((*this)[i] == 0x1f5)							retVal[i] =		 0x1f4;	// Single character
		else if	((*this)[i] >= 0x1f9 && (*this)[i] <= 0x21f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x223 && (*this)[i] <= 0x233 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x23c)							retVal[i] =		 0x23b;	// Single character
		else if	((*this)[i] == 0x242)							retVal[i] =		 0x241;	// Single character
		else if	((*this)[i] >= 0x247 && (*this)[i] <= 0x24f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x250)							retVal[i] =		 0x2c6f;	// Single character
		else if	((*this)[i] == 0x251)							retVal[i] =		 0x2c6d;	// Single character
		else if	((*this)[i] == 0x253)							retVal[i] =		 0x181;	// Single character
		else if	((*this)[i] == 0x254)							retVal[i] =		 0x186;	// Single character
		else if	((*this)[i] >= 0x256 && (*this)[i] <= 0x257)				retVal[i] = (*this)[i] - 0xcd;	// Alphabet
		else if	((*this)[i] == 0x259)							retVal[i] =		 0x18f;	// Single character
		else if	((*this)[i] == 0x25b)							retVal[i] =		 0x190;	// Single character
		else if	((*this)[i] == 0x260)							retVal[i] =		 0x193;	// Single character
		else if	((*this)[i] == 0x263)							retVal[i] =		 0x194;	// Single character
		else if	((*this)[i] == 0x268)							retVal[i] =		 0x197;	// Single character
		else if	((*this)[i] == 0x269)							retVal[i] =		 0x196;	// Single character
		else if	((*this)[i] == 0x26b)							retVal[i] =		 0x2c62;	// Single character
		else if	((*this)[i] == 0x26f)							retVal[i] =		 0x19c;	// Single character
		else if	((*this)[i] == 0x271)							retVal[i] =		 0x2c6e;	// Single character
		else if	((*this)[i] == 0x272)							retVal[i] =		 0x19d;	// Single character
		else if	((*this)[i] == 0x275)							retVal[i] =		 0x19f;	// Single character
		else if	((*this)[i] == 0x27d)							retVal[i] =		 0x2c64;	// Single character
		else if	((*this)[i] == 0x280)							retVal[i] =		 0x1a6;	// Single character
		else if	((*this)[i] == 0x283)							retVal[i] =		 0x1a9;	// Single character
		else if	((*this)[i] == 0x288)							retVal[i] =		 0x1ae;	// Single character
		else if	((*this)[i] == 0x289)							retVal[i] =		 0x244;	// Single character
		else if	((*this)[i] >= 0x28a && (*this)[i] <= 0x28b)				retVal[i] = (*this)[i] - 0xd9;	// Alphabet
		else if	((*this)[i] == 0x28c)							retVal[i] =		 0x245;	// Single character
		else if	((*this)[i] == 0x292)							retVal[i] =		 0x1b7;	// Single character
		else if	((*this)[i] == 0x345)							retVal[i] =		 0x399;	// Single character
		else if	((*this)[i] >= 0x371 && (*this)[i] <= 0x373 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x377)							retVal[i] =		 0x376;	// Single character
		else if	((*this)[i] >= 0x37b && (*this)[i] <= 0x37d)				retVal[i] = (*this)[i] + 0x82;	// Alphabet
		else if	((*this)[i] == 0x3ac)							retVal[i] =		 0x386;	// Single character
		else if	((*this)[i] >= 0x3ad && (*this)[i] <= 0x3af)				retVal[i] = (*this)[i] - 0x25;	// Alphabet
		else if	((*this)[i] >= 0x3b1 && (*this)[i] <= 0x3c1)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0x3c2)							retVal[i] =		 0x3a3;	// Single character
		else if	((*this)[i] >= 0x3c3 && (*this)[i] <= 0x3cb)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0x3cc)							retVal[i] =		 0x38c;	// Single character
		else if	((*this)[i] >= 0x3cd && (*this)[i] <= 0x3ce)				retVal[i] = (*this)[i] - 0x3f;	// Alphabet
		else if	((*this)[i] == 0x3d0)							retVal[i] =		 0x392;	// Single character
		else if	((*this)[i] == 0x3d1)							retVal[i] =		 0x398;	// Single character
		else if	((*this)[i] == 0x3d5)							retVal[i] =		 0x3a6;	// Single character
		else if	((*this)[i] == 0x3d6)							retVal[i] =		 0x3a0;	// Single character
		else if	((*this)[i] == 0x3d7)							retVal[i] =		 0x3cf;	// Single character
		else if	((*this)[i] >= 0x3d9 && (*this)[i] <= 0x3ef &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x3f0)							retVal[i] =		 0x39a;	// Single character
		else if	((*this)[i] == 0x3f1)							retVal[i] =		 0x3a1;	// Single character
		else if	((*this)[i] == 0x3f2)							retVal[i] =		 0x3f9;	// Single character
		else if	((*this)[i] == 0x3f5)							retVal[i] =		 0x395;	// Single character
		else if	((*this)[i] == 0x3f8)							retVal[i] =		 0x3f7;	// Single character
		else if	((*this)[i] == 0x3fb)							retVal[i] =		 0x3fa;	// Single character
		else if	((*this)[i] >= 0x430 && (*this)[i] <= 0x44f)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] >= 0x450 && (*this)[i] <= 0x45f)				retVal[i] = (*this)[i] - 0x50;	// Alphabet
		else if	((*this)[i] >= 0x461 && (*this)[i] <= 0x481 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x48b && (*this)[i] <= 0x4bf &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x4c2 && (*this)[i] <= 0x4ce && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x4cf)							retVal[i] =		 0x4c0;	// Single character
		else if	((*this)[i] >= 0x4d1 && (*this)[i] <= 0x523 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x561 && (*this)[i] <= 0x586)				retVal[i] = (*this)[i] - 0x30;	// Alphabet
		else if	((*this)[i] == 0x1d79)							retVal[i] =		 0xa77d;	// Single character
		else if	((*this)[i] == 0x1d7d)							retVal[i] =		 0x2c63;	// Single character
		else if	((*this)[i] >= 0x1e01 && (*this)[i] <= 0x1e95 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1e9b)							retVal[i] =		 0x1e60;	// Single character
		else if	((*this)[i] >= 0x1ea1 && (*this)[i] <= 0x1eff &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x1f00 && (*this)[i] <= 0x1f07)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f10 && (*this)[i] <= 0x1f15)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f20 && (*this)[i] <= 0x1f27)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f30 && (*this)[i] <= 0x1f37)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f40 && (*this)[i] <= 0x1f45)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1f51)							retVal[i] =		 0x1f59;	// Single character
		else if	((*this)[i] == 0x1f53)							retVal[i] =		 0x1f5b;	// Single character
		else if	((*this)[i] == 0x1f55)							retVal[i] =		 0x1f5d;	// Single character
		else if	((*this)[i] == 0x1f57)							retVal[i] =		 0x1f5f;	// Single character

		/* Work around stupid MSVC's limit on alternatives...
		 * ----
		 * This is not generated automatically, but
		 * needs to be inserted manually here.
		 */
		if	((*this)[i] >= 0x1f60 && (*this)[i] <= 0x1f67)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f70 && (*this)[i] <= 0x1f71)				retVal[i] = (*this)[i] + 0x4a;	// Alphabet
		else if	((*this)[i] >= 0x1f72 && (*this)[i] <= 0x1f75)				retVal[i] = (*this)[i] + 0x56;	// Alphabet
		else if	((*this)[i] >= 0x1f76 && (*this)[i] <= 0x1f77)				retVal[i] = (*this)[i] + 0x64;	// Alphabet
		else if	((*this)[i] >= 0x1f78 && (*this)[i] <= 0x1f79)				retVal[i] = (*this)[i] + 0x80;	// Alphabet
		else if	((*this)[i] >= 0x1f7a && (*this)[i] <= 0x1f7b)				retVal[i] = (*this)[i] + 0x70;	// Alphabet
		else if	((*this)[i] >= 0x1f7c && (*this)[i] <= 0x1f7d)				retVal[i] = (*this)[i] + 0x7e;	// Alphabet
		else if	((*this)[i] >= 0x1f80 && (*this)[i] <= 0x1f87)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f90 && (*this)[i] <= 0x1f97)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fa0 && (*this)[i] <= 0x1fa7)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fb0 && (*this)[i] <= 0x1fb1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1fb3)							retVal[i] =		 0x1fbc;	// Single character
		else if	((*this)[i] == 0x1fbe)							retVal[i] =		 0x399;	// Single character
		else if	((*this)[i] == 0x1fc3)							retVal[i] =		 0x1fcc;	// Single character
		else if	((*this)[i] >= 0x1fd0 && (*this)[i] <= 0x1fd1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fe0 && (*this)[i] <= 0x1fe1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1fe5)							retVal[i] =		 0x1fec;	// Single character
		else if	((*this)[i] == 0x1ff3)							retVal[i] =		 0x1ffc;	// Single character
		else if	((*this)[i] == 0x214e)							retVal[i] =		 0x2132;	// Single character
		else if	((*this)[i] >= 0x2170 && (*this)[i] <= 0x217f)				retVal[i] = (*this)[i] - 0x10;	// Alphabet
		else if	((*this)[i] == 0x2184)							retVal[i] =		 0x2183;	// Single character
		else if	((*this)[i] >= 0x24d0 && (*this)[i] <= 0x24e9)				retVal[i] = (*this)[i] - 0x1a;	// Alphabet
		else if	((*this)[i] >= 0x2c30 && (*this)[i] <= 0x2c5e)				retVal[i] = (*this)[i] - 0x30;	// Alphabet
		else if	((*this)[i] == 0x2c61)							retVal[i] =		 0x2c60;	// Single character
		else if	((*this)[i] == 0x2c65)							retVal[i] =		 0x23a;	// Single character
		else if	((*this)[i] == 0x2c66)							retVal[i] =		 0x23e;	// Single character
		else if	((*this)[i] >= 0x2c68 && (*this)[i] <= 0x2c6c && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x2c73)							retVal[i] =		 0x2c72;	// Single character
		else if	((*this)[i] == 0x2c76)							retVal[i] =		 0x2c75;	// Single character
		else if	((*this)[i] >= 0x2c81 && (*this)[i] <= 0x2ce3 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x2d00 && (*this)[i] <= 0x2d25)				retVal[i] = (*this)[i] - 0x1c60;	// Alphabet
		else if	((*this)[i] >= 0xa641 && (*this)[i] <= 0xa65f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa663 && (*this)[i] <= 0xa66d &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa681 && (*this)[i] <= 0xa697 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa723 && (*this)[i] <= 0xa72f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa733 && (*this)[i] <= 0xa76f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa77a && (*this)[i] <= 0xa77c && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa77f && (*this)[i] <= 0xa787 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0xa78c)							retVal[i] =		 0xa78b;	// Single character
		else if	((*this)[i] >= 0xff41 && (*this)[i] <= 0xff5a)				retVal[i] = (*this)[i] - 0x20;	// Alphabet															
	}							
								
	mutex.Release();					
								
	return retVal;						
}								
															
S::String S::String::ToLower() const			
{								
	String	 retVal = *this;				
								
	mutex.Lock();						
								
	for (Int i = 0; i < Length(); i++)			
	{
		if	((*this)[i] >= 0x41 && (*this)[i] <= 0x5a)				retVal[i] = (*this)[i] + 0x20;	// Alphabet
		else if	((*this)[i] >= 0xc0 && (*this)[i] <= 0xd6)				retVal[i] = (*this)[i] + 0x20;	// Alphabet
		else if	((*this)[i] >= 0xd8 && (*this)[i] <= 0xde)				retVal[i] = (*this)[i] + 0x20;	// Alphabet
		else if	((*this)[i] >= 0x100 && (*this)[i] <= 0x12e && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x130)							retVal[i] =		 0x69;	// Single character
		else if	((*this)[i] >= 0x132 && (*this)[i] <= 0x136 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x139 && (*this)[i] <= 0x147 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x14a && (*this)[i] <= 0x176 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x178)							retVal[i] =		 0xff;	// Single character
		else if	((*this)[i] >= 0x179 && (*this)[i] <= 0x17d &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x181)							retVal[i] =		 0x253;	// Single character
		else if	((*this)[i] >= 0x182 && (*this)[i] <= 0x184 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x186)							retVal[i] =		 0x254;	// Single character
		else if	((*this)[i] == 0x187)							retVal[i] =		 0x188;	// Single character
		else if	((*this)[i] >= 0x189 && (*this)[i] <= 0x18a)				retVal[i] = (*this)[i] + 0xcd;	// Alphabet
		else if	((*this)[i] == 0x18b)							retVal[i] =		 0x18c;	// Single character
		else if	((*this)[i] == 0x18e)							retVal[i] =		 0x1dd;	// Single character
		else if	((*this)[i] == 0x18f)							retVal[i] =		 0x259;	// Single character
		else if	((*this)[i] == 0x190)							retVal[i] =		 0x25b;	// Single character
		else if	((*this)[i] == 0x191)							retVal[i] =		 0x192;	// Single character
		else if	((*this)[i] == 0x193)							retVal[i] =		 0x260;	// Single character
		else if	((*this)[i] == 0x194)							retVal[i] =		 0x263;	// Single character
		else if	((*this)[i] == 0x196)							retVal[i] =		 0x269;	// Single character
		else if	((*this)[i] == 0x197)							retVal[i] =		 0x268;	// Single character
		else if	((*this)[i] == 0x198)							retVal[i] =		 0x199;	// Single character
		else if	((*this)[i] == 0x19c)							retVal[i] =		 0x26f;	// Single character
		else if	((*this)[i] == 0x19d)							retVal[i] =		 0x272;	// Single character
		else if	((*this)[i] == 0x19f)							retVal[i] =		 0x275;	// Single character
		else if	((*this)[i] >= 0x1a0 && (*this)[i] <= 0x1a4 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x1a6)							retVal[i] =		 0x280;	// Single character
		else if	((*this)[i] == 0x1a7)							retVal[i] =		 0x1a8;	// Single character
		else if	((*this)[i] == 0x1a9)							retVal[i] =		 0x283;	// Single character
		else if	((*this)[i] == 0x1ac)							retVal[i] =		 0x1ad;	// Single character
		else if	((*this)[i] == 0x1ae)							retVal[i] =		 0x288;	// Single character
		else if	((*this)[i] == 0x1af)							retVal[i] =		 0x1b0;	// Single character
		else if	((*this)[i] >= 0x1b1 && (*this)[i] <= 0x1b2)				retVal[i] = (*this)[i] + 0xd9;	// Alphabet
		else if	((*this)[i] >= 0x1b3 && (*this)[i] <= 0x1b5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x1b7)							retVal[i] =		 0x292;	// Single character
		else if	((*this)[i] == 0x1b8)							retVal[i] =		 0x1b9;	// Single character
		else if	((*this)[i] == 0x1bc)							retVal[i] =		 0x1bd;	// Single character
		else if	((*this)[i] == 0x1c4)							retVal[i] =		 0x1c6;	// Single character
		else if	((*this)[i] == 0x1c5)							retVal[i] =		 0x1c6;	// Single character
		else if	((*this)[i] == 0x1c7)							retVal[i] =		 0x1c9;	// Single character
		else if	((*this)[i] == 0x1c8)							retVal[i] =		 0x1c9;	// Single character
		else if	((*this)[i] == 0x1ca)							retVal[i] =		 0x1cc;	// Single character
		else if	((*this)[i] >= 0x1cb && (*this)[i] <= 0x1db &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x1de && (*this)[i] <= 0x1ee && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x1f1)							retVal[i] =		 0x1f3;	// Single character
		else if	((*this)[i] >= 0x1f2 && (*this)[i] <= 0x1f4 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x1f6)							retVal[i] =		 0x195;	// Single character
		else if	((*this)[i] == 0x1f7)							retVal[i] =		 0x1bf;	// Single character
		else if	((*this)[i] >= 0x1f8 && (*this)[i] <= 0x21e && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x220)							retVal[i] =		 0x19e;	// Single character
		else if	((*this)[i] >= 0x222 && (*this)[i] <= 0x232 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x23a)							retVal[i] =		 0x2c65;	// Single character
		else if	((*this)[i] == 0x23b)							retVal[i] =		 0x23c;	// Single character
		else if	((*this)[i] == 0x23d)							retVal[i] =		 0x19a;	// Single character
		else if	((*this)[i] == 0x23e)							retVal[i] =		 0x2c66;	// Single character
		else if	((*this)[i] == 0x241)							retVal[i] =		 0x242;	// Single character
		else if	((*this)[i] == 0x243)							retVal[i] =		 0x180;	// Single character
		else if	((*this)[i] == 0x244)							retVal[i] =		 0x289;	// Single character
		else if	((*this)[i] == 0x245)							retVal[i] =		 0x28c;	// Single character
		else if	((*this)[i] >= 0x246 && (*this)[i] <= 0x24e && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x370 && (*this)[i] <= 0x372 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x376)							retVal[i] =		 0x377;	// Single character
		else if	((*this)[i] == 0x386)							retVal[i] =		 0x3ac;	// Single character
		else if	((*this)[i] >= 0x388 && (*this)[i] <= 0x38a)				retVal[i] = (*this)[i] + 0x25;	// Alphabet
		else if	((*this)[i] == 0x38c)							retVal[i] =		 0x3cc;	// Single character
		else if	((*this)[i] >= 0x38e && (*this)[i] <= 0x38f)				retVal[i] = (*this)[i] + 0x3f;	// Alphabet
		else if	((*this)[i] >= 0x391 && (*this)[i] <= 0x3aa)				retVal[i] = (*this)[i] + 0x20;	// Alphabet
		else if	((*this)[i] == 0x3cf)							retVal[i] =		 0x3d7;	// Single character
		else if	((*this)[i] >= 0x3d8 && (*this)[i] <= 0x3ee && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x3f4)							retVal[i] =		 0x3b8;	// Single character
		else if	((*this)[i] == 0x3f7)							retVal[i] =		 0x3f8;	// Single character
		else if	((*this)[i] == 0x3f9)							retVal[i] =		 0x3f2;	// Single character
		else if	((*this)[i] == 0x3fa)							retVal[i] =		 0x3fb;	// Single character
		else if	((*this)[i] >= 0x3fd && (*this)[i] <= 0x3ff)				retVal[i] = (*this)[i] - 0x82;	// Alphabet
		else if	((*this)[i] >= 0x400 && (*this)[i] <= 0x40f)				retVal[i] = (*this)[i] + 0x50;	// Alphabet
		else if	((*this)[i] >= 0x410 && (*this)[i] <= 0x42f)				retVal[i] = (*this)[i] + 0x20;	// Alphabet
		else if	((*this)[i] >= 0x460 && (*this)[i] <= 0x480 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x48a && (*this)[i] <= 0x4be && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x4c0)							retVal[i] =		 0x4cf;	// Single character
		else if	((*this)[i] >= 0x4c1 && (*this)[i] <= 0x4cd &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x4d0 && (*this)[i] <= 0x522 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x531 && (*this)[i] <= 0x556)				retVal[i] = (*this)[i] + 0x30;	// Alphabet
		else if	((*this)[i] >= 0x10a0 && (*this)[i] <= 0x10c5)				retVal[i] = (*this)[i] + 0x1c60;	// Alphabet
		else if	((*this)[i] >= 0x1e00 && (*this)[i] <= 0x1e94 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x1e9e)							retVal[i] =		 0xdf;	// Single character
		else if	((*this)[i] >= 0x1ea0 && (*this)[i] <= 0x1efe && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0x1f08 && (*this)[i] <= 0x1f0f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f18 && (*this)[i] <= 0x1f1d)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f28 && (*this)[i] <= 0x1f2f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f38 && (*this)[i] <= 0x1f3f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f48 && (*this)[i] <= 0x1f4d)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f59 && (*this)[i] <= 0x1f5c)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f68 && (*this)[i] <= 0x1f6f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f88 && (*this)[i] <= 0x1f8f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f98 && (*this)[i] <= 0x1f9f)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fa8 && (*this)[i] <= 0x1faf)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fb8 && (*this)[i] <= 0x1fb9)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fba && (*this)[i] <= 0x1fbb)				retVal[i] = (*this)[i] - 0x4a;	// Alphabet
		else if	((*this)[i] == 0x1fbc)							retVal[i] =		 0x1fb3;	// Single character
		else if	((*this)[i] >= 0x1fc8 && (*this)[i] <= 0x1fcb)				retVal[i] = (*this)[i] - 0x56;	// Alphabet
		else if	((*this)[i] == 0x1fcc)							retVal[i] =		 0x1fc3;	// Single character
		else if	((*this)[i] >= 0x1fd8 && (*this)[i] <= 0x1fd9)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fda && (*this)[i] <= 0x1fdb)				retVal[i] = (*this)[i] - 0x64;	// Alphabet
		else if	((*this)[i] >= 0x1fe8 && (*this)[i] <= 0x1fe9)				retVal[i] = (*this)[i] - 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fea && (*this)[i] <= 0x1feb)				retVal[i] = (*this)[i] - 0x70;	// Alphabet
		else if	((*this)[i] == 0x1fec)							retVal[i] =		 0x1fe5;	// Single character
		else if	((*this)[i] >= 0x1ff8 && (*this)[i] <= 0x1ff9)				retVal[i] = (*this)[i] - 0x80;	// Alphabet
		else if	((*this)[i] >= 0x1ffa && (*this)[i] <= 0x1ffb)				retVal[i] = (*this)[i] - 0x7e;	// Alphabet
		else if	((*this)[i] == 0x1ffc)							retVal[i] =		 0x1ff3;	// Single character
		else if	((*this)[i] == 0x2126)							retVal[i] =		 0x3c9;	// Single character
		else if	((*this)[i] == 0x212a)							retVal[i] =		 0x6b;	// Single character
		else if	((*this)[i] == 0x212b)							retVal[i] =		 0xe5;	// Single character
		else if	((*this)[i] == 0x2132)							retVal[i] =		 0x214e;	// Single character
		else if	((*this)[i] >= 0x2160 && (*this)[i] <= 0x216f)				retVal[i] = (*this)[i] + 0x10;	// Alphabet
		else if	((*this)[i] == 0x2183)							retVal[i] =		 0x2184;	// Single character
		else if	((*this)[i] >= 0x24b6 && (*this)[i] <= 0x24cf)				retVal[i] = (*this)[i] + 0x1a;	// Alphabet
		else if	((*this)[i] >= 0x2c00 && (*this)[i] <= 0x2c2e)				retVal[i] = (*this)[i] + 0x30;	// Alphabet

		/* Work around stupid MSVC's limit on alternatives...
		 * ----
		 * This is not generated automatically, but
		 * needs to be inserted manually here.
		 */
		if	((*this)[i] == 0x2c60)							retVal[i] =		 0x2c61;	// Single character
		else if	((*this)[i] == 0x2c62)							retVal[i] =		 0x26b;	// Single character
		else if	((*this)[i] == 0x2c63)							retVal[i] =		 0x1d7d;	// Single character
		else if	((*this)[i] == 0x2c64)							retVal[i] =		 0x27d;	// Single character
		else if	((*this)[i] >= 0x2c67 && (*this)[i] <= 0x2c6b &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0x2c6d)							retVal[i] =		 0x251;	// Single character
		else if	((*this)[i] == 0x2c6e)							retVal[i] =		 0x271;	// Single character
		else if	((*this)[i] == 0x2c6f)							retVal[i] =		 0x250;	// Single character
		else if	((*this)[i] == 0x2c72)							retVal[i] =		 0x2c73;	// Single character
		else if	((*this)[i] == 0x2c75)							retVal[i] =		 0x2c76;	// Single character
		else if	((*this)[i] >= 0x2c80 && (*this)[i] <= 0x2ce2 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa640 && (*this)[i] <= 0xa660 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa664 && (*this)[i] <= 0xa66c && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa680 && (*this)[i] <= 0xa696 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa722 && (*this)[i] <= 0xa72e && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa732 && (*this)[i] <= 0xa76e && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] >= 0xa779 && (*this)[i] <= 0xa77b &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0xa77d)							retVal[i] =		 0x1d79;	// Single character
		else if	((*this)[i] >= 0xa77e && (*this)[i] <= 0xa786 && !((*this)[i] & 1))	retVal[i] = (*this)[i] + 0x1;	// Special letters
		else if	((*this)[i] == 0xa78b)							retVal[i] =		 0xa78c;	// Single character
		else if	((*this)[i] >= 0xff21 && (*this)[i] <= 0xff3a)				retVal[i] = (*this)[i] + 0x20;	// Alphabet															
	}							
								
	mutex.Release();					
								
	return retVal;						
}								
															
S::String S::String::ToTitle() const			
{								
	String	 retVal = *this;				
								
	mutex.Lock();						
								
	for (Int i = 0; i < Length(); i++)			
	{
		if	((*this)[i] >= 0x61 && (*this)[i] <= 0x7a)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0xb5)							retVal[i] =		 0x39c;	// Single character
		else if	((*this)[i] >= 0xe0 && (*this)[i] <= 0xf6)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] >= 0xf8 && (*this)[i] <= 0xfe)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0xff)							retVal[i] =		 0x178;	// Single character
		else if	((*this)[i] >= 0x101 && (*this)[i] <= 0x12f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x131)							retVal[i] =		 0x49;	// Single character
		else if	((*this)[i] >= 0x133 && (*this)[i] <= 0x137 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x13a && (*this)[i] <= 0x148 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x14b && (*this)[i] <= 0x177 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x17a && (*this)[i] <= 0x17e && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x17f)							retVal[i] =		 0x53;	// Single character
		else if	((*this)[i] == 0x180)							retVal[i] =		 0x243;	// Single character
		else if	((*this)[i] >= 0x183 && (*this)[i] <= 0x185 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x188)							retVal[i] =		 0x187;	// Single character
		else if	((*this)[i] == 0x18c)							retVal[i] =		 0x18b;	// Single character
		else if	((*this)[i] == 0x192)							retVal[i] =		 0x191;	// Single character
		else if	((*this)[i] == 0x195)							retVal[i] =		 0x1f6;	// Single character
		else if	((*this)[i] == 0x199)							retVal[i] =		 0x198;	// Single character
		else if	((*this)[i] == 0x19a)							retVal[i] =		 0x23d;	// Single character
		else if	((*this)[i] == 0x19e)							retVal[i] =		 0x220;	// Single character
		else if	((*this)[i] >= 0x1a1 && (*this)[i] <= 0x1a5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1a8)							retVal[i] =		 0x1a7;	// Single character
		else if	((*this)[i] == 0x1ad)							retVal[i] =		 0x1ac;	// Single character
		else if	((*this)[i] == 0x1b0)							retVal[i] =		 0x1af;	// Single character
		else if	((*this)[i] >= 0x1b4 && (*this)[i] <= 0x1b6 && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1b9)							retVal[i] =		 0x1b8;	// Single character
		else if	((*this)[i] == 0x1bd)							retVal[i] =		 0x1bc;	// Single character
		else if	((*this)[i] == 0x1bf)							retVal[i] =		 0x1f7;	// Single character
		else if	((*this)[i] == 0x1c4)							retVal[i] =		 0x1c5;	// Single character
		else if	((*this)[i] == 0x1c5)							retVal[i] =		 0x1c5;	// Single character
		else if	((*this)[i] == 0x1c6)							retVal[i] =		 0x1c5;	// Single character
		else if	((*this)[i] == 0x1c7)							retVal[i] =		 0x1c8;	// Single character
		else if	((*this)[i] == 0x1c8)							retVal[i] =		 0x1c8;	// Single character
		else if	((*this)[i] == 0x1c9)							retVal[i] =		 0x1c8;	// Single character
		else if	((*this)[i] == 0x1ca)							retVal[i] =		 0x1cb;	// Single character
		else if	((*this)[i] == 0x1cb)							retVal[i] =		 0x1cb;	// Single character
		else if	((*this)[i] >= 0x1cc && (*this)[i] <= 0x1dc && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1dd)							retVal[i] =		 0x18e;	// Single character
		else if	((*this)[i] >= 0x1df && (*this)[i] <= 0x1ef &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1f1)							retVal[i] =		 0x1f2;	// Single character
		else if	((*this)[i] == 0x1f2)							retVal[i] =		 0x1f2;	// Single character
		else if	((*this)[i] >= 0x1f3 && (*this)[i] <= 0x1f5 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x1f9 && (*this)[i] <= 0x21f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x223 && (*this)[i] <= 0x233 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x23c)							retVal[i] =		 0x23b;	// Single character
		else if	((*this)[i] == 0x242)							retVal[i] =		 0x241;	// Single character
		else if	((*this)[i] >= 0x247 && (*this)[i] <= 0x24f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x250)							retVal[i] =		 0x2c6f;	// Single character
		else if	((*this)[i] == 0x251)							retVal[i] =		 0x2c6d;	// Single character
		else if	((*this)[i] == 0x253)							retVal[i] =		 0x181;	// Single character
		else if	((*this)[i] == 0x254)							retVal[i] =		 0x186;	// Single character
		else if	((*this)[i] >= 0x256 && (*this)[i] <= 0x257)				retVal[i] = (*this)[i] - 0xcd;	// Alphabet
		else if	((*this)[i] == 0x259)							retVal[i] =		 0x18f;	// Single character
		else if	((*this)[i] == 0x25b)							retVal[i] =		 0x190;	// Single character
		else if	((*this)[i] == 0x260)							retVal[i] =		 0x193;	// Single character
		else if	((*this)[i] == 0x263)							retVal[i] =		 0x194;	// Single character
		else if	((*this)[i] == 0x268)							retVal[i] =		 0x197;	// Single character
		else if	((*this)[i] == 0x269)							retVal[i] =		 0x196;	// Single character
		else if	((*this)[i] == 0x26b)							retVal[i] =		 0x2c62;	// Single character
		else if	((*this)[i] == 0x26f)							retVal[i] =		 0x19c;	// Single character
		else if	((*this)[i] == 0x271)							retVal[i] =		 0x2c6e;	// Single character
		else if	((*this)[i] == 0x272)							retVal[i] =		 0x19d;	// Single character
		else if	((*this)[i] == 0x275)							retVal[i] =		 0x19f;	// Single character
		else if	((*this)[i] == 0x27d)							retVal[i] =		 0x2c64;	// Single character
		else if	((*this)[i] == 0x280)							retVal[i] =		 0x1a6;	// Single character
		else if	((*this)[i] == 0x283)							retVal[i] =		 0x1a9;	// Single character
		else if	((*this)[i] == 0x288)							retVal[i] =		 0x1ae;	// Single character
		else if	((*this)[i] == 0x289)							retVal[i] =		 0x244;	// Single character
		else if	((*this)[i] >= 0x28a && (*this)[i] <= 0x28b)				retVal[i] = (*this)[i] - 0xd9;	// Alphabet
		else if	((*this)[i] == 0x28c)							retVal[i] =		 0x245;	// Single character
		else if	((*this)[i] == 0x292)							retVal[i] =		 0x1b7;	// Single character
		else if	((*this)[i] == 0x345)							retVal[i] =		 0x399;	// Single character
		else if	((*this)[i] >= 0x371 && (*this)[i] <= 0x373 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x377)							retVal[i] =		 0x376;	// Single character
		else if	((*this)[i] >= 0x37b && (*this)[i] <= 0x37d)				retVal[i] = (*this)[i] + 0x82;	// Alphabet
		else if	((*this)[i] == 0x3ac)							retVal[i] =		 0x386;	// Single character
		else if	((*this)[i] >= 0x3ad && (*this)[i] <= 0x3af)				retVal[i] = (*this)[i] - 0x25;	// Alphabet
		else if	((*this)[i] >= 0x3b1 && (*this)[i] <= 0x3c1)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0x3c2)							retVal[i] =		 0x3a3;	// Single character
		else if	((*this)[i] >= 0x3c3 && (*this)[i] <= 0x3cb)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] == 0x3cc)							retVal[i] =		 0x38c;	// Single character
		else if	((*this)[i] >= 0x3cd && (*this)[i] <= 0x3ce)				retVal[i] = (*this)[i] - 0x3f;	// Alphabet
		else if	((*this)[i] == 0x3d0)							retVal[i] =		 0x392;	// Single character
		else if	((*this)[i] == 0x3d1)							retVal[i] =		 0x398;	// Single character
		else if	((*this)[i] == 0x3d5)							retVal[i] =		 0x3a6;	// Single character
		else if	((*this)[i] == 0x3d6)							retVal[i] =		 0x3a0;	// Single character
		else if	((*this)[i] == 0x3d7)							retVal[i] =		 0x3cf;	// Single character
		else if	((*this)[i] >= 0x3d9 && (*this)[i] <= 0x3ef &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x3f0)							retVal[i] =		 0x39a;	// Single character
		else if	((*this)[i] == 0x3f1)							retVal[i] =		 0x3a1;	// Single character
		else if	((*this)[i] == 0x3f2)							retVal[i] =		 0x3f9;	// Single character
		else if	((*this)[i] == 0x3f5)							retVal[i] =		 0x395;	// Single character
		else if	((*this)[i] == 0x3f8)							retVal[i] =		 0x3f7;	// Single character
		else if	((*this)[i] == 0x3fb)							retVal[i] =		 0x3fa;	// Single character
		else if	((*this)[i] >= 0x430 && (*this)[i] <= 0x44f)				retVal[i] = (*this)[i] - 0x20;	// Alphabet
		else if	((*this)[i] >= 0x450 && (*this)[i] <= 0x45f)				retVal[i] = (*this)[i] - 0x50;	// Alphabet
		else if	((*this)[i] >= 0x461 && (*this)[i] <= 0x481 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x48b && (*this)[i] <= 0x4bf &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x4c2 && (*this)[i] <= 0x4ce && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x4cf)							retVal[i] =		 0x4c0;	// Single character
		else if	((*this)[i] >= 0x4d1 && (*this)[i] <= 0x523 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x561 && (*this)[i] <= 0x586)				retVal[i] = (*this)[i] - 0x30;	// Alphabet
		else if	((*this)[i] == 0x1d79)							retVal[i] =		 0xa77d;	// Single character
		else if	((*this)[i] == 0x1d7d)							retVal[i] =		 0x2c63;	// Single character
		else if	((*this)[i] >= 0x1e01 && (*this)[i] <= 0x1e95 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x1e9b)							retVal[i] =		 0x1e60;	// Single character
		else if	((*this)[i] >= 0x1ea1 && (*this)[i] <= 0x1eff &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x1f00 && (*this)[i] <= 0x1f07)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f10 && (*this)[i] <= 0x1f15)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f20 && (*this)[i] <= 0x1f27)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f30 && (*this)[i] <= 0x1f37)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f40 && (*this)[i] <= 0x1f45)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1f51)							retVal[i] =		 0x1f59;	// Single character
		else if	((*this)[i] == 0x1f53)							retVal[i] =		 0x1f5b;	// Single character
		else if	((*this)[i] == 0x1f55)							retVal[i] =		 0x1f5d;	// Single character
		else if	((*this)[i] == 0x1f57)							retVal[i] =		 0x1f5f;	// Single character

		/* Work around stupid MSVC's limit on alternatives...
		 * ----
		 * This is not generated automatically, but
		 * needs to be inserted manually here.
		 */
		if	((*this)[i] >= 0x1f60 && (*this)[i] <= 0x1f67)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f70 && (*this)[i] <= 0x1f71)				retVal[i] = (*this)[i] + 0x4a;	// Alphabet
		else if	((*this)[i] >= 0x1f72 && (*this)[i] <= 0x1f75)				retVal[i] = (*this)[i] + 0x56;	// Alphabet
		else if	((*this)[i] >= 0x1f76 && (*this)[i] <= 0x1f77)				retVal[i] = (*this)[i] + 0x64;	// Alphabet
		else if	((*this)[i] >= 0x1f78 && (*this)[i] <= 0x1f79)				retVal[i] = (*this)[i] + 0x80;	// Alphabet
		else if	((*this)[i] >= 0x1f7a && (*this)[i] <= 0x1f7b)				retVal[i] = (*this)[i] + 0x70;	// Alphabet
		else if	((*this)[i] >= 0x1f7c && (*this)[i] <= 0x1f7d)				retVal[i] = (*this)[i] + 0x7e;	// Alphabet
		else if	((*this)[i] >= 0x1f80 && (*this)[i] <= 0x1f87)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1f90 && (*this)[i] <= 0x1f97)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fa0 && (*this)[i] <= 0x1fa7)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fb0 && (*this)[i] <= 0x1fb1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1fb3)							retVal[i] =		 0x1fbc;	// Single character
		else if	((*this)[i] == 0x1fbe)							retVal[i] =		 0x399;	// Single character
		else if	((*this)[i] == 0x1fc3)							retVal[i] =		 0x1fcc;	// Single character
		else if	((*this)[i] >= 0x1fd0 && (*this)[i] <= 0x1fd1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] >= 0x1fe0 && (*this)[i] <= 0x1fe1)				retVal[i] = (*this)[i] + 0x8;	// Alphabet
		else if	((*this)[i] == 0x1fe5)							retVal[i] =		 0x1fec;	// Single character
		else if	((*this)[i] == 0x1ff3)							retVal[i] =		 0x1ffc;	// Single character
		else if	((*this)[i] == 0x214e)							retVal[i] =		 0x2132;	// Single character
		else if	((*this)[i] >= 0x2170 && (*this)[i] <= 0x217f)				retVal[i] = (*this)[i] - 0x10;	// Alphabet
		else if	((*this)[i] == 0x2184)							retVal[i] =		 0x2183;	// Single character
		else if	((*this)[i] >= 0x24d0 && (*this)[i] <= 0x24e9)				retVal[i] = (*this)[i] - 0x1a;	// Alphabet
		else if	((*this)[i] >= 0x2c30 && (*this)[i] <= 0x2c5e)				retVal[i] = (*this)[i] - 0x30;	// Alphabet
		else if	((*this)[i] == 0x2c61)							retVal[i] =		 0x2c60;	// Single character
		else if	((*this)[i] == 0x2c65)							retVal[i] =		 0x23a;	// Single character
		else if	((*this)[i] == 0x2c66)							retVal[i] =		 0x23e;	// Single character
		else if	((*this)[i] >= 0x2c68 && (*this)[i] <= 0x2c6c && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0x2c73)							retVal[i] =		 0x2c72;	// Single character
		else if	((*this)[i] == 0x2c76)							retVal[i] =		 0x2c75;	// Single character
		else if	((*this)[i] >= 0x2c81 && (*this)[i] <= 0x2ce3 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0x2d00 && (*this)[i] <= 0x2d25)				retVal[i] = (*this)[i] - 0x1c60;	// Alphabet
		else if	((*this)[i] >= 0xa641 && (*this)[i] <= 0xa65f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa663 && (*this)[i] <= 0xa66d &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa681 && (*this)[i] <= 0xa697 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa723 && (*this)[i] <= 0xa72f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa733 && (*this)[i] <= 0xa76f &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa77a && (*this)[i] <= 0xa77c && !((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] >= 0xa77f && (*this)[i] <= 0xa787 &&  ((*this)[i] & 1))	retVal[i] = (*this)[i] - 0x1;	// Special letters
		else if	((*this)[i] == 0xa78c)							retVal[i] =		 0xa78b;	// Single character
		else if	((*this)[i] >= 0xff41 && (*this)[i] <= 0xff5a)				retVal[i] = (*this)[i] - 0x20;	// Alphabet															
	}							
								
	mutex.Release();					
								
	return retVal;						
}								
