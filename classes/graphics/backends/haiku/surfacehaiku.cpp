 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/haiku/surfacehaiku.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <Screen.h>
#include <Bitmap.h>
#include <Region.h>
#include <Gradient.h>
#include <GradientLinear.h>

S::GUI::SurfaceBackend *CreateSurfaceHaiku(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceHaiku(iSurface, maxSize);
}

S::Int		 surfaceHaikuTmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceHaiku);

S::Short	 S::GUI::SurfaceHaiku::surfaceDPI = -1;

S::GUI::SurfaceHaiku::SurfaceHaiku(Void *iView, const Size &maxSize)
{
	type	   = SURFACE_HAIKU;

	view	   = (BView *) iView;

	bitmap	   = NIL;
	bitmapView = NIL;

	if (view != NIL)
	{
		size = maxSize;

		BScreen	 screen;
		BRect	 frame = screen.Frame();

		if (maxSize == Size())
		{
			size.cx	= 1 + frame.right  + 2;
			size.cy	= 1 + frame.bottom + 2;
		}

		rightToLeft.SetSurfaceSize(size);

		bitmap	   = new BBitmap(BRect(0, 0, size.cx, size.cy), screen.ColorSpace(), true);
		bitmapView = new BView(bitmap->Bounds(), NULL, B_FOLLOW_ALL_SIDES, 0);

		bitmap->AddChild(bitmapView);

		paintRects.Add(new Rect(Point(0, 0), size));

		allocSize = size;
	}

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceHaiku::~SurfaceHaiku()
{
	if (view != NIL)
	{
		bitmap->RemoveChild(bitmapView);

		delete bitmapView;
		delete bitmap;

		delete paintRects.GetFirst();
	}
}

S::Int S::GUI::SurfaceHaiku::Lock()
{
	if (view != NIL)
	{
		BWindow	*window = view->Window();

		window->Lock();
		bitmap->Lock();

		return Success();
	}

	return SurfaceBackend::Lock();
}

S::Int S::GUI::SurfaceHaiku::Release()
{
	if (view != NIL)
	{
		BWindow	*window = view->Window();

		window->Unlock();
		bitmap->Unlock();

		return Success();
	}

	return SurfaceBackend::Release();
}

S::Int S::GUI::SurfaceHaiku::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (view != NIL && !painting)
	{
		bitmap->RemoveChild(bitmapView);

		delete bitmapView;
		delete bitmap;

		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		bitmap	   = new BBitmap(BRect(0, 0, size.cx, size.cy), B_RGB32, true);
		bitmapView = new BView(bitmap->Bounds(), NULL, B_FOLLOW_ALL_SIDES, 0);

		bitmap->AddChild(bitmapView);

		paintRects.Add(new Rect(Point(0, 0), size));
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceHaiku::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceHaiku::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (view != NIL)
	{
		bitmapView->Sync();

		view->DrawBitmapAsync(bitmap, BRect(pRect.left, pRect.top, pRect.right, pRect.bottom), BRect(pRect.left, pRect.top, pRect.right, pRect.bottom));
	}

	return Success();
}

S::Int S::GUI::SurfaceHaiku::StartPaint(const Rect &iPRect)
{
	if (view == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), *(paintRects.GetLast()));

	BRegion	 clippingRegion(BRect(pRect.left, pRect.top, pRect.right, pRect.bottom));

	bitmapView->PushState();
	bitmapView->ConstrainClippingRegion(&clippingRegion);

	paintRects.Add(new Rect(pRect));

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceHaiku::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0) PaintRect(*(paintRects.GetLast()));

	delete paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	bitmapView->PopState();

	return Success();
}

S::Void *S::GUI::SurfaceHaiku::GetSystemSurface() const
{
	return (Void *) view;
}

S::Short S::GUI::SurfaceHaiku::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	Short	 dpi = 96;

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceHaiku::SetPixel(const Point &iPoint, const Color &color)
{
	if (view == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	if (!painting)
	{
		view->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());
		view->FillRect(BRect(point.x, point.y, point.x, point.y));
	}

	bitmapView->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());
	bitmapView->FillRect(BRect(point.x, point.y, point.x, point.y));

	return Success();
}

