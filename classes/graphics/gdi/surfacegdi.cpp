 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/gdi/surfacegdi.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/color.h>
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

	bmp_dc = NIL;
	cDc_bitmap = NIL;

	if (gdi_dc != NIL)
	{
		size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
		size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;

		bmp_dc = CreateCompatibleDC(gdi_dc);

		HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);
		Bitmap	 bmpGDI(bitmap);

		BlitToBitmap(Rect(Point(0, 0), size), bmpGDI, Rect(Point(0, 0), size));

		cDc_bitmap = (HBITMAP) SelectObject(bmp_dc, bitmap);
	}
}

S::GUI::SurfaceGDI::~SurfaceGDI()
{
	if (bmp_dc != NIL && cDc_bitmap != NIL)
	{
		HBITMAP	 bitmap = (HBITMAP) SelectObject(bmp_dc, cDc_bitmap);

		DeleteDC(bmp_dc);
		::DeleteObject(bitmap);
	}
}

S::Int S::GUI::SurfaceGDI::PaintRect(Rect pRect)
{
	if (painting) return Failure;

	if (gdi_dc != NIL) BitBlt(gdi_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, bmp_dc, pRect.left, pRect.top, SRCCOPY);

	return Success;
}

S::Int S::GUI::SurfaceGDI::StartPaint(Rect pRect)
{
	if (paintRect.left == -1 && paintRect.top == -1 && paintRect.right == -1 && paintRect.bottom == -1)
	{
		paintRect = pRect;
	}
	else
	{
		paintRect.left = (Int) Math::Min(paintRect.left, pRect.left);
		paintRect.top = (Int) Math::Min(paintRect.top, pRect.top);
		paintRect.right = (Int) Math::Max(paintRect.right, pRect.right);
		paintRect.bottom = (Int) Math::Max(paintRect.bottom, pRect.bottom);
	}

	if (!painting)
	{
		if (real_dc != NIL) gdi_dc = CreateCompatibleDC(real_dc);
	}

	painting++;

	return Success;
}

S::Int S::GUI::SurfaceGDI::EndPaint()
{
	if (!painting) return Failure;

	painting--;

	if (painting == 0)
	{
		if (gdi_dc != NIL) DeleteDC(gdi_dc);

		gdi_dc = real_dc;

		PaintRect(paintRect);

		paintRect.left = -1;
		paintRect.top = -1;
		paintRect.right = -1;
		paintRect.bottom = -1;
	}

	return Success;
}

S::Void *S::GUI::SurfaceGDI::GetSystemSurface()
{
	if (painting)	return bmp_dc;
	else		return gdi_dc;
}

S::Int S::GUI::SurfaceGDI::SetPixel(Int x, Int y, Int color)
{
	if (gdi_dc == NIL) return Success;

	::SetPixel(gdi_dc, TranslateX(x), TranslateY(y), color);
	::SetPixel(bmp_dc, TranslateX(x), TranslateY(y), color);

	return Success;
}

S::Int S::GUI::SurfaceGDI::GetPixel(Int x, Int y)
{
	if (gdi_dc == NIL) return 0;

	return ::GetPixel(gdi_dc, TranslateX(x), TranslateY(y));
}

S::Int S::GUI::SurfaceGDI::Line(Point pos1, Point pos2, Int color)
{
	if (gdi_dc == NIL) return Success;

	HPEN	 hPen = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = (HPEN) SelectObject(gdi_dc, hPen);

	pos1 = TranslatePoint(pos1);
	pos2 = TranslatePoint(pos2);

	MoveToEx(gdi_dc, pos1.x, pos1.y, NIL);
	LineTo(gdi_dc, pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(gdi_dc, hOldPen);

	hOldPen = (HPEN) SelectObject(bmp_dc, hPen);

	MoveToEx(bmp_dc, pos1.x, pos1.y, NIL);
	LineTo(bmp_dc, pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(bmp_dc, hOldPen);

	::DeleteObject(hPen);

	return Success;
}

S::Int S::GUI::SurfaceGDI::Box(Rect rect, Int color, Int style)
{
	if (gdi_dc == NIL) return Success;

	rect = TranslateRect(rect);

	HBRUSH	 brush = CreateSolidBrush(color);
	RECT	 wRect = rect;

	if (style == FILLED)
	{
		FillRect(gdi_dc, &wRect, brush);
		FillRect(bmp_dc, &wRect, brush);
	}
	else if (style == OUTLINED)
	{
		FrameRect(gdi_dc, &wRect, brush);
		FrameRect(bmp_dc, &wRect, brush);
	}
	else if (style == INVERT)
	{
		InvertRect(gdi_dc, &wRect);
		InvertRect(bmp_dc, &wRect);
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
				::SetPixel(bmp_dc, x, y, color);
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
				::SetPixel(bmp_dc, x, y, color);
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
				::SetPixel(bmp_dc, x, y, color);
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
				::SetPixel(bmp_dc, x, y, color);
				dot = False;
			}
			else dot = True;
		}
	}

	::DeleteObject(brush);

	return Success;
}

