 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <windows.h>
#include <gdiplus.h>

#include <smooth/graphics/backends/gdiplus/surfacegdiplus.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/misc/math.h>

#include <fribidi.h>

S::GUI::SurfaceBackend *CreateSurfaceGDIPlus(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceGDIPlus(iSurface, maxSize);
}

S::Int		 surfaceGDITmp = S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceGDIPlus);

S::Short	 S::GUI::SurfaceGDIPlus::surfaceDPI = -1;

S::GUI::SurfaceGDIPlus::SurfaceGDIPlus(Void *iWindow, const Size &maxSize)
{
	type = SURFACE_GDIPLUS;

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

		Gdiplus::Graphics	*graphics = new Gdiplus::Graphics(gdi_dc);

		paintBitmap	= new Gdiplus::Bitmap(size.cx, size.cy, graphics);
		paintContext	= new Gdiplus::Graphics(paintBitmap);

		paintRects.Add(new Rect(Point(0, 0), size));

		delete graphics;

		ReleaseDC(window, gdi_dc);

		allocSize = size;
	}
}

S::GUI::SurfaceGDIPlus::~SurfaceGDIPlus()
{
	if (window != NIL)
	{
		delete paintContext;
		delete paintBitmap;
		delete paintRects.GetFirst();
	}
}

S::Int S::GUI::SurfaceGDIPlus::SetSize(const Size &nSize)
{
	size = nSize;

	rightToLeft.SetSurfaceSize(size);

	if (allocSize.cx >= nSize.cx && allocSize.cy >= nSize.cy) return Success();

	if (window != NIL && !painting)
	{
		delete paintContext;
		delete paintBitmap;
		delete paintRects.GetFirst();

		paintRects.RemoveAll();

		HDC	 gdi_dc = GetWindowDC(window);

		Gdiplus::Graphics	*graphics = new Gdiplus::Graphics(gdi_dc);

		paintBitmap	= new Gdiplus::Bitmap(size.cx, size.cy, graphics);
		paintContext	= new Gdiplus::Graphics(paintBitmap);

		paintRects.Add(new Rect(Point(0, 0), size));

		delete graphics;

		ReleaseDC(window, gdi_dc);
	}

	allocSize = nSize;

	return Success();
}

const S::GUI::Size &S::GUI::SurfaceGDIPlus::GetSize() const
{
	return size;
}

S::Int S::GUI::SurfaceGDIPlus::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		HDC			 gdi_dc = GetWindowDC(window);
		Gdiplus::Graphics	*graphics = new Gdiplus::Graphics(gdi_dc);

		Gdiplus::RectF		 srcRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top);
		Gdiplus::RectF		 destRect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top);

		graphics->DrawImage(paintBitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, Gdiplus::UnitPixel);

		delete graphics;

		ReleaseDC(window, gdi_dc);
	}

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(fontSize.TranslateRect(iPRect)), *(paintRects.GetLast()));

	paintContext->SetClip(Gdiplus::Rect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top));

	paintRects.Add(new Rect(pRect));

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0) PaintRect(*(paintRects.GetLast()));

	delete paintRects.GetLast();

	paintRects.Remove(paintRects.GetNthIndex(paintRects.Length() - 1));

	Rect	 pRect(*paintRects.GetLast());

	paintContext->SetClip(Gdiplus::Rect(pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top));

	return Success();
}

S::Void *S::GUI::SurfaceGDIPlus::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceGDIPlus::GetSurfaceDPI() const
{
	if (surfaceDPI != -1) return surfaceDPI;

	HDC	 dc = GetWindowDC(0);
	Short	 dpi = GetDeviceCaps(dc, LOGPIXELSY);

	ReleaseDC(0, dc);

	surfaceDPI = dpi;

	return dpi;
}

