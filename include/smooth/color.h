 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COLOR_
#define _H_OBJSMOOTH_COLOR_

#include "definitions.h"

const S::Int	 RGB		= 0;	// Red, Green, Blue
const S::Int	 HSV		= 1;	// Hue, Saturation, Value
const S::Int	 YUV		= 2;	// Value, Blue, Red
const S::Int	 CMY		= 3;	// Cyan, Magenta, Yellow (subtractive)
const S::Int	 CMYK		= 4;	// Cyan, Magenta, Yellow, Black
const S::Int	 CIELAB		= 5;	// Lightness, Red/Green, Yellow/Blue
const S::Int	 COLORFORGE	= 6;	// Saturation, Red->Green, (Red->Green)->Blue
const S::Int	 GRAY		= 7;	// Black
const S::Int 	 HIFI		= 8;	// High fidelity images (Cyan, Magenty, Yellow, Red, Green, Blue, Black)
const S::Int	 ACMY		= 9;	// Cyan, Magenta, Yellow (additive)

inline S::Int GetRed(S::UnsignedLong color)
{ return color & 255; }

inline S::Int GetGreen(S::UnsignedLong color)
{ return (color >> 8) & 255; }

inline S::Int GetBlue(S::UnsignedLong color)
{ return (color >> 16) & 255; }

inline S::Int GetAlpha(S::UnsignedLong color)
{ return (color >> 24) & 255; }

inline S::Long CombineColor(S::Int a, S::Int b, S::Int c)
{ return a + b * 256 + c * 65536; }

inline S::Long CombineColor(S::Int a, S::Int b, S::Int c, S::Int d)
{ return a + b * 256 + c * 65536 + d * 16777216; }

S::Int ConvertColor(S::Int, S::Int, S::Int);
S::Int DownsampleColor(S::Int, S::Int);
S::Int UpsampleColor(S::Int, S::Int);

#endif
