 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/surface.h>
#include <smooth/graphics/gdi/bitmapgdi.h>
#include <smooth/color.h>
#include <picture.h>

S::GUI::Surface::Surface()
{
	type = SURFACE_NULL;

	size.cx	= 0;
	size.cy	= 0;

	paintRect.left = -1;
	paintRect.top = -1;
	paintRect.right = -1;
	paintRect.bottom = -1;

	painting = 0;
}

S::GUI::Surface::~Surface()
{
}

S::Int S::GUI::Surface::TranslateX(Int x)
{
	if (Setup::rightToLeft)	return size.cx - x;
	else			return x;
}

S::Point S::GUI::Surface::TranslatePoint(Point p)
{
	if (Setup::rightToLeft)	return Point(size.cx - p.x, p.y);
	else			return p;
}

S::Rect S::GUI::Surface::TranslateRect(Rect r)
{
	if (Setup::rightToLeft)	return Rect(Point(size.cx - r.right, r.top), Size(r.right - r.left, r.bottom - r.top));
	else			return r;
}

S::Int S::GUI::Surface::GetSurfaceType()
{
	return type;
}

S::Int S::GUI::Surface::SetSize(Size nSize)
{
	size = nSize;

	return Success;
}

S::Size S::GUI::Surface::GetSize()
{
	return size;
}

S::Int S::GUI::Surface::PaintRect(Rect pRect)
{
	return Success;
}

S::Int S::GUI::Surface::StartPaint(Rect pRect)
{
	painting++;

	return Success;
}

S::Int S::GUI::Surface::EndPaint()
{
	painting--;

	return Success;
}

S::Int S::GUI::Surface::SetPixel(Int x, Int y, Int color)
{
	return Success;
}

S::Int S::GUI::Surface::GetPixel(Int x, Int y)
{
	return 0;
}

S::Int S::GUI::Surface::Line(Point pos1, Point pos2, Int color)
{
	return Success;
}

S::Int S::GUI::Surface::Frame(Rect rect, Int style)
{
	Long	 color1 = 0;
	Long	 color2 = 0;
	Point	 p1;
	Point	 p2;
	Point	 p3;
	Point	 p4;

	if (Setup::rightToLeft)
	{
		rect = TranslateRect(rect);
		rect.left--;
		rect.right--;
	}

	p1.x = rect.left;
	p1.y = rect.top;
	p2.x = rect.right;
	p2.y = rect.top;
	p3.x = rect.left;
	p3.y = rect.bottom;
	p4.x = rect.right;
	p4.y = rect.bottom;

	switch (style)
	{
		case FRAME_UP: // up
		{
			color1 = RGB(min(GetRed(Setup::BackgroundColor) + 64, 255), min(GetGreen(Setup::BackgroundColor) + 64, 255), min(GetBlue(Setup::BackgroundColor) + 64, 255));
			color2 = RGB(max(GetRed(Setup::BackgroundColor) - 64, 0), max(GetGreen(Setup::BackgroundColor) - 64, 0), max(GetBlue(Setup::BackgroundColor) - 64, 0));
		}
		break;

		case FRAME_DOWN: // down
		{
			color1 = RGB(max(GetRed(Setup::BackgroundColor) - 64, 0), max(GetGreen(Setup::BackgroundColor) - 64, 0), max(GetBlue(Setup::BackgroundColor) - 64, 0));
			color2 = RGB(min(GetRed(Setup::BackgroundColor) + 64, 255), min(GetGreen(Setup::BackgroundColor) + 64, 255), min(GetBlue(Setup::BackgroundColor) + 64, 255));
		}
		break;
	}

	Bool	 preRTL = Setup::rightToLeft;

	Setup::rightToLeft = False;

	Line(p1, p2, color1);
	Line(p1, p3, color1);
	Line(p2, p4, color2);
	p4.x++;
	Line(p3, p4, color2);

	Setup::rightToLeft = preRTL;

	return Success;
}

S::Int S::GUI::Surface::Box(Rect rect, Int color, Int style)
{
	return Success;
}

S::Int S::GUI::Surface::SetText(String string, Rect rect, Font font)
{
	return Success;
}

S::Int S::GUI::Surface::SetShadowedText(String string, Rect rect, Font font)
{
	if (string == NIL) return Error;

	rect.left += 2;
	rect.top += 2;
	rect.right += 2;
	rect.bottom += 2;

	Int	 fontColor = font.GetColor();

	font.SetColor(CombineColor(((Float) GetRed(Setup::BackgroundColor)) / 3 * 2, ((Float) GetGreen(Setup::BackgroundColor)) / 3 * 2, ((Float) GetBlue(Setup::BackgroundColor)) / 3 * 2));

	SetText(string, rect, font);

	rect.left -= 2;
	rect.top -= 2;
	rect.right -= 2;
	rect.bottom -= 2;

	font.SetColor(fontColor);

	SetText(string, rect, font);

	return Success;
}

