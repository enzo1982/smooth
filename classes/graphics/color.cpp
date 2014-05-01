 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

S::GUI::Color S::GUI::Color::ConvertTo(ColorSpace cs) const
{
	double	p1 = 0;
	double	p2 = 0;
	double	p3 = 0;
	double	p4 = 0;
	double	r;
	double	g;
	double	b;
	double	h;
	double	s;
	double	v;
	double	min;
	double	max;

	switch (colorSpace)
	{
		case RGB:
			switch (cs)
			{
				case RGB:
					return *this;
				case RGBA:
					return Color(255 << 24 | color, RGBA);
				case HSV:
					r = GetRed();
					g = GetGreen();
					b = GetBlue();

					max = Math::Max(Math::Max(r, g), b);
					min = Math::Min(Math::Min(r, g), b);

					v = max;

					if (max != 0) s = (max - min) / max;
					else	      s = 0;

					h = 0;

					if (s != 0)
					{
						double	 delta = max - min;

						if	(r == max) h =	   (g - b) / delta;
						else if (g == max) h = 2 + (b - r) / delta;
						else if (b == max) h = 4 + (r - g) / delta;

						h *= 60;

						if (h < 0) h += 360;
					}

					p1 = h / 360 * 255;
					p2 = s * 255;
					p3 = v;

					p1 = Math::Round(p1);
					p2 = Math::Round(p2);

					return Color((int) p1, (int) p2, (int) p3, HSV);
				case YUV:
					p1 = (GetRed() + GetGreen() + GetBlue()) / 3;
					p2 =  GetBlue();
					p3 =  GetRed();

					p1 = Math::Round(p1);

					return Color((int) p1, (int) p2, (int) p3, YUV);
				case CMY:
					p1 = 255 - GetRed();
					p2 = 255 - GetGreen();
					p3 = 255 - GetBlue();

					return Color((int) p1, (int) p2, (int) p3, CMY);
				case CMYK:
					r = GetRed();
					g = GetGreen();
					b = GetBlue();

					p4 = 255 - Math::Max(Math::Max(r, g), b);
					p1 = 255 - r - p4;
					p2 = 255 - g - p4;
					p3 = 255 - b - p4;

					return Color(((Int) p1) | ((Int) p2) << 8 | ((Int) p3) << 16 | ((Int) p4) << 24, CMYK);
				case GRAY:
					return Color((GetRed() + GetGreen() + GetBlue()) / 3, GRAY);
				default:
					break;
			}

			break;
		case RGBA:
			switch (cs)
			{
				case RGB:
					return Color(GetRed(), GetGreen(), GetBlue(), RGB);
				case RGBA:
					return *this;
				case HSV:
					return ConvertTo(RGB).ConvertTo(HSV);
				case YUV:
					return ConvertTo(RGB).ConvertTo(YUV);
				case CMY:
					return ConvertTo(RGB).ConvertTo(CMY);
				case CMYK:
					return ConvertTo(RGB).ConvertTo(CMYK);
				case GRAY:
					return ConvertTo(RGB).ConvertTo(GRAY);
				default:
					break;
			}

			break;
		case HSV:
			switch (cs)
			{
				case RGB:
					h = ((double) GetRed()) / 255 * 360;
					s = ((double) GetGreen()) / 255;
					v = GetBlue();

					if (s == 0)
					{
						p1 = v;
						p2 = v;
						p3 = v;
					}
					else
					{
						while (h <    0) h += 360;
						while (h >= 360) h -= 360;

						h /= 60;

						double	 f = h - (int) h;
						double	 p = v * (1 -  s	   );
						double	 q = v * (1 - (s *      f ));
						double	 t = v * (1 - (s * (1 - f)));

						switch ((int) h)
						{
							case 0:
								p1 = v;
								p2 = t;
								p3 = p;
								break;
							case 1:
								p1 = q;
								p2 = v;
								p3 = p;
								break;
							case 2:
								p1 = p;
								p2 = v;
								p3 = t;
								break;
							case 3:
								p1 = p;
								p2 = q;
								p3 = v;
								break;
							case 4:
								p1 = t;
								p2 = p;
								p3 = v;
								break;
							case 5:
								p1 = v;
								p2 = p;
								p3 = q;
								break;
							default:
								break;
						}
					}

					p1 = Math::Round(p1);
					p2 = Math::Round(p2);
					p3 = Math::Round(p3);

					return Color((int) p1, (int) p2, (int) p3, RGB);
				case RGBA:
					return ConvertTo(RGB).ConvertTo(RGBA);
				case HSV:
					return *this;
				case YUV:
					return ConvertTo(RGB).ConvertTo(YUV);
				case CMY:
					return ConvertTo(RGB).ConvertTo(CMY);
				case CMYK:
					return ConvertTo(RGB).ConvertTo(CMYK);
				case GRAY:
					return ConvertTo(RGB).ConvertTo(GRAY);
				default:
					break;
			}

			break;
		case YUV:
			switch (cs)
			{
				case RGB:
					p1 =	 GetBlue();
					p3 =	 GetGreen();
					p2 = 3 * GetRed() - p1 - p3;

					if (p2 < 0) p2 = 0;

					return Color((int) p1, (int) p2, (int) p3, RGB);
				case RGBA:
					return ConvertTo(RGB).ConvertTo(RGBA);
				case HSV:
					return ConvertTo(RGB).ConvertTo(HSV);
				case YUV:
					return *this;
				case CMY:
					return ConvertTo(RGB).ConvertTo(CMY);
				case CMYK:
					return ConvertTo(RGB).ConvertTo(CMYK);
				case GRAY:
					return ConvertTo(RGB).ConvertTo(GRAY);
				default:
					break;
			}

			break;
		case CMY:
			switch (cs)
			{
				case RGB:
					p1 = 255 - GetRed();
					p2 = 255 - GetGreen();
					p3 = 255 - GetBlue();

					return Color((int) p1, (int) p2, (int) p3, RGB);
				case RGBA:
					return ConvertTo(RGB).ConvertTo(RGBA);
				case HSV:
					return ConvertTo(RGB).ConvertTo(HSV);
				case YUV:
					return ConvertTo(RGB).ConvertTo(YUV);
				case CMY:
					return *this;
				case CMYK:
					return ConvertTo(RGB).ConvertTo(CMYK);
				case GRAY:
					return ConvertTo(RGB).ConvertTo(GRAY);
				default:
					break;
			}

			break;
		case CMYK:
			switch (cs)
			{
				case RGB:
					p4 =	   GetAlpha();
					p1 = 255 - GetRed()   - p4;
					p2 = 255 - GetGreen() - p4;
					p3 = 255 - GetBlue()  - p4;

					return Color((int) p1, (int) p2, (int) p3, RGB);
				case RGBA:
					return ConvertTo(RGB).ConvertTo(RGBA);
				case HSV:
					return ConvertTo(RGB).ConvertTo(HSV);
				case YUV:
					return ConvertTo(RGB).ConvertTo(YUV);
				case CMY:
					return ConvertTo(RGB).ConvertTo(CMY);
				case CMYK:
					return *this;
				case GRAY:
					return ConvertTo(RGB).ConvertTo(GRAY);
				default:
					break;
			}

			break;
		case GRAY:
			switch (cs)
			{
				case RGB:
					return Color(color, color, color, RGB);
				case RGBA:
					return ConvertTo(RGB).ConvertTo(RGBA);
				case HSV:
					return ConvertTo(RGB).ConvertTo(HSV);
				case YUV:
					return ConvertTo(RGB).ConvertTo(YUV);
				case CMY:
					return ConvertTo(RGB).ConvertTo(CMY);
				case CMYK:
					return ConvertTo(RGB).ConvertTo(CMYK);
				case GRAY:
					return *this;
				default:
					break;
			}

			break;
		default:
			break;
	}

	return *this;
}

S::GUI::Color S::GUI::Color::Downsample(Int bpcc) const
{
	if (bpcc == 8) return *this;

	int first  = GetRed();
	int second = GetGreen();
	int third  = GetBlue();

	first  >>= (8 - bpcc);
	second >>= (8 - bpcc);
	third  >>= (8 - bpcc);

	return Color((Long) (first + Math::Pow(2, bpcc) * second + Math::Pow(4, bpcc) * third), colorSpace);
}

S::GUI::Color S::GUI::Color::Upsample(Int bpcc) const
{
	if (bpcc == 8) return *this;

	int first  =  color		   & (int) (Math::Pow(2, bpcc) - 1);
	int second = (color >>	    bpcc ) & (int) (Math::Pow(2, bpcc) - 1);
	int third  = (color >> (2 * bpcc)) & (int) (Math::Pow(2, bpcc) - 1);

	first  = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) first);
	second = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) second);
	third  = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) third);

	return Color(first, second, third, colorSpace);
}
