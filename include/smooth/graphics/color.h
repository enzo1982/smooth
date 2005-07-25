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

namespace smooth
{
	namespace GUI
	{
		class Color;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 RGB		= 0;	// Red, Green, Blue
		const Int	 HSV		= 1;	// Hue, Saturation, Value
		const Int	 YUV		= 2;	// Value, Blue, Red
		const Int	 CMY		= 3;	// Cyan, Magenta, Yellow (subtractive)
		const Int	 CMYK		= 4;	// Cyan, Magenta, Yellow, Black
		const Int	 CIELAB		= 5;	// Lightness, Red/Green, Yellow/Blue
		const Int	 COLORFORGE	= 6;	// Saturation, Red->Green, (Red->Green)->Blue
		const Int	 GRAY		= 7;	// Black
		const Int 	 HIFI		= 8;	// High fidelity images (Cyan, Magenty, Yellow, Red, Green, Blue, Black)
		const Int	 ACMY		= 9;	// Cyan, Magenta, Yellow (additive)

		class SMOOTHAPI Color
		{
			protected:
				Long		 color;
			public:
						 Color();
						 Color(const Color &);
						 Color(Long);
						 Color(Int, Int, Int, Int = 0);

				Int		 GetRed()					{ return color & 255; }
				Int		 GetGreen()					{ return (color >> 8) & 255; }
				Int		 GetBlue()					{ return (color >> 16) & 255; }
				Int		 GetAlpha()					{ return (color >> 24) & 255; }

				Void		 SetColor(Long nColor)				{ color = nColor; }
				Void		 SetColor(Int r, Int g, Int b, Int a = 0)	{ color = r + g * 256 + b * 65536 + a * 16777216; }

				Color &operator	 =(const Long nColor)				{ SetColor(nColor); return *this; }
				operator	 Long() const					{ return color; }

				static Color	 ConvertColor(Long, Int, Int);

				static Color	 Average(Color, Color);

				static Color	 DownsampleColor(Long, Int);
				static Color	 UpsampleColor(Long, Int);
		};
	};
};

#endif