S::Int S::GUI::Surface::Gradient(Rect rect, Int color1, Int color2, Int style)
{
	Float	 red1	= GetRed(color1);
	Float	 green1	= GetGreen(color1);
	Float	 blue1	= GetBlue(color1);
	Float	 red2	= GetRed(color2);
	Float	 green2	= GetGreen(color2);
	Float	 blue2	= GetBlue(color2);
	Int	 xmax	= rect.right - rect.left;
	Int	 ymax	= rect.bottom - rect.top;
	picture	*pic	= new picture(rect.right - rect.left, rect.bottom - rect.top, 24);

	switch (style)
	{
		case OR_HORZ:
		{
			Float	 biasr = (red2 - red1) / xmax;
			Float	 biasg = (green2 - green1) / xmax;
			Float	 biasb = (blue2 - blue1) / xmax;

			if (Setup::rightToLeft)
			{
				for (Int x = xmax - 1; x >= 0; x--)
				{
					red1	= red1 + biasr;
					green1	= green1 + biasg;
					blue1	= blue1 + biasb;

					for (Int y = 0; y < ymax; y++) pic->SetPixel(x, y, CombineColor(red1, green1, blue1));
				}
			}
			else
			{
				for (Int x = 0; x < xmax; x++)
				{
					red1	= red1 + biasr;
					green1	= green1 + biasg;
					blue1	= blue1 + biasb;

					for (Int y = 0; y < ymax; y++) pic->SetPixel(x, y, CombineColor(red1, green1, blue1));
				}
			}
		}
		break;

		case OR_VERT:
		{
			Float	 biasr = (red2 - red1) / ymax;
			Float	 biasg = (green2 - green1) / ymax;
			Float	 biasb = (blue2 - blue1) / ymax;

			for (Int y = 0; y < ymax; y++)
			{
				red1	= red1 + biasr;
				green1	= green1 + biasg;
				blue1	= blue1 + biasb;

				for (Int x = 0; x < xmax; x++) pic->SetPixel(x, y, CombineColor(red1, green1, blue1));
			}
		}
		break;
	}

	Rect	 srect;

	srect.left	= 0;
	srect.top	= 0;
	srect.right	= rect.right - rect.left;
	srect.bottom	= rect.bottom - rect.top;

	BitmapGDI	*bitmap = new BitmapGDI(pic->GetBitmap());

	BlitFromBitmap(bitmap, srect, rect);

	delete bitmap;

	delete pic;

	return Success;
}

S::Int S::GUI::Surface::Bar(Point p1, Point p2, Int orientation)
{
	if (Setup::rightToLeft && orientation == OR_HORZ)
	{
		Point	 px = TranslatePoint(p1);

		p1 = TranslatePoint(p2);
		p2 = px;
	}
	else
	{
		p1 = TranslatePoint(p1);
		p2 = TranslatePoint(p2);
	}

	Bool	 preRTL = Setup::rightToLeft;

	Setup::rightToLeft = False;

	if (orientation == OR_HORZ)
	{
		Line(p1, p2, RGB(max(GetRed(Setup::BackgroundColor) - 64, 0), max(GetGreen(Setup::BackgroundColor) - 64, 0), max(GetBlue(Setup::BackgroundColor) - 64, 0)));
		SetPixel(p2.x, p2.y, RGB(min(GetRed(Setup::BackgroundColor) + 64, 255), min(GetGreen(Setup::BackgroundColor) + 64, 255), min(GetBlue(Setup::BackgroundColor) + 64, 255)));
		p1.y++;
		p2.y++;
		p2.x++;
		Line(p1, p2, RGB(min(GetRed(Setup::BackgroundColor) + 64, 255), min(GetGreen(Setup::BackgroundColor) + 64, 255), min(GetBlue(Setup::BackgroundColor) + 64, 255)));
	}
	else if (orientation == OR_VERT)
	{
		p2.y++;
		Line(p1, p2, RGB(max(GetRed(Setup::BackgroundColor) - 64, 0), max(GetGreen(Setup::BackgroundColor) - 64, 0), max(GetBlue(Setup::BackgroundColor) - 64, 0)));
		p1.x++;
		p2.x++;
		Line(p1, p2, RGB(min(GetRed(Setup::BackgroundColor) + 64, 255), min(GetGreen(Setup::BackgroundColor) + 64, 255), min(GetBlue(Setup::BackgroundColor) + 64, 255)));
	}

	Setup::rightToLeft = preRTL;

	return Success;
}

S::Int S::GUI::Surface::BlitFromBitmap(Bitmap *bitmap, Rect srcRect, Rect destRect)
{
	return Success;
}

S::Int S::GUI::Surface::BlitToBitmap(Rect srcRect, Bitmap *bitmap, Rect destRect)
{
	return Success;
}
