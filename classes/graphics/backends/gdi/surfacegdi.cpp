 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::SurfaceBackend *CreateSurfaceGDI(S::Void *iSurface)
{
	return new S::GUI::SurfaceGDI(iSurface);
}

S::Int	 surfaceGDITmp = S::GUI::SurfaceBackend::AddBackend(&CreateSurfaceGDI);

S::GUI::SurfaceGDI::SurfaceGDI(Void *iDc)
{
	type = SURFACE_GDI;

	gdi_dc = (HDC) iDc;
	real_dc = (HDC) iDc;

	if (gdi_dc != NIL)
	{
		size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
		size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;

		HDC	 bmp_dc = CreateCompatibleDC(gdi_dc);

		HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);
		Bitmap	 bmpGDI(bitmap);

		BlitToBitmap(Rect(Point(0, 0), size), bmpGDI, Rect(Point(0, 0), size));

		cDc_contexts.AddEntry(bmp_dc);
		cDc_bitmaps.AddEntry((HBITMAP) SelectObject(bmp_dc, bitmap));
		cDc_rects.AddEntry(new Rect(Point(0, 0), size));
	}
}

S::GUI::SurfaceGDI::~SurfaceGDI()
{
	if (gdi_dc != NIL)
	{
		HBITMAP	 bitmap = (HBITMAP) SelectObject(cDc_contexts.GetFirstEntry(), cDc_bitmaps.GetFirstEntry());

		DeleteDC(cDc_contexts.GetFirstEntry());
		::DeleteObject(bitmap);

		delete cDc_rects.GetFirstEntry();
	}
}

S::Int S::GUI::SurfaceGDI::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (gdi_dc != NIL)
	{
		BitBlt(gdi_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetFirstEntry(), pRect.left, pRect.top, SRCCOPY);
	}

	return Success();
}