S::Int S::GUI::SurfaceHaiku::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (view == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if	(pos1.x < pos2.x) pos2.x--;
		else if (pos1.x > pos2.x) pos2.x++;

		if	(pos1.y < pos2.y) pos2.y--;
		else if (pos1.y > pos2.y) pos2.y++;
	}

	/* Adjust to Windows GDI behaviour for horizontal and vertical lines.
	 */
	if (pos1.x == pos2.x && pos1.y < pos2.y) pos2.y--;
	if (pos1.x == pos2.x && pos1.y > pos2.y) pos1.y--;

	if (pos1.y == pos2.y && pos1.x < pos2.x) pos2.x--;
	if (pos1.y == pos2.y && pos1.x > pos2.x) pos1.x--;

	if (!painting)
	{
		view->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());
		view->MovePenTo(BPoint(pos1.x, pos1.y));
		view->StrokeLine(BPoint(pos2.x, pos2.y));
	}

	bitmapView->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());
	bitmapView->MovePenTo(BPoint(pos1.x, pos1.y));
	bitmapView->StrokeLine(BPoint(pos2.x, pos2.y));

	return Success();
}

S::Int S::GUI::SurfaceHaiku::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (view == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect) - Size(1, 1);

	if (!painting) view->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());
	bitmapView->SetHighColor(color.GetRed(), color.GetGreen(), color.GetBlue());

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			if (!painting) view->FillRoundRect(BRect(rect.left, rect.top, rect.right, rect.bottom), ellipse.cx, ellipse.cy);
			bitmapView->FillRoundRect(BRect(rect.left, rect.top, rect.right, rect.bottom), ellipse.cx, ellipse.cy);
		}
		else
		{
			if (!painting) view->FillRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
			bitmapView->FillRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting) view->StrokeRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
		bitmapView->StrokeRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
	}
	else if (style & Rect::Inverted)
	{
		if (!painting) view->InvertRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
		bitmapView->InvertRect(BRect(rect.left, rect.top, rect.right, rect.bottom));
	}
	else if (style & Rect::Dotted)
	{
		pattern	 pattern = {{ 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA }};

		if (!painting) view->StrokeRect(BRect(rect.left, rect.top, rect.right, rect.bottom), pattern);
		bitmapView->StrokeRect(BRect(rect.left, rect.top, rect.right, rect.bottom), pattern);
	}

	return Success();
}

S::Int S::GUI::SurfaceHaiku::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (view == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	Rect	 rect	    = iRect;
	Int	 lineHeight = font.GetScaledTextSizeY() + 3;

	const Array<String>	&lines = string.Explode("\n");

	foreach (const String &line, lines)
	{
		Rect	 tRect = rightToLeft.TranslateRect(rect);

		tRect.left = rightToLeft.GetRightToLeft() ? tRect.right - font.GetScaledTextSizeX(line) : tRect.left;

		if (!painting)
		{
			BFont		 viewFont;
			font_height	 height;

			viewFont.SetFamilyAndStyle(font.GetName(), NULL);
			viewFont.SetFace((font.GetStyle() & Font::Italic    ? B_ITALIC_FACE     : 0) |
					 (font.GetStyle() & Font::Underline ? B_UNDERSCORE_FACE : 0) |
					 (font.GetStyle() & Font::StrikeOut ? B_STRIKEOUT_FACE  : 0) |
					 (font.GetWeight() >= Font::Bold    ? B_BOLD_FACE       : 0));
			viewFont.SetSize(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 96.0));

			view->SetFont(&viewFont);
			view->GetFontHeight(&height);

			view->SetDrawingMode(B_OP_OVER);
			view->SetHighColor(font.GetColor().GetRed(), font.GetColor().GetGreen(), font.GetColor().GetBlue());
			view->MovePenTo(BPoint(tRect.left, tRect.top + height.ascent * fontSize.TranslateY(96) / 72.0));
			view->DrawString(line.ConvertTo("UTF-8"));
			view->SetDrawingMode(B_OP_COPY);
		}

		BFont		 viewFont;
		font_height	 height;

		viewFont.SetFamilyAndStyle(font.GetName(), NULL);
		viewFont.SetFace((font.GetStyle() & Font::Italic    ? B_ITALIC_FACE     : 0) |
				 (font.GetStyle() & Font::Underline ? B_UNDERSCORE_FACE : 0) |
				 (font.GetStyle() & Font::StrikeOut ? B_STRIKEOUT_FACE  : 0) |
				 (font.GetWeight() >= Font::Bold    ? B_BOLD_FACE       : 0));
		viewFont.SetSize(Math::Round(font.GetSize() * fontSize.TranslateY(96) / 96.0));

		bitmapView->SetFont(&viewFont);
		bitmapView->GetFontHeight(&height);

		bitmapView->SetDrawingMode(B_OP_OVER);
		bitmapView->SetHighColor(font.GetColor().GetRed(), font.GetColor().GetGreen(), font.GetColor().GetBlue());
		bitmapView->MovePenTo(BPoint(tRect.left, tRect.top + height.ascent * fontSize.TranslateY(96) / 72.0));
		bitmapView->DrawString(line.ConvertTo("UTF-8"));
		bitmapView->SetDrawingMode(B_OP_COPY);

		rect.top += lineHeight;
	}

	String::ExplodeFinish();

	return Success();
}