S::Int S::GUI::SurfaceGDI::SetText(String string, Rect rect, Font font, Bool shadow)
{
	if (gdi_dc == NIL)	return Success;

	if (string == NIL)	return Failure;
	if (shadow)		return SurfaceBackend::SetText(string, rect, font, shadow);

	HFONT	 hfont;
	HFONT	 holdfont;
	HFONT	 holdfont2;
	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 height = font.GetLineSizeY(string) + 3;
	int	 txtsize = string.Length();
	String	 line;

	for (Int j = 0; j < txtsize; j++) if (string[j] == 10) lines++;

	if (Setup::enableUnicode)	hfont = CreateFontW(-MulDiv(font.GetSize(), GetDeviceCaps(gdi_dc, LOGPIXELSY), 72), 0, 0, 0, font.GetWeight(), font.GetItalic(), font.GetUnderline(), font.GetStrikeOut(), ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());
	else				hfont = CreateFontA(-MulDiv(font.GetSize(), GetDeviceCaps(gdi_dc, LOGPIXELSY), 72), 0, 0, 0, font.GetWeight(), font.GetItalic(), font.GetUnderline(), font.GetStrikeOut(), ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());

	SetBkMode(gdi_dc, TRANSPARENT);
	SetBkMode(bmp_dc, TRANSPARENT);

	SetTextColor(gdi_dc, font.GetColor());
	SetTextColor(bmp_dc, font.GetColor());

	holdfont = (HFONT) SelectObject(gdi_dc, hfont);
	holdfont2 = (HFONT) SelectObject(bmp_dc, hfont);

	for (Int i = 0; i < lines; i++)
	{
		Bool	 rtlCharacters = False;

		origoffset = offset;

		for (int j = 0; j <= txtsize; j++)
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

		if (Setup::rightToLeft) Rect.right--;

		if (rtlCharacters && Setup::useIconv)
		{
			/*	Reorder the string with fribidi, then
				ligate using GetCharacterPlacement and
				display using the glyph indices.
				This does not work with Kanji.	*/

			unsigned long	*visual = new unsigned long [line.Length() + 1];
			long		 type = FRIBIDI_TYPE_ON;

			fribidi_log2vis((unsigned long *) line.ConvertTo("UCS-4LE"), line.Length(), &type, visual, NIL, NIL, NIL);

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

			if (Setup::rightToLeft)	SetTextAlign(gdi_dc, TA_RIGHT);
			else			SetTextAlign(gdi_dc, TA_LEFT);

			if (Setup::rightToLeft)	SetTextAlign(bmp_dc, TA_RIGHT);
			else			SetTextAlign(bmp_dc, TA_LEFT);

			if (Setup::enableUnicode)	GetCharacterPlacementW(gdi_dc, line, line.Length(), 0, &resultsw, GCP_GLYPHSHAPE | GCP_LIGATE);
			else				GetCharacterPlacementA(gdi_dc, line, line.Length(), 0, &resultsa, GCP_GLYPHSHAPE | GCP_LIGATE);

			if (Setup::enableUnicode)	ExtTextOutW(gdi_dc, (Setup::rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else				ExtTextOutA(gdi_dc, (Setup::rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			if (Setup::enableUnicode)	ExtTextOutW(bmp_dc, (Setup::rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else				ExtTextOutA(bmp_dc, (Setup::rightToLeft ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			delete [] glyphs;
		}
		else
		{
			/*	Let Windows do any reordering and ligating.
				Works with Kanji, but RTL is only supported
				on XP and later versions of Windows.	*/

			SetTextAlign(gdi_dc, TA_LEFT);
			SetTextAlign(bmp_dc, TA_LEFT);

			if (Setup::enableUnicode)	DrawTextExW(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (Setup::rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else				DrawTextExA(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (Setup::rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);

			if (Setup::enableUnicode)	DrawTextExW(bmp_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (Setup::rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else				DrawTextExA(bmp_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (Setup::rightToLeft ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
		}

		rect.top += height;
	}

	SelectObject(gdi_dc, holdfont);
	SelectObject(bmp_dc, holdfont2);

	::DeleteObject(hfont);

	return Success;
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(const Bitmap &oBitmap, Rect srcRect, Rect destRect)
{
	if (gdi_dc == NIL) return Success;

	Bitmap	 bitmap = oBitmap;

	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	destRect = TranslateRect(destRect);

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
		BitBlt(bmp_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
		StretchBlt(bmp_dc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success;
}

S::Int S::GUI::SurfaceGDI::BlitToBitmap(Rect srcRect, const Bitmap &oBitmap, Rect destRect)
{
	if (gdi_dc == NIL) return Success;

	Bitmap	 bitmap = oBitmap;

	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	srcRect = TranslateRect(srcRect);

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

	return Success;
}
