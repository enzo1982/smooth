 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/surfacebackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/color.h>

S::GUI::SurfaceBackend *CreateSurfaceBackend(S::Void *iSurface)
{
	return new S::GUI::SurfaceBackend(iSurface);
}

S::Int	 surfaceBackendTmp = S::GUI::SurfaceBackend::AddBackend(&CreateSurfaceBackend);

S::Array<S::GUI::SurfaceBackend *(*)(S::Void *)>	*S::GUI::SurfaceBackend::backend_creators = NIL;

S::Int S::GUI::SurfaceBackend::AddBackend(SurfaceBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error;

	if (backend_creators == NIL) backend_creators = new Array<SurfaceBackend *(*)(Void *)>;

	backend_creators->AddEntry(backend);

	return Success;
}

S::GUI::SurfaceBackend *S::GUI::SurfaceBackend::CreateBackendInstance(Void *iSurface)
{
	if (backend_creators->GetFirstEntry() != &CreateSurfaceBackend)	return backend_creators->GetFirstEntry()(iSurface);
	else								return backend_creators->GetLastEntry()(iSurface);
}

S::GUI::SurfaceBackend::SurfaceBackend(Void *iSurface)
{
	type = SURFACE_NONE;

	size.cx	= 0;
	size.cy	= 0;

	paintRect.left = -1;
	paintRect.top = -1;
	paintRect.right = -1;
	paintRect.bottom = -1;

	painting = 0;
}

S::GUI::SurfaceBackend::~SurfaceBackend()
{
}

S::Int S::GUI::SurfaceBackend::TranslateX(Int x)
{
	if (Setup::rightToLeft)	return size.cx - x;
	else			return x;
}

S::Point S::GUI::SurfaceBackend::TranslatePoint(Point p)
{
	if (Setup::rightToLeft)	return Point(size.cx - p.x, p.y);
	else			return p;
}

S::Rect S::GUI::SurfaceBackend::TranslateRect(Rect r)
{
	if (Setup::rightToLeft)	return Rect(Point(size.cx - r.right, r.top), Size(r.right - r.left, r.bottom - r.top));
	else			return r;
}

S::Int S::GUI::SurfaceBackend::GetSurfaceType()
{
	return type;
}

S::Int S::GUI::SurfaceBackend::SetSize(Size nSize)
{
	size = nSize;

	return Success;
}

const S::Size S::GUI::SurfaceBackend::GetSize()
{
	return size;
}

S::Int S::GUI::SurfaceBackend::PaintRect(Rect pRect)
{
	return Success;
}

S::Int S::GUI::SurfaceBackend::StartPaint(Rect pRect)
{
	painting++;

	return Success;
}

S::Int S::GUI::SurfaceBackend::EndPaint()
{
	painting--;

	return Success;
}

S::Void *S::GUI::SurfaceBackend::GetSystemSurface()
{
	return NIL;
}

S::Int S::GUI::SurfaceBackend::SetPixel(Int x, Int y, Int color)
{
	return Success;
}

S::Int S::GUI::SurfaceBackend::GetPixel(Int x, Int y)
{
	return 0;
}

S::Int S::GUI::SurfaceBackend::Line(Point pos1, Point pos2, Int color)
{
	return Success;
}

S::Int S::GUI::SurfaceBackend::Frame(Rect rect, Int style)
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

S::Int S::GUI::SurfaceBackend::Box(Rect rect, Int color, Int style)
{
	return Success;
}

S::Int S::GUI::SurfaceBackend::SetText(String string, Rect rect, Font font, Bool shadow)
{
	if (shadow)
	{
		rect.left += 2;
		rect.top += 2;
		rect.right += 2;
		rect.bottom += 2;

		Int	 fontColor = font.GetColor();

		font.SetColor(CombineColor((Int) ((Float) GetRed(Setup::BackgroundColor) / 3 * 2), (Int) ((Float) GetGreen(Setup::BackgroundColor) / 3 * 2), (Int) ((Float) GetBlue(Setup::BackgroundColor) / 3 * 2)));

		SetText(string, rect, font, False);

		rect.left -= 2;
		rect.top -= 2;
		rect.right -= 2;
		rect.bottom -= 2;

		font.SetColor(fontColor);

		SetText(string, rect, font, False);
	}

	return Success;
}

S::Int S::GUI::SurfaceBackend::Gradient(Rect rect, Int color1, Int color2, Int style)
{
	Float	 red1	= GetRed(color1);
	Float	 green1	= GetGreen(color1);
	Float	 blue1	= GetBlue(color1);
	Float	 red2	= GetRed(color2);
	Float	 green2	= GetGreen(color2);
	Float	 blue2	= GetBlue(color2);
	Int	 xmax	= rect.right - rect.left;
	Int	 ymax	= rect.bottom - rect.top;
	Bitmap	 bmp(rect.right - rect.left, rect.bottom - rect.top, 24);

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

					for (Int y = 0; y < ymax; y++) bmp.SetPixel(x, y, CombineColor((Int) red1, (Int) green1, (Int) blue1));
				}
			}
			else
			{
				for (Int x = 0; x < xmax; x++)
				{
					red1	= red1 + biasr;
					green1	= green1 + biasg;
					blue1	= blue1 + biasb;

					for (Int y = 0; y < ymax; y++) bmp.SetPixel(x, y, CombineColor((Int) red1, (Int) green1, (Int) blue1));
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

				for (Int x = 0; x < xmax; x++) bmp.SetPixel(x, y, CombineColor((Int) red1, (Int) green1, (Int) blue1));
			}
		}
		break;
	}

	Rect	 srect;

	srect.left	= 0;
	srect.top	= 0;
	srect.right	= rect.right - rect.left;
	srect.bottom	= rect.bottom - rect.top;

	BlitFromBitmap(bmp, srect, rect);

	return Success;
}

S::Int S::GUI::SurfaceBackend::Bar(Point p1, Point p2, Int orientation)
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

S::Int S::GUI::SurfaceBackend::BlitFromBitmap(const Bitmap &bitmap, Rect srcRect, Rect destRect)
{
	return Success;
}

S::Int S::GUI::SurfaceBackend::BlitToBitmap(Rect srcRect, const Bitmap &bitmap, Rect destRect)
{
	return Success;
}