S::Int S::GUI::SurfaceGDI::StartPaint(const Rect &iPRect)
{
	if (gdi_dc == NIL) return Success();

	Rect	 pRect = TranslateRect(iPRect);
	Rect	 oRect = *(cDc_rects.GetLastEntry());

	HDC	 bmp_dc = CreateCompatibleDC(cDc_contexts.GetFirstEntry());
	HBITMAP	 bitmap = CreateCompatibleBitmap(cDc_contexts.GetFirstEntry(), size.cx, size.cy);

	cDc_bitmaps.AddEntry((HBITMAP) SelectObject(bmp_dc, bitmap));

	if (pRect.left >= oRect.left && pRect.top >= oRect.top && pRect.right <= oRect.right && pRect.bottom <= oRect.bottom)	BitBlt(bmp_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetLastEntry(), pRect.left, pRect.top, SRCCOPY);
	else															BitBlt(bmp_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, cDc_contexts.GetFirstEntry(), pRect.left, pRect.top, SRCCOPY);

	cDc_contexts.AddEntry(bmp_dc);
	cDc_rects.AddEntry(new Rect(pRect));

	if (!painting)
	{
		if (real_dc != NIL) gdi_dc = CreateCompatibleDC(real_dc);
	}

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceGDI::EndPaint()
{
	if (!painting) return Error();

	painting--;

	Rect	 iRect = Rect::OverlapRect(*(cDc_rects.GetLastEntry()), *(cDc_rects.GetNthEntry(cDc_rects.GetNOfEntries() - 2)));

	BitBlt(cDc_contexts.GetNthEntry(cDc_contexts.GetNOfEntries() - 2), iRect.left, iRect.top, iRect.right - iRect.left, iRect.bottom - iRect.top, cDc_contexts.GetLastEntry(), iRect.left, iRect.top, SRCCOPY);

	if (painting == 0)
	{
		if (gdi_dc != NIL) DeleteDC(gdi_dc);

		gdi_dc = real_dc;

		PaintRect(iRect);
	}

	HBITMAP	 bitmap = (HBITMAP) SelectObject(cDc_contexts.GetLastEntry(), cDc_bitmaps.GetLastEntry());

	DeleteDC(cDc_contexts.GetLastEntry());
	::DeleteObject(bitmap);

	delete cDc_rects.GetLastEntry();

	cDc_contexts.RemoveEntry(cDc_contexts.GetNthEntryIndex(cDc_contexts.GetNOfEntries() - 1));
	cDc_bitmaps.RemoveEntry(cDc_bitmaps.GetNthEntryIndex(cDc_bitmaps.GetNOfEntries() - 1));
	cDc_rects.RemoveEntry(cDc_rects.GetNthEntryIndex(cDc_rects.GetNOfEntries() - 1));

	return Success();
}

S::Void *S::GUI::SurfaceGDI::GetSystemSurface()
{
	if (painting)	return cDc_contexts.GetLastEntry();
	else		return gdi_dc;
}

S::Int S::GUI::SurfaceGDI::SetPixel(Int x, Int y, Int color)
{
	if (gdi_dc == NIL) return Success();

	::SetPixel(gdi_dc, TranslateX(x), TranslateY(y), color);
	::SetPixel(cDc_contexts.GetLastEntry(), TranslateX(x), TranslateY(y), color);

	return Success();
}

S::Int S::GUI::SurfaceGDI::GetPixel(Int x, Int y)
{
	if (gdi_dc == NIL) return 0;

	return ::GetPixel(gdi_dc, TranslateX(x), TranslateY(y));
}

S::Int S::GUI::SurfaceGDI::Line(const Point &iPos1, const Point &iPos2, Int color)
{
	if (gdi_dc == NIL) return Success();

	HPEN	 hPen = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = (HPEN) SelectObject(gdi_dc, hPen);

	Point	 pos1 = TranslatePoint(iPos1);
	Point	 pos2 = TranslatePoint(iPos2);

	MoveToEx(gdi_dc, pos1.x, pos1.y, NIL);
	LineTo(gdi_dc, pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(gdi_dc, hOldPen);

	hOldPen = (HPEN) SelectObject(cDc_contexts.GetLastEntry(), hPen);

	MoveToEx(cDc_contexts.GetLastEntry(), pos1.x, pos1.y, NIL);
	LineTo(cDc_contexts.GetLastEntry(), pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(cDc_contexts.GetLastEntry(), hOldPen);

	::DeleteObject(hPen);

	return Success();
}

S::Int S::GUI::SurfaceGDI::Box(const Rect &iRect, Int color, Int style)
{
	if (gdi_dc == NIL) return Success();

	Rect	 rect = TranslateRect(iRect);

	HBRUSH	 brush = CreateSolidBrush(color);
	RECT	 wRect = rect;

	if (style == FILLED)
	{
		FillRect(gdi_dc, &wRect, brush);
		FillRect(cDc_contexts.GetLastEntry(), &wRect, brush);
	}
	else if (style == OUTLINED)
	{
		FrameRect(gdi_dc, &wRect, brush);
		FrameRect(cDc_contexts.GetLastEntry(), &wRect, brush);
	}
	else if (style == INVERT)
	{
		InvertRect(gdi_dc, &wRect);
		InvertRect(cDc_contexts.GetLastEntry(), &wRect);
	}
	else if (style == OUTLINEDOTS)
	{
		Bool	 dot = False;
		Int	 x;
		Int	 y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == True)
			{
				::SetPixel(gdi_dc, x, y, color);
				::SetPixel(cDc_contexts.GetLastEntry(), x, y, color);
				dot = False;
			}
			else dot = True;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == True)
			{
				::SetPixel(gdi_dc, x, y, color);
				::SetPixel(cDc_contexts.GetLastEntry(), x, y, color);
				dot = False;
			}
			else dot = True;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == True)
			{
				::SetPixel(gdi_dc, x, y, color);
				::SetPixel(cDc_contexts.GetLastEntry(), x, y, color);
				dot = False;
			}
			else dot = True;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == True)
			{
				::SetPixel(gdi_dc, x, y, color);
				::SetPixel(cDc_contexts.GetLastEntry(), x, y, color);
				dot = False;
			}
			else dot = True;
		}
	}

	::DeleteObject(brush);

	return Success();
}

