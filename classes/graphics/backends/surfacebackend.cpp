 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/surfacebackend.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/graphics/backends/gdi/surfacegdi.h>
#endif

S::GUI::SurfaceBackend *CreateSurfaceBackend(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceBackend(iSurface, maxSize);
}

S::GUI::SurfaceBackend *(*S::GUI::SurfaceBackend::backend_creator)(S::Void *, const S::GUI::Size &) = &CreateSurfaceBackend;

S::Int S::GUI::SurfaceBackend::SetBackend(SurfaceBackend *(*backend)(Void *, const Size &))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::GUI::SurfaceBackend *S::GUI::SurfaceBackend::CreateBackendInstance(Void *iSurface, const Size &maxSize)
{
	return backend_creator(iSurface, maxSize);
}

S::GUI::SurfaceBackend::SurfaceBackend(Void *iSurface, const Size &maxSize)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) SurfaceGDI();
#endif

	type = SURFACE_NONE;

	size.cx	= 0;
	size.cy	= 0;

	paintRect.left = -1;
	paintRect.top = -1;
	paintRect.right = -1;
	paintRect.bottom = -1;

	painting = 0;

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceBackend::~SurfaceBackend()
{
}

S::Int S::GUI::SurfaceBackend::GetSurfaceType() const
{
	return type;
}

S::Int S::GUI::SurfaceBackend::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceBackend::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceBackend::SetRightToLeft(Bool nRightToLeft)
{
	rightToLeft.SetRightToLeft(nRightToLeft);

	return Success();
}

S::Int S::GUI::SurfaceBackend::PaintRect(const Rect &pRect)
{
	return Success();
}

S::Int S::GUI::SurfaceBackend::StartPaint(const Rect &pRect)
{
	painting++;

	return Success();
}

S::Int S::GUI::SurfaceBackend::EndPaint()
{
	painting--;

	return Success();
}

S::Void *S::GUI::SurfaceBackend::GetSystemSurface() const
{
	return NIL;
}

S::Int S::GUI::SurfaceBackend::GetSurfaceDPI() const
{
	return 96;
}

S::Int S::GUI::SurfaceBackend::SetPixel(const Point &point, const Color &color)
{
	return Success();
}

S::Int S::GUI::SurfaceBackend::Line(const Point &pos1, const Point &pos2, const Color &color)
{
	return Success();
}

S::Int S::GUI::SurfaceBackend::Frame(const Rect &iRect, Int style)
{
	Rect	 rect = rightToLeft.TranslateRect(iRect);

	Point	 p1 = Point(rect.left, rect.top);
	Point	 p2 = Point(rect.right - 1, rect.top);
	Point	 p3 = Point(rect.left, rect.bottom - 1);
	Point	 p4 = Point(rect.right - 1, rect.bottom - 1);

	Long	 color1 = 0;
	Long	 color2 = 0;

	switch (style)
	{
		case FRAME_UP:
			color1 = Color(Math::Min(Setup::BackgroundColor.GetRed() + 64, 255), Math::Min(Setup::BackgroundColor.GetGreen() + 64, 255), Math::Min(Setup::BackgroundColor.GetBlue() + 64, 255));
			color2 = Color(Math::Max(Setup::BackgroundColor.GetRed() - 64, 0), Math::Max(Setup::BackgroundColor.GetGreen() - 64, 0), Math::Max(Setup::BackgroundColor.GetBlue() - 64, 0));

			break;
		case FRAME_DOWN:
			color1 = Color(Math::Max(Setup::BackgroundColor.GetRed() - 64, 0), Math::Max(Setup::BackgroundColor.GetGreen() - 64, 0), Math::Max(Setup::BackgroundColor.GetBlue() - 64, 0));
			color2 = Color(Math::Min(Setup::BackgroundColor.GetRed() + 64, 255), Math::Min(Setup::BackgroundColor.GetGreen() + 64, 255), Math::Min(Setup::BackgroundColor.GetBlue() + 64, 255));

			break;
	}

	Bool	 preRTL = rightToLeft.GetRightToLeft();

	rightToLeft.SetRightToLeft(False);

	Line(p1, p2, color1);
	Line(p1, p3, color1);
	Line(p2, p4, color2);
	Line(p3, p4, color2);

	SetPixel(p4, color2);

	rightToLeft.SetRightToLeft(preRTL);

	return Success();
}

S::Int S::GUI::SurfaceBackend::Box(const Rect &rect, const Color &color, Int style, const Size &ellipse)
{
	return Success();
}

