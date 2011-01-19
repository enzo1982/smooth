 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(fontSize.TranslateRect(iPRect)), *(paintRects.GetLast()));
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

	paintRects.Remove(paintRects.GetNthIndex(paintRects.Length() - 1));

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

	HDC	 dc = GetWindowDC(0);
	Short	 dpi = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(0, dc);

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceGDI::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPoint));

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

	Point	 pos1 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos1));
	Point	 pos2 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(iPos2));

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

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	HDC	 gdi_dc = GetWindowDC(window);
	HBRUSH	 brush = CreateSolidBrush(color);
	HPEN	 pen = CreatePen(PS_SOLID, 0, color);
	RECT	 wRect = { rect.left, rect.top, rect.right, rect.bottom };

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			HBRUSH	 previousBrush	= (HBRUSH) SelectObject(paintContext, brush);
			HPEN	 previousPen	= (HPEN) SelectObject(paintContext, pen);

			if (!painting) RoundRect(gdi_dc, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);
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
		Bool	 dot = False;
		Int	 x;
		Int	 y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == True)
			{
				if (!painting) ::SetPixel(gdi_dc, x, y, color);
				::SetPixel(paintContext, x, y, color);
				dot = False;
			}
			else dot = True;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == True)
			{
				if (!painting) ::SetPixel(gdi_dc, x, y, color);
				::SetPixel(paintContext, x, y, color);
				dot = False;
			}
			else dot = True;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == True)
			{
				if (!painting) ::SetPixel(gdi_dc, x, y, color);
				::SetPixel(paintContext, x, y, color);
				dot = False;
			}
			else dot = True;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == True)
			{
				if (!painting) ::SetPixel(gdi_dc, x, y, color);
				::SetPixel(paintContext, x, y, color);
				dot = False;
			}
			else dot = True;
		}
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

	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 txtsize = string.Length();
	String	 line;
	Rect	 rect = iRect;
	Int	 lineHeight = font.GetTextSizeY() + 3;

	for (Int j = 0; j < txtsize; j++) if (string[j] == 10) lines++;

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

	for (Int i = 0; i < lines; i++)
	{
		Bool	 rtlCharacters = False;

		origoffset = offset;

		for (Int j = 0; j <= txtsize; j++)
		{
			if (j + origoffset == txtsize)
			{
				line[j] = 0;
				break;
			}

			if (string[j + origoffset] == 10 || string[j + origoffset] == 0)
			{
				offset++;
				line[j] = 0;
				break;
			}
			else
			{
				offset++;
				line[j] = string[j + origoffset];

				if (line[j] >= 0x0590 && line[j] <= 0x07BF) rtlCharacters = True;
			}
		}

		Rect	 tRect = rightToLeft.TranslateRect(fontSize.TranslateRect(rect));
		RECT	 wRect = { tRect.left, tRect.top, tRect.right, tRect.bottom };

		if (rtlCharacters && Setup::useIconv)
		{
			/* Reorder the string with fribidi, then get
			 * the glyph indices using GetCharacterPlacement
			 * and display using the glyph indices.
			 * This does not work with Kanji.
			 */
			FriBidiChar	*visual = new FriBidiChar [line.Length() + 1];
			FriBidiParType	 type = (rightToLeft.GetRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

			fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), line.Length(), &type, visual, NIL, NIL, NIL);

			visual[line.Length()] = 0;

			line.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;

			GCP_RESULTSA	 resultsa;
			GCP_RESULTSW	 resultsw;
			wchar_t		*glyphs = new wchar_t [line.Length() + 1];

			ZeroMemory(&resultsa, sizeof(resultsa));
			ZeroMemory(&resultsw, sizeof(resultsw));

			resultsa.lStructSize = sizeof(resultsa);
			resultsa.lpGlyphs    = glyphs;
			resultsa.nGlyphs     = line.Length() + 1;

			resultsw.lStructSize = sizeof(resultsw);
			resultsw.lpGlyphs    = glyphs;
			resultsw.nGlyphs     = line.Length() + 1;

			ZeroMemory(glyphs, 2 * (line.Length() + 1));

			if (!painting)
			{
				if (rightToLeft.GetRightToLeft()) SetTextAlign(gdi_dc, TA_RIGHT);
				else				  SetTextAlign(gdi_dc, TA_LEFT);
			}

			if (rightToLeft.GetRightToLeft()) SetTextAlign(paintContext, TA_RIGHT);
			else				  SetTextAlign(paintContext, TA_LEFT);

			if (Setup::enableUnicode) GetCharacterPlacementW(paintContext, line, line.Length(), 0, &resultsw, 0);
			else			  GetCharacterPlacementA(paintContext, line, line.Length(), 0, &resultsa, 0);

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

	if (!painting) SelectObject(gdi_dc, holdfont);
	SelectObject(paintContext, holdfont2);

	::DeleteObject(hfont);

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window  == NIL) return Success();
	if (oBitmap == NIL) return Error();

	Bitmap	 bitmap	  = oBitmap;
	Rect	 destRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iDestRect));
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

S::Int S::GUI::SurfaceGDI::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
{
	if (window  == NIL) return Success();
	if (oBitmap == NIL) return Error();

	Bitmap	 bitmap	 = oBitmap;
	Rect	 srcRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iSrcRect));
	HDC	 gdi_dc	 = GetWindowDC(window);
	HDC	 cdc	 = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	 = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(cdc, HALFTONE);
		StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);
	ReleaseDC(window, gdi_dc);

	return Success();
}