S::Int S::GUI::SurfaceGDI::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
{
	if (gdi_dc == NIL)	return Success();

	if (string == NIL)	return Error();
	if (shadow)		return SurfaceBackend::SetText(string, iRect, font, shadow);

	HFONT	 hfont;
	HFONT	 holdfont;
	HFONT	 holdfont2;
	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 height = font.GetLineSizeY(string) + 3;
	int	 txtsize = string.Length();
	String	 line;
	Rect	 rect = iRect;

	for (Int j = 0; j < txtsize; j++) if (string[j] == 10) lines++;

	if (Setup::enableUnicode)	hfont = CreateFontW(-MulDiv(font.GetSize(), GetDeviceCaps(gdi_dc, LOGPIXELSY), 72), 0, 0, 0, font.GetWeight(), font.GetItalic(), font.GetUnderline(), font.GetStrikeOut(), ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());
	else				hfont = CreateFontA(-MulDiv(font.GetSize(), GetDeviceCaps(gdi_dc, LOGPIXELSY), 72), 0, 0, 0, font.GetWeight(), font.GetItalic(), font.GetUnderline(), font.GetStrikeOut(), ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());

	SetBkMode(gdi_dc, TRANSPARENT);
	SetBkMode(cDc_contexts.GetLastEntry(), TRANSPARENT);

	SetTextColor(gdi_dc, font.GetColor());
	SetTextColor(cDc_contexts.GetLastEntry(), font.GetColor());

	holdfont = (HFONT) SelectObject(gdi_dc, hfont);
	holdfont2 = (HFONT) SelectObject(cDc_contexts.GetLastEntry(), hfont);

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

		RECT	 Rect = TranslateRect(rect);

		if (rightToLeft) Rect.right--;

		if (rtlCharacters && Setup::useIconv)
		{
			/*	Reorder the string with fribidi, then get
				the glyph indices using GetCharacterPlacement
				and display using the glyph indices.
				This does not work with Kanji.	*/

			FriBidiParType	*visual = new FriBidiParType [line.Length() + 1];
			FriBidiParType	 type = FRIBIDI_TYPE_ON;

			fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), line.Length(), &type, (FriBidiChar *) visual, NIL, NIL, NIL);

			line.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;

			GCP_RESULTSA	 resultsa;
			GCP_RESULTSW	 resultsw;
			wchar_t		*glyphs = new wchar_t [line.Length() + 1];

			ZeroMemory(&resultsa, sizeof(resultsa));
			ZeroMemory(&resultsw, sizeof(resultsw));

			resultsa.lStructSize = sizeof(resultsa);
			resultsa.lpGlyphs = glyphs;
			resultsa.nGlyphs = line.Length() + 1;

			resultsw.lStructSize = sizeof(resultsw);
			resultsw.lpGlyphs = glyphs;
			resultsw.nGlyphs = line.Length() + 1;

			ZeroMemory(glyphs, 2 * (line.Length() + 1));

			if (rightToLeft)	SetTextAlign(gdi_dc, TA_RIGHT);
			else			SetTextAlign(gdi_dc, TA_LEFT);

			if (rightToLeft)	SetTextAlign(cDc_contexts.GetLastEntry(), TA_RIGHT);
			else			SetTextAlign(cDc_contexts.GetLastEntry(), TA_LEFT);

			if (Setup::enableUnicode)	GetCharacterPlacementW(gdi_dc, line, line.Length(), 0, &resultsw, 0);
			else				GetCharacterPlacementA(gdi_dc, line, line.Length(), 0, &resultsa, 0);

			if (rightToLeft) Rect.left -= 10;
			else			Rect.right += 10;

			if (Setup::enableUnicode)	ExtTextOutW(gdi_dc, (rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else				ExtTextOutA(gdi_dc, (rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			if (Setup::enableUnicode)	ExtTextOutW(cDc_contexts.GetLastEntry(), (rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else				ExtTextOutA(cDc_contexts.GetLastEntry(), (rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			delete [] glyphs;
		}
		else
		{
			/*	Let Windows do any reordering and ligating.
				Works with Kanji, but RTL is only supported
				on XP and later versions of Windows.	*/

			SetTextAlign(gdi_dc, TA_LEFT);
			SetTextAlign(cDc_contexts.GetLastEntry(), TA_LEFT);

			if (Setup::enableUnicode)	DrawTextExW(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else				DrawTextExA(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);

			if (Setup::enableUnicode)	DrawTextExW(cDc_contexts.GetLastEntry(), line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else				DrawTextExA(cDc_contexts.GetLastEntry(), line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
		}

		rect.top += height;
	}

	SelectObject(gdi_dc, holdfont);
	SelectObject(cDc_contexts.GetLastEntry(), holdfont2);

	::DeleteObject(hfont);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (gdi_dc == NIL) return Success();

	Bitmap	 bitmap	  = oBitmap;
	Rect	 destRect = TranslateRect(iDestRect);
	HDC	 cdc	  = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	  = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
		BitBlt(cDc_contexts.GetLastEntry(), destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
		StretchBlt(cDc_contexts.GetLastEntry(), destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
{
	if (gdi_dc == NIL) return Success();

	Bitmap	 bitmap	 = oBitmap;
	Rect	 srcRect = TranslateRect(iSrcRect);
	HDC	 cdc	 = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup	 = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success();
}
