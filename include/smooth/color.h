 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COLOR_
#define _H_OBJSMOOTH_COLOR_

const int	 RGB		= 0;	// Red, Green, Blue
const int	 HSV		= 1;	// Hue, Saturation, Value
const int	 YUV		= 2;	// Value, Blue, Red
const int	 CMY		= 3;	// Cyan, Magenta, Yellow (subtractive)
const int	 CMYK		= 4;	// Cyan, Magenta, Yellow, Black
const int	 CIELAB		= 5;	// Lightness, Red/Green, Yellow/Blue
const int	 COLORFORGE	= 6;	// Saturation, Red->Green, (Red->Green)->Blue
const int	 GRAY		= 7;	// Black
const int 	 HIFI		= 8;	// High fidelity images (Cyan, Magenty, Yellow, Red, Green, Blue, Black)
const int	 ACMY		= 9;	// Cyan, Magenta, Yellow (additive)

int ConvertColor(int, int, int);
int DownsampleColor(int, int);
int UpsampleColor(int, int);

#endif