S::Int S::GUI::SurfaceGDIPlus::SetPixel(const Point &point, const Color &color)
{
	if (window == NIL) return Success();

	if (!painting)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		::SetPixel(gdi_dc, rightToLeft.TranslateX(fontSize.TranslateX(point.x)), rightToLeft.TranslateY(fontSize.TranslateY(point.y)), color);

		ReleaseDC(window, gdi_dc);
	}

	HDC	 cdc = paintContext->GetHDC();

	::SetPixel(cdc, rightToLeft.TranslateX(fontSize.TranslateX(point.x)), rightToLeft.TranslateY(fontSize.TranslateY(point.y)), color);

	paintContext->ReleaseHDC(cdc);

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = iPos1;
	Point	 pos2 = iPos2;

	/* Adjust to Windows GDI behavior for diagonal lines.
	 */
	if (Math::Abs(pos2.x - pos1.x) == Math::Abs(pos2.y - pos1.y))
	{
		if (pos1.x < pos2.x && pos1.y < pos2.y) { pos2.x--; pos2.y--; }
		if (pos1.x < pos2.x && pos1.y > pos2.y) { pos2.x--; pos2.y++; }
	} 

	pos1 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(pos1));
	pos2 = rightToLeft.TranslatePoint(fontSize.TranslatePoint(pos2));

	Gdiplus::Pen	 pen(Gdiplus::Color(color.GetRed(), color.GetGreen(), color.GetBlue()));

	if (!painting)
	{
		HDC			 gdi_dc = GetWindowDC(window);
		Gdiplus::Graphics	*pGraphics = new Gdiplus::Graphics(gdi_dc);

		pGraphics->DrawLine(&pen, INT(pos1.x), INT(pos1.y), INT(pos2.x), INT(pos2.y));

		delete pGraphics;

		ReleaseDC(window, gdi_dc);
	}

	paintContext->DrawLine(&pen, INT(pos1.x), INT(pos1.y), INT(pos2.x), INT(pos2.y));

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(fontSize.TranslateRect(iRect));

	HDC	 gdi_dc = GetWindowDC(window);
	HBRUSH	 brush = CreateSolidBrush(color);
	HPEN	 pen = CreatePen(PS_SOLID, 0, color);
	RECT	 wRect = rect;

	Gdiplus::SolidBrush	 gdip_brush(Gdiplus::Color(color.GetRed(), color.GetGreen(), color.GetBlue()));
	Gdiplus::Pen		 gdip_pen(Gdiplus::Color(color.GetRed(), color.GetGreen(), color.GetBlue()));
	Gdiplus::Rect		 gdip_rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			HDC	 cdc		= paintContext->GetHDC();

			HBRUSH	 previousBrush	= (HBRUSH) SelectObject(cdc, brush);
			HPEN	 previousPen	= (HPEN) SelectObject(cdc, pen);

			if (!painting) RoundRect(gdi_dc, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);
			RoundRect(cdc, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);

			brush	= (HBRUSH) SelectObject(cdc, previousBrush);
			pen	= (HPEN) SelectObject(cdc, previousPen);

			paintContext->ReleaseHDC(cdc);
		}
		else
		{
			if (!painting) FillRect(gdi_dc, &wRect, brush);
			paintContext->FillRectangle(&gdip_brush, gdip_rect);
		}
	}
	else if (style == Rect::Outlined)
	{
		if (!painting) FrameRect(gdi_dc, &wRect, brush);
		paintContext->DrawRectangle(&gdip_pen, gdip_rect);
	}
	else if (style & Rect::Inverted)
	{
		HDC	 cdc = paintContext->GetHDC();

		if (!painting) InvertRect(gdi_dc, &wRect);
		InvertRect(cdc, &wRect);

		paintContext->ReleaseHDC(cdc);
	}
	else if (style & Rect::Dotted)
	{
		HDC	 cdc = paintContext->GetHDC();

		Bool	 dot = False;
		Int	 x;
		Int	 y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == True)
			{
				if (!painting) ::SetPixel(gdi_dc, x, y, color);
				::SetPixel(cdc, x, y, color);
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
				::SetPixel(cdc, x, y, color);
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
				::SetPixel(cdc, x, y, color);
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
				::SetPixel(cdc, x, y, color);
				dot = False;
			}
			else dot = True;
		}

		paintContext->ReleaseHDC(cdc);
	}

	::DeleteObject(brush);
	::DeleteObject(pen);

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::SetText(const String &string, const Rect &iRect, const Font &font, Bool shadow)
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

	HDC	 cdc = paintContext->GetHDC();

	if (Setup::enableUnicode)	hfont = CreateFontW(-Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0), 0, 0, 0, font.GetWeight(), font.GetStyle() & Font::Italic, font.GetStyle() & Font::Underline, font.GetStyle() & Font::StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());
	else				hfont = CreateFontA(-Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0), 0, 0, 0, font.GetWeight(), font.GetStyle() & Font::Italic, font.GetStyle() & Font::Underline, font.GetStyle() & Font::StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());

	if (!painting)
	{
		SetBkMode(gdi_dc, TRANSPARENT);
		SetTextColor(gdi_dc, font.GetColor());

		holdfont = (HFONT) SelectObject(gdi_dc, hfont);
	}

	SetBkMode(cdc, TRANSPARENT);
	SetTextColor(cdc, font.GetColor());

	holdfont2 = (HFONT) SelectObject(cdc, hfont);

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

		RECT	 Rect = rightToLeft.TranslateRect(fontSize.TranslateRect(rect));

		if (rightToLeft.GetRightToLeft()) Rect.right--;

		if (rtlCharacters && Setup::useIconv)
		{
			/*	Reorder the string with fribidi, then get
				the glyph indices using GetCharacterPlacement
				and display using the glyph indices.
				This does not work with Kanji.	*/

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
			resultsa.lpGlyphs = glyphs;
			resultsa.nGlyphs = line.Length() + 1;

			resultsw.lStructSize = sizeof(resultsw);
			resultsw.lpGlyphs = glyphs;
			resultsw.nGlyphs = line.Length() + 1;

			ZeroMemory(glyphs, 2 * (line.Length() + 1));

			if (!painting)
			{
				if (rightToLeft.GetRightToLeft()) SetTextAlign(gdi_dc, TA_RIGHT);
				else				  SetTextAlign(gdi_dc, TA_LEFT);
			}

			if (rightToLeft.GetRightToLeft()) SetTextAlign(cdc, TA_RIGHT);
			else				  SetTextAlign(cdc, TA_LEFT);

			if (Setup::enableUnicode) GetCharacterPlacementW(cdc, line, line.Length(), 0, &resultsw, 0);
			else			  GetCharacterPlacementA(cdc, line, line.Length(), 0, &resultsa, 0);

			if (rightToLeft.GetRightToLeft()) Rect.left -= 10;
			else				  Rect.right += 10;

			if (!painting)
			{
				if (Setup::enableUnicode) ExtTextOutW(gdi_dc, (rightToLeft.GetRightToLeft() ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
				else			  ExtTextOutA(gdi_dc, (rightToLeft.GetRightToLeft() ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);
			}

			if (Setup::enableUnicode) ExtTextOutW(cdc, (rightToLeft.GetRightToLeft() ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, resultsw.lpGlyphs, resultsw.nGlyphs, NIL);
			else			  ExtTextOutA(cdc, (rightToLeft.GetRightToLeft() ? Rect.right : Rect.left), Rect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &Rect, (char *) resultsa.lpGlyphs, resultsa.nGlyphs, NIL);

			delete [] glyphs;
		}
		else
		{
			/*	Let Windows do any reordering and ligating.
				Works with Kanji, but RTL is only supported
				on XP and later versions of Windows.	*/

			if (!painting)
			{
				SetTextAlign(gdi_dc, TA_LEFT);

				if (Setup::enableUnicode) DrawTextExW(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
				else			  DrawTextExA(gdi_dc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			}

			SetTextAlign(cdc, TA_LEFT);

			if (Setup::enableUnicode) DrawTextExW(cdc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
			else			  DrawTextExA(cdc, line, -1, &Rect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
		}

		rect.top += lineHeight;
	}

	if (!painting) SelectObject(gdi_dc, holdfont);
	SelectObject(cdc, holdfont2);

	::DeleteObject(hfont);

	paintContext->ReleaseHDC(cdc);

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::BlitFromBitmap(const Bitmap &oBitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window  == NIL) return Success();
	if (oBitmap == NIL) return Error();

	Gdiplus::Bitmap		 bitmap((HBITMAP) oBitmap.GetSystemBitmap(), NIL);
	Rect			 destRect = rightToLeft.TranslateRect(fontSize.TranslateRect(iDestRect));
	HDC			 gdi_dc = GetWindowDC(window);
	Gdiplus::Graphics	*screen = new Gdiplus::Graphics(gdi_dc);
	Gdiplus::CachedBitmap	 cachedBitmap(&bitmap, screen);

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		if (!painting)
		{
			screen->DrawCachedBitmap(&cachedBitmap, destRect.left, destRect.top);
		}

		paintContext->DrawCachedBitmap(&cachedBitmap, destRect.left, destRect.top);
	}
	else
	{
		if (!painting)
		{
			screen->DrawImage(&bitmap, INT(destRect.left), INT(destRect.top), INT(destRect.right - destRect.left), INT(destRect.bottom - destRect.top));
		}

		paintContext->DrawImage(&bitmap, INT(destRect.left), INT(destRect.top), INT(destRect.right - destRect.left), INT(destRect.bottom - destRect.top));
	}

	delete screen;

	ReleaseDC(window, gdi_dc);

	return Success();
}

S::Int S::GUI::SurfaceGDIPlus::BlitToBitmap(const Rect &iSrcRect, const Bitmap &oBitmap, const Rect &destRect)
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
