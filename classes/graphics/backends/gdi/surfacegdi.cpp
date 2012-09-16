 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/gdi/surfacegdi.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <fribidi.h>

S::GUI::SurfaceBackend *CreateSurfaceGDI(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceGDI(iSurface, maxSize);
}

S::Int		 surfaceGDITmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceGDI);

S::Short	 S::GUI::SurfaceGDI::surfaceDPI = -1;

S::GUI::SurfaceGDI::SurfaceGDI(Void *iWindow, const Size &maxSize)
{
	type = SURFACE_GDI;

	window = (HWND) iWindow;

	if (window != NIL)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		size = maxSize;

		if (maxSize == Size())
		{
			size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
			size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;
		}

		rightToLeft.SetSurfaceSize(size);

		paintContext = CreateCompatibleDC(gdi_dc);

		HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		paintBitmap = (HBITMAP) SelectObject(paintContext, bitmap);
		paintRects.Add(new Rect(Point(0, 0), size));

		ReleaseDC(window, gdi_dc);

		allocSize = size;
	}

	fontSize.SetFontSize(GetSurfaceDPI());
}

S::GUI::SurfaceGDI::~SurfaceGDI()
{
	if (window != NIL)
	{
		HBITMAP	 bitmap = (HBITMAP) SelectObject(paintContext, paintBitmap);

		DeleteDC(paintContext);
		::DeleteObject(bitmap);

		delete paintRects.GetFirst();
	}
}

S::Int S::GUI::SurfaceGDI::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		HBITMAP	 bitmap = (HBITMAP) SelectObject(paintContext, paintBitmap);

		::DeleteObject(bitmap);

		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		HDC	 gdi_dc = GetWindowDC(window);

		bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		paintBitmap = (HBITMAP) SelectObject(paintContext, bitmap);
		paintRects.Add(new Rect(Point(0, 0), size));

		ReleaseDC(window, gdi_dc);
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceGDI::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceGDI::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		BitBlt(gdi_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, paintContext, pRect.left, pRect.top, SRCCOPY);

		ReleaseDC(window, gdi_dc);
	}

	return Success();
}

S::Int S::GUI::SurfaceGDI::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), *(paintRects.GetLast()));
	HRGN	 region = CreateRectRgn(pRect.left, pRect.top, pRect.right, pRect.bottom);

	SelectClipRgn(paintContext, region);

	::DeleteObject(region);

	paintRects.Add(new Rect(pRect));

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceGDI::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0) PaintRect(*(paintRects.GetLast()));

	delete paintRects.GetLast();

	paintRects.RemoveNth(paintRects.Length() - 1);

	Rect	 pRect(*paintRects.GetLast());
	HRGN	 region = CreateRectRgn(pRect.left, pRect.top, pRect.right, pRect.bottom);

	SelectClipRgn(paintContext, region);

	::DeleteObject(region);

	return Success();
}

S::Void *S::GUI::SurfaceGDI::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceGDI::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	HDC	 dc  = GetWindowDC(0);
	Short	 dpi = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(0, dc);

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceGDI::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	if (!painting)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		::SetPixel(gdi_dc, point.x, point.y, color);

		ReleaseDC(window, gdi_dc);
	}

	::SetPixel(paintContext, point.x, point.y, color);

	return Success();
}

S::Int S::GUI::SurfaceGDI::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	HPEN	 hPen = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = NIL;

	if (!painting)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		hOldPen = (HPEN) SelectObject(gdi_dc, hPen);

		MoveToEx(gdi_dc, pos1.x, pos1.y, NIL);
		LineTo(gdi_dc, pos2.x, pos2.y);

		hPen = (HPEN) SelectObject(gdi_dc, hOldPen);

		ReleaseDC(window, gdi_dc);
	}

	hOldPen = (HPEN) SelectObject(paintContext, hPen);

	MoveToEx(paintContext, pos1.x, pos1.y, NIL);
	LineTo(paintContext, pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(paintContext, hOldPen);

	::DeleteObject(hPen);

	return Success();
}

