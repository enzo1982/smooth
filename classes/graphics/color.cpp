 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

S::GUI::Color::Color()
{
	color = 0;
}

S::GUI::Color::Color(const Color &iColor)
{
	color = iColor.color;
}

S::GUI::Color::Color(Int iColor)
{
	color = iColor;
}

S::GUI::Color::Color(Int r, Int g, Int b, Int a)
{
	SetColor(r, g, b, a);
}

S::GUI::Color S::GUI::Color::ConvertColor(Long col, Int cs1, Int cs2)
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
	double	f;
	double	p;
	double	q;
	double	t;
	double	min;
	double	max;
	double	delta;

	switch (cs1)
	{
		case RGB:
			switch (cs2)
			{
				case RGB:
					return Color(col);
				case HSV:
					r = Color(col).GetRed();
					g = Color(col).GetGreen();
					b = Color(col).GetBlue();

					if (r > g)
					{
						if (r > b)	max = r;
						else		max = b;
						if (g < b)	min = g;
						else		min = b;
					}
					else
					{
						if (g > b)	max = g;
						else		max = b;
						if (r < b)	min = r;
						else		min = b;
					}

					v = max;

					if (max != 0)	s = (max - min) / max;
					else		s = 0;

					if (s == 0)	h = 0;
					else
					{
						h = 0;
						delta = max - min;

						if (r == max)		h = (g - b) / delta;
						else if (g == max)	h = 2 + (b - r) / delta;
						else if (b == max)	h = 4 + (r - g) / delta;

						h *= 60;

						if (h < 0)	h += 360;
					}

					p1 = h / 360 * 255;
					p2 = s * 255;
					p3 = v;

					p1 = Math::Round(p1);
					p2 = Math::Round(p2);

					return Color((int) p1, (int) p2, (int) p3);
				case YUV:
					p1 = (Color(col).GetRed() + Color(col).GetGreen() + Color(col).GetBlue()) / 3;
					p2 = Color(col).GetBlue();
					p3 = Color(col).GetRed();

					p1 = Math::Round(p1);

					return Color((int) p1, (int) p2, (int) p3);
				case CMY:
					p1 = 255 - Color(col).GetRed();
					p2 = 255 - Color(col).GetGreen();
					p3 = 255 - Color(col).GetBlue();

					return Color((int) p1, (int) p2, (int) p3);
				case CMYK:
					r = Color(col).GetRed();
					g = Color(col).GetGreen();
					b = Color(col).GetBlue();

					if (r > g)
					{
						if (r > b)	p4 = 255 - r;
						else		p4 = 255 - b;
					}
					else
					{
						if (g > b)	p4 = 255 - g;
						else		p4 = 255 - b;
					}

					p1 = 255 - r - p4;
					p2 = 255 - g - p4;
					p3 = 255 - b - p4;

					return Color((Int) p1, (Int) p2, (Int) p3, (Int) p4);
				case GRAY:
					return Color((Color(col).GetRed() + Color(col).GetGreen() + Color(col).GetBlue()) / 3);
				default:
					break;
			}

			break;
		case HSV:
			switch (cs2)
			{
				case RGB:
					h = ((double) Color(col).GetRed()) / 255 * 360;
					s = ((double) Color(col).GetGreen()) / 255;
					v = Color(col).GetBlue();

					if (s == 0)
					{
						p1 = v;
						p2 = v;
						p3 = v;
					}
					else
					{
						while (h < 0)		h += 360;
						while (h >= 360)	h -= 360;

						h /= 60;

						f = h - (int) h;
						p = v * (1 - s);
						q = v * (1 - (s * f));
						t = v * (1 - (s * (1 - f)));

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

					return Color((int) p1, (int) p2, (int) p3);
				case HSV:
					return Color(col);
				case YUV:
					return ConvertColor(RGB, YUV, ConvertColor(HSV, RGB, col));
				case CMY:
					return ConvertColor(RGB, CMY, ConvertColor(HSV, RGB, col));
				case CMYK:
					return ConvertColor(RGB, CMYK, ConvertColor(HSV, RGB, col));
				case GRAY:
					return ConvertColor(RGB, GRAY, ConvertColor(HSV, RGB, col));
				default:
					break;
			}
		case YUV:
			switch (cs2)
			{
				case RGB:
					p1 = Color(col).GetBlue();
					p3 = Color(col).GetGreen();
					p2 = 3 * Color(col).GetRed() - p1 - p3;

					if (p2 < 0) p2 = 0;

					return Color((int) p1, (int) p2, (int) p3);
				case HSV:
					return ConvertColor(RGB, HSV, ConvertColor(YUV, RGB, col));
				case YUV:
					return Color(col);
				case CMY:
					return ConvertColor(RGB, CMY, ConvertColor(YUV, RGB, col));
				case CMYK:
					return ConvertColor(RGB, CMYK, ConvertColor(YUV, RGB, col));
				case GRAY:
					return ConvertColor(RGB, GRAY, ConvertColor(YUV, RGB, col));
				default:
					break;
			}

			break;
		case CMY:
			switch (cs2)
			{
				case RGB:
					p1 = 255 - Color(col).GetRed();
					p2 = 255 - Color(col).GetGreen();
					p3 = 255 - Color(col).GetBlue();

					return Color((int) p1, (int) p2, (int) p3);
				case HSV:
					return ConvertColor(RGB, HSV, ConvertColor(CMY, RGB, col));
				case YUV:
					return ConvertColor(RGB, YUV, ConvertColor(CMY, RGB, col));
				case CMY:
					return Color(col);
				case CMYK:
					return ConvertColor(RGB, CMYK, ConvertColor(CMY, RGB, col));
				case GRAY:
					return ConvertColor(RGB, GRAY, ConvertColor(CMY, RGB, col));
				default:
					break;
			}

			break;
		case CMYK:
			switch (cs2)
			{
				case RGB:
					p4 = Color(col).GetAlpha();
					p1 = 255 - Color(col).GetRed() - p4;
					p2 = 255 - Color(col).GetGreen() - p4;
					p3 = 255 - Color(col).GetBlue() - p4;

					return Color((int) p1, (int) p2, (int) p3);
				case HSV:
					return ConvertColor(RGB, HSV, ConvertColor(CMYK, RGB, col));
				case YUV:
					return ConvertColor(RGB, YUV, ConvertColor(CMYK, RGB, col));
				case CMY:
					return ConvertColor(RGB, CMY, ConvertColor(CMYK, RGB, col));
				case CMYK:
					return Color(col);
				case GRAY:
					return ConvertColor(RGB, GRAY, ConvertColor(CMYK, RGB, col));
				default:
					break;
			}

			break;
		case GRAY:
			switch (cs2)
			{
				case RGB:
					return Color(col, col, col);
				case HSV:
					return ConvertColor(RGB, HSV, ConvertColor(GRAY, RGB, col));
				case YUV:
					return ConvertColor(RGB, YUV, ConvertColor(GRAY, RGB, col));
				case CMY:
					return ConvertColor(RGB, CMY, ConvertColor(GRAY, RGB, col));
				case CMYK:
					return ConvertColor(RGB, CMYK, ConvertColor(GRAY, RGB, col));
				case GRAY:
					return Color(col);
				default:
					break;
			}

			break;
		default:
			break;
	}

	return Color(col);
}