S::Int S::GUI::SurfaceBackend::SetText(const String &string, const Rect &iRect, const Font &iFont, Bool shadow)
{
	if (shadow)
	{
		Rect	 rect = iRect + Point(2, 2);
		Font	 font = iFont;
		Int	 fontColor = font.GetColor();

		font.SetColor(Color((Int) ((Float) Setup::BackgroundColor.GetRed() / 3 * 2), (Int) ((Float) Setup::BackgroundColor.GetGreen() / 3 * 2), (Int) ((Float) Setup::BackgroundColor.GetBlue() / 3 * 2)));

		SetText(string, rect, font, False);

		rect = rect - Point(2, 2);

		font.SetColor(fontColor);

		SetText(string, rect, font, False);
	}

	return Success();
}

S::Int S::GUI::SurfaceBackend::Gradient(const Rect &rect, const Color &color1, const Color &color2, Int style)
{
	Float	 red1	= color1.GetRed();
	Float	 green1	= color1.GetGreen();
	Float	 blue1	= color1.GetBlue();
	Float	 red2	= color2.GetRed();
	Float	 green2	= color2.GetGreen();
	Float	 blue2	= color2.GetBlue();
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

			if (rightToLeft.GetRightToLeft())
			{
				for (Int x = xmax - 1; x >= 0; x--)
				{
					red1	= red1 + biasr;
					green1	= green1 + biasg;
					blue1	= blue1 + biasb;

					for (Int y = 0; y < ymax; y++) bmp.SetPixel(Point(x, y), Color((Int) red1, (Int) green1, (Int) blue1));
				}
			}
			else
			{
				for (Int x = 0; x < xmax; x++)
				{
					red1	= red1 + biasr;
					green1	= green1 + biasg;
					blue1	= blue1 + biasb;

					for (Int y = 0; y < ymax; y++) bmp.SetPixel(Point(x, y), Color((Int) red1, (Int) green1, (Int) blue1));
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

				for (Int x = 0; x < xmax; x++) bmp.SetPixel(Point(x, y), Color((Int) red1, (Int) green1, (Int) blue1));
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

	return Success();
}

S::Int S::GUI::SurfaceBackend::Bar(const Point &iP1, const Point &iP2, Int orientation)
{
	Point	 p1 = rightToLeft.TranslatePoint(iP1);
	Point	 p2 = rightToLeft.TranslatePoint(iP2);

	if (rightToLeft.GetRightToLeft())
	{
		if (orientation == OR_HORZ) { p1 = rightToLeft.TranslatePoint(iP2);	p2 = rightToLeft.TranslatePoint(iP1); }
		if (orientation == OR_VERT) { p1 -= Point(2, 0);			p2 -= Point(2, 0); }
	}

	Bool	 preRTL = rightToLeft.GetRightToLeft();

	rightToLeft.SetRightToLeft(False);

	if (orientation == OR_HORZ)
	{
		Line(p1, p2, Color(Math::Max(Setup::BackgroundColor.GetRed() - 64, 0), Math::Max(Setup::BackgroundColor.GetGreen() - 64, 0), Math::Max(Setup::BackgroundColor.GetBlue() - 64, 0)));
		p1.y++;
		p2.y++;
		Line(p1, p2, Color(Math::Min(Setup::BackgroundColor.GetRed() + 64, 255), Math::Min(Setup::BackgroundColor.GetGreen() + 64, 255), Math::Min(Setup::BackgroundColor.GetBlue() + 64, 255)));
	}
	else if (orientation == OR_VERT)
	{
		p2.y++;
		Line(p1, p2, Color(Math::Max(Setup::BackgroundColor.GetRed() - 64, 0), Math::Max(Setup::BackgroundColor.GetGreen() - 64, 0), Math::Max(Setup::BackgroundColor.GetBlue() - 64, 0)));
		p1.x++;
		p2.x++;
		Line(p1, p2, Color(Math::Min(Setup::BackgroundColor.GetRed() + 64, 255), Math::Min(Setup::BackgroundColor.GetGreen() + 64, 255), Math::Min(Setup::BackgroundColor.GetBlue() + 64, 255)));
	}

	rightToLeft.SetRightToLeft(preRTL);

	return Success();
}

S::Int S::GUI::SurfaceBackend::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &destRect)
{
	return Success();
}

S::Int S::GUI::SurfaceBackend::BlitToBitmap(const Rect &srcRect, const Bitmap &bitmap, const Rect &destRect)
{
	return Success();
}