S::Int S::GUI::SurfaceHaiku::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (view == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect) - Size(1, 1);

	BGradient	*gradient = NIL;

	switch (style)
	{
		case OR_HORZ:
			gradient = new BGradientLinear(0, 0, rect.GetWidth(), 0);

			if (rightToLeft.GetRightToLeft())
			{
				gradient->AddColorStop(BGradient::ColorStop(color1.GetRed(), color1.GetGreen(), color1.GetBlue(), 255, 255), 0);
				gradient->AddColorStop(BGradient::ColorStop(color2.GetRed(), color2.GetGreen(), color2.GetBlue(), 255, 0), 1);
			}
			else
			{
				gradient->AddColorStop(BGradient::ColorStop(color1.GetRed(), color1.GetGreen(), color1.GetBlue(), 255, 0), 0);
				gradient->AddColorStop(BGradient::ColorStop(color2.GetRed(), color2.GetGreen(), color2.GetBlue(), 255, 255), 1);
			}

			break;
		case OR_VERT:
			gradient = new BGradientLinear(0, 0, 0, rect.GetHeight());

			gradient->AddColorStop(BGradient::ColorStop(color1.GetRed(), color1.GetGreen(), color1.GetBlue(), 255, 0), 0);
			gradient->AddColorStop(BGradient::ColorStop(color2.GetRed(), color2.GetGreen(), color2.GetBlue(), 255, 255), 1);

			break;
	}

	if (!painting) view->FillRect(BRect(rect.left, rect.top, rect.right, rect.bottom), *gradient);
	bitmapView->FillRect(BRect(rect.left, rect.top, rect.right, rect.bottom), *gradient);

	delete gradient;

	return Success();
}

S::Int S::GUI::SurfaceHaiku::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (view == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	if (!painting)
	{
		view->DrawBitmapAsync((BBitmap *) bitmap.GetSystemBitmap(), BRect(srcRect.left, srcRect.top, srcRect.right, srcRect.bottom), BRect(destRect.left, destRect.top, destRect.right, destRect.bottom));
	}

	bitmapView->DrawBitmapAsync((BBitmap *) bitmap.GetSystemBitmap(), BRect(srcRect.left, srcRect.top, srcRect.right, srcRect.bottom), BRect(destRect.left, destRect.top, destRect.right, destRect.bottom));

	return Success();
}

S::Int S::GUI::SurfaceHaiku::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (view == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	BBitmap *destBitmap = (BBitmap *) bitmap.GetSystemBitmap();
	BView	*destView   = new BView(destBitmap->Bounds(), NULL, B_FOLLOW_ALL_SIDES, 0);

	destBitmap->AddChild(destView);
	destBitmap->Lock();

	bitmapView->Sync();

	destView->DrawBitmap(this->bitmap, BRect(srcRect.left, srcRect.top, srcRect.right, srcRect.bottom), BRect(destRect.left, destRect.top, destRect.right, destRect.bottom));

	destBitmap->Unlock();
	destBitmap->RemoveChild(destView);

	delete destView;

	return Success();
}