S::GUI::Color S::GUI::Color::Average(Color c1, Color c2)
{
	return Color((c1.GetRed() + c2.GetRed()) / 2, (c1.GetGreen() + c2.GetGreen()) / 2, (c1.GetBlue() + c2.GetBlue()) / 2, (c1.GetAlpha() + c2.GetAlpha()) / 2);
}

S::GUI::Color S::GUI::Color::DownsampleColor(Long col, Int bpcc)
{
	if (bpcc == 8) return Color(col);

	int first = Color(col).GetRed();
	int second = Color(col).GetGreen();
	int third = Color(col).GetBlue();
	int fourth = Color(col).GetAlpha();

	first >>= (8 - bpcc);
	second >>= (8 - bpcc);
	third >>= (8 - bpcc);
	fourth >>= (8 - bpcc);

	return Color((Long) (first + Math::Pow(2, bpcc) * second + Math::Pow(4, bpcc) * third + Math::Pow(8, bpcc) * fourth));
}

S::GUI::Color S::GUI::Color::UpsampleColor(Long col, Int bpcc)
{
	if (bpcc == 8) return Color(col);

	int first = col & (int) (Math::Pow(2, bpcc) - 1);
	int second = (col >> bpcc) & (int) (Math::Pow(2, bpcc) - 1);
	int third = (col >> (2 * bpcc)) & (int) (Math::Pow(2, bpcc) - 1);
	int fourth = (col >> (3 * bpcc)) & (int) (Math::Pow(2, bpcc) - 1);

	first = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) first);
	second = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) second);
	third = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) third);
	fourth = (int) (255 / (Math::Pow(2, bpcc) - 1) * (double) fourth);

	return Color(first, second, third, fourth);
}