S::Int S::GUI::SurfaceGDI::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	HDC	 gdi_dc = GetWindowDC(window);
	HBRUSH	 brush = CreateSolidBrush(color);
	HPEN	 pen = CreatePen(PS_SOLID, 0, color);
	RECT	 wRect = { rect.left, rect.top, rect.right, rect.bottom };

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			if (!painting)
			{
				HBRUSH	 previousBrush	= (HBRUSH) SelectObject(gdi_dc, brush);
				HPEN	 previousPen	= (HPEN) SelectObject(gdi_dc, pen);

				RoundRect(gdi_dc, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);

				brush	= (HBRUSH) SelectObject(gdi_dc, previousBrush);
				pen	= (HPEN) SelectObject(gdi_dc, previousPen);
			}

			HBRUSH	 previousBrush	= (HBRUSH) SelectObject(paintContext, brush);
			HPEN	 previousPen	= (HPEN) SelectObject(paintContext, pen);

			RoundRect(paintContext, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);

			brush	= (HBRUSH) SelectObject(paintContext, previousBrush);
			pen	= (HPEN) SelectObject(paintContext, previousPen);
		}
		else
		{
			if (!painting) FillRect(gdi_dc, &wRect, brush);
			FillRect(paintContext, &wRect, brush);
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting) FrameRect(gdi_dc, &wRect, brush);
		FrameRect(paintContext, &wRect, brush);
	}
	else if (style & Rect::Inverted)
	{
		if (!painting) InvertRect(gdi_dc, &wRect);
		InvertRect(paintContext, &wRect);
	}
	else if (style & Rect::Dotted)
	{
		if (!painting)
		{
			for (Int x = rect.left								 + 1; x <  rect.right;	 x += 2) ::SetPixel(gdi_dc, x, rect.top, color);
			for (Int y = rect.top	 - (rect.right - rect.left			   ) % 2 + 2; y <  rect.bottom;	 y += 2) ::SetPixel(gdi_dc, rect.right - 1, y, color);
			for (Int x = rect.right	 - (rect.right - rect.left + rect.bottom - rect.top) % 2 - 2; x >= rect.left;	 x -= 2) ::SetPixel(gdi_dc, x, rect.bottom - 1, color);
			for (Int y = rect.bottom - (			     rect.bottom - rect.top) % 2 - 1; y >= rect.top;	 y -= 2) ::SetPixel(gdi_dc, rect.left, y, color);
		}

		for (Int x = rect.left								 + 1;  x <  rect.right;	 x += 2) ::SetPixel(paintContext, x, rect.top, color);
		for (Int y = rect.top	 - (rect.right - rect.left			   ) % 2 + 2;  y <  rect.bottom; y += 2) ::SetPixel(paintContext, rect.right - 1, y, color);
		for (Int x = rect.right	 - (rect.right - rect.left + rect.bottom - rect.top) % 2 - 2;  x >= rect.left;	 x -= 2) ::SetPixel(paintContext, x, rect.bottom - 1, color);
		for (Int y = rect.bottom - (			     rect.bottom - rect.top) % 2 - 1;  y >= rect.top;	 y -= 2) ::SetPixel(paintContext, rect.left, y, color);
	}

	::DeleteObject(brush);
	::DeleteObject(pen);

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDI::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (window == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	Rect	 rect	    = iRect;
	Int	 lineHeight = font.GetScaledTextSizeY() + 3;

	HDC	 gdi_dc = GetWindowDC(window);
	HFONT	 hfont;
	HFONT	 holdfont = NIL;
	HFONT	 holdfont2 = NIL;

	if (Setup::enableUnicode)	hfont = CreateFontW(-Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0), 0, 0, 0, font.GetWeight(), font.GetStyle() & Font::Italic, font.GetStyle() & Font::Underline, font.GetStyle() & Font::StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());
	else				hfont = CreateFontA(-Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0), 0, 0, 0, font.GetWeight(), font.GetStyle() & Font::Italic, font.GetStyle() & Font::Underline, font.GetStyle() & Font::StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());

	if (!painting)
	{
		SetBkMode(gdi_dc, TRANSPARENT);
		SetTextColor(gdi_dc, font.GetColor());

		holdfont = (HFONT) SelectObject(gdi_dc, hfont);
	}

	SetBkMode(paintContext, TRANSPARENT);
	SetTextColor(paintContext, font.GetColor());

	holdfont2 = (HFONT) SelectObject(paintContext, hfont);

	const Array<String>	&lines = string.Explode("\n");

	foreach (const String &line, lines)
	{
		Bool	 rtlCharacters = False;

		for (Int i = 0; i < line.Length(); i++) if (line[i] >= 0x0590 && line[i] <= 0x07BF) { rtlCharacters = True; break; }

		Rect	 tRect = rightToLeft.TranslateRect(rect);
		RECT	 wRect = { tRect.left, tRect.top, tRect.right, tRect.bottom };

		if (rtlCharacters && Setup::useIconv)
		{
			String		 visualLine;

			/* Reorder the string with fribidi, then get
			 * the glyph indices using GetCharacterPlacement
			 * and display using the glyph indices.
			 * This does not work with Kanji.
			 */
			FriBidiChar	*visual = new FriBidiChar [line.Length() + 1];
			FriBidiParType	 type = (rightToLeft.GetRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

			fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), line.Length(), &type, visual, NIL, NIL, NIL);

			visual[line.Length()] = 0;

			visualLine.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;

			GCP_RESULTSA	 resultsa;
			GCP_RESULTSW	 resultsw;
			wchar_t		*glyphs = new wchar_t [visualLine.Length() + 1];

			ZeroMemory(&resultsa, sizeof(resultsa));
			ZeroMemory(&resultsw, sizeof(resultsw));

			resultsa.lStructSize = sizeof(resultsa);
			resultsa.lpGlyphs    = glyphs;
			resultsa.nGlyphs     = visualLine.Length() + 1;

			resultsw.lStructSize = sizeof(resultsw);
			resultsw.lpGlyphs    = glyphs;
			resultsw.nGlyphs     = visualLine.Length() + 1;

			ZeroMemory(glyphs, 2 * (visualLine.Length() + 1));

			if (!painting)
			{
				if (rightToLeft.GetRightToLeft()) SetTextAlign(gdi_dc, TA_RIGHT);
				else				  SetTextAlign(gdi_dc, TA_LEFT);
			}

			if (rightToLeft.GetRightToLeft()) SetTextAlign(paintContext, TA_RIGHT);
			else				  SetTextAlign(paintContext, TA_LEFT);

			if (Setup::enableUnicode) GetCharacterPlacementW(paintContext, visualLine, visualLine.Length(), 0, &resultsw, 0);
			else			  GetCharacterPlacementA(paintContext, visualLine, visualLine.Length(), 0, &resultsa, 0);

			if (rightToLeft.GetRightToLeft()) wRect.left -= 10;
			else				  wRect.right += 10;

			if (!painting)
			{
				if (Setup::enableUnicode) ExtTextOutW(gdi_dc, (rightToLeft.GetRightToLeft() ? wRect.right : wRect.left), wRect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &wRect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
				else			  ExtTextOutA(gdi_dc, (rightToLeft.GetRightToLeft() ? wRect.right : wRect.left), wRect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &wRect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);
			}

			if (Setup::enableUnicode) ExtTextOutW(paintContext, (rightToLeft.GetRightToLeft() ? wRect.right : wRect.left), wRect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &wRect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else			  ExtTextOutA(paintContext, (rightToLeft.GetRightToLeft() ? wRect.right : wRect.left), wRect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &wRect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			delete [] glyphs;
		}
		else
		{
			/* Let Windows do any reordering and ligating.
			 * Works with Kanji, but RTL is only supported
			 * on XP and later versions of Windows.
			 */
			if (!painting)
			{
				SetTextAlign(gdi_dc, TA_LEFT);

				if (Setup::enableUnicode) DrawTextExW(gdi_dc, line, -1, &wRect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
				else			  DrawTextExA(gdi_dc, line, -1, &wRect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			}

			SetTextAlign(paintContext, TA_LEFT);

			if (Setup::enableUnicode) DrawTextExW(paintContext, line, -1, &wRect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else			  DrawTextExA(paintContext, line, -1, &wRect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
		}

		rect.top += lineHeight;
	}

	String::ExplodeFinish();

	if (!painting) SelectObject(gdi_dc, holdfont);
	SelectObject(paintContext, holdfont2);

	::DeleteObject(hfont);

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);
	HDC	 gdi_dc	  = GetWindowDC(window);
	HDC	 cdc	  = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	  = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		if (!painting)
		{
			BitBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
		}

		BitBlt(paintContext, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		if (!painting)
		{
			SetStretchBltMode(gdi_dc, HALFTONE);
			StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
		}

		SetStretchBltMode(paintContext, HALFTONE);
		StretchBlt(paintContext, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);
	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);
	HDC	 cdc	 = CreateCompatibleDC(paintContext);
	HBITMAP	 backup	 = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, paintContext, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(cdc, HALFTONE);
		StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, paintContext, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success();
}
