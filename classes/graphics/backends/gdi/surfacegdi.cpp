 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/backends/platform.h>
#include <smooth/graphics/backends/gdi/surfacegdi.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/color.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#include <fribidi/fribidi.h>

S::GUI::SurfaceBackend *CreateSurfaceGDI(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceGDI(iSurface, maxSize);
}

static S::Bool ProbeGDI()
{
	return S::True;
}

static S::Int Register_surfaceGDITmp() { return S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceGDI); }

S::Int		 surfaceGDITmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_GDI, &Register_surfaceGDITmp);
S::Int		 surfaceGDIProbeTmp = S::Backends::Platform::AddProbe(S::Backends::PLATFORM_GDI, 2, &ProbeGDI);

S::Short	 S::GUI::SurfaceGDI::surfaceDPI = -1;

S::GUI::SurfaceGDI::SurfaceGDI(Void *iWindow, const Size &maxSize)
{
	type	     = SURFACE_GDI;

	window	     = (HWND) iWindow;

	paintBitmap  = NIL;
	paintContext = NIL;

	if (window != NIL)
	{
		HDC	 gdi_dc = GetWindowDC(window);

		size = maxSize;

		if (maxSize == Size())
		{
			size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
			size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;
		}

		fontSize.SetFontSize(GetSurfaceDPI());
		rightToLeft.SetSurfaceSize(size);

		paintContext = CreateCompatibleDC(gdi_dc);

		HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		paintBitmap = (HBITMAP) SelectObject(paintContext, bitmap);
		paintRects.Add(Rect(Point(0, 0), size));

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

		paintRects.RemoveAll();

		HDC	 gdi_dc = GetWindowDC(window);

		bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

		paintBitmap = (HBITMAP) SelectObject(paintContext, bitmap);
		paintRects.Add(Rect(Point(0, 0), size));

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

		BitBlt(gdi_dc, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight(), paintContext, pRect.left, pRect.top, SRCCOPY);

		ReleaseDC(window, gdi_dc);
	}

	return Success();
}

S::Int S::GUI::SurfaceGDI::StartPaint(const Rect &iPRect)
{
	if (window == NIL) return Success();

	Rect	 pRect = Rect::OverlapRect(rightToLeft.TranslateRect(iPRect), paintRects.GetLast());
	HRGN	 region = CreateRectRgn(pRect.left, pRect.top, pRect.right, pRect.bottom);

	SaveDC(paintContext);
	SelectClipRgn(paintContext, region);

	::DeleteObject(region);

	paintRects.Add(pRect);

	painting++;

	return Success();
}

S::Int S::GUI::SurfaceGDI::EndPaint()
{
	if (!painting) return Error();

	painting--;

	if (painting == 0) PaintRect(paintRects.GetLast());

	paintRects.RemoveNth(paintRects.Length() - 1);

	RestoreDC(paintContext, -1);

	return Success();
}

S::Void *S::GUI::SurfaceGDI::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceGDI::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	Float	 dpi = 96.0;

	surfaceDPI = Math::Round(dpi * Setup::FontSize);

	return surfaceDPI;
}

S::Int S::GUI::SurfaceGDI::SetPixel(const Point &iPoint, const Color &color)
{
	if (window == NIL) return Success();

	Point	 point = rightToLeft.TranslatePoint(iPoint);

	::SetPixel(paintContext, point.x, point.y, color);

	if (!painting) PaintRect(Rect(point, Size(1, 1)));

	return Success();
}

S::Int S::GUI::SurfaceGDI::Line(const Point &iPos1, const Point &iPos2, const Color &color)
{
	if (window == NIL) return Success();

	Point	 pos1 = rightToLeft.TranslatePoint(iPos1);
	Point	 pos2 = rightToLeft.TranslatePoint(iPos2);

	HPEN	 hPen	 = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = (HPEN) SelectObject(paintContext, hPen);

	MoveToEx(paintContext, pos1.x, pos1.y, NIL);
	LineTo(paintContext, pos2.x, pos2.y);

	SelectObject(paintContext, hOldPen);

	::DeleteObject(hPen);

	if (!painting) PaintRect(Rect(Point(Math::Min(pos1.x, pos2.x), Math::Min(pos1.y, pos2.y)), Size(Math::Abs(pos2.x - pos1.x) + 1, Math::Abs(pos2.y - pos1.y) + 1)));

	return Success();
}

S::Int S::GUI::SurfaceGDI::Box(const Rect &iRect, const Color &color, Int style, const Size &ellipse)
{
	if (window == NIL) return Success();

	Rect	 rect	= rightToLeft.TranslateRect(iRect);

	HBRUSH	 brush	= CreateSolidBrush(color);
	RECT	 wRect	= { rect.left, rect.top, rect.right, rect.bottom };

	if (style & Rect::Filled)
	{
		if (style & Rect::Rounded)
		{
			HPEN	 pen		= CreatePen(PS_SOLID, 0, color);
			HBRUSH	 previousBrush	= (HBRUSH) SelectObject(paintContext, brush);
			HPEN	 previousPen	= (HPEN) SelectObject(paintContext, pen);

			RoundRect(paintContext, rect.left, rect.top, rect.right, rect.bottom, ellipse.cx, ellipse.cy);

			SelectObject(paintContext, previousBrush);
			SelectObject(paintContext, previousPen);

			::DeleteObject(pen);
		}
		else
		{
			FillRect(paintContext, &wRect, brush);
		}
	}
	else if (style == Rect::Outlined)
	{
		FrameRect(paintContext, &wRect, brush);
	}
	else if (style & Rect::Inverted)
	{
		InvertRect(paintContext, &wRect);
	}
	else if (style & Rect::Dotted)
	{
		for (Int x = rect.left						    + 1;  x <  rect.right;  x += 2) ::SetPixel(paintContext, x, rect.top, color);
		for (Int y = rect.top	 - (rect.GetWidth()		      ) % 2 + 2;  y <  rect.bottom; y += 2) ::SetPixel(paintContext, rect.right - 1, y, color);
		for (Int x = rect.right	 - (rect.GetWidth() + rect.GetHeight()) % 2 - 2;  x >= rect.left;   x -= 2) ::SetPixel(paintContext, x, rect.bottom - 1, color);
		for (Int y = rect.bottom - (		      rect.GetHeight()) % 2 - 1;  y >= rect.top;    y -= 2) ::SetPixel(paintContext, rect.left, y, color);
	}

	::DeleteObject(brush);

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceGDI::SetText(const String &string, const Rect &iRect, const Font &iFont, Bool shadow)
{
	if (window == NIL) return Success();

	if (string == NIL) return Error();
	if (shadow)	   return SurfaceBackend::SetText(string, iRect, iFont, shadow);

	Int	 stringLength = string.Length();

	Font	 font	      = iFont;
	Rect	 rect	      = iRect;
	Int	 lineHeight   = 0;

	/* Fall back to Tahoma when trying to draw Hebrew on pre Windows 8 using Segoe UI.
	 */
	if (font.GetName() == "Segoe UI" && !Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2))
	{
		for (Int i = 0; i < stringLength; i++) if (string[i] >= 0x0590 && string[i] <= 0x05FF) { font.SetName("Tahoma"); break; }
	}

	/* Set up Windows font.
	 */
	HFONT	 hfont	  = CreateFont(-Math::Round(font.GetSize() * fontSize.TranslateY(96) / 72.0), 0, 0, 0, font.GetWeight(), font.GetStyle() & Font::Italic, font.GetStyle() & Font::Underline, font.GetStyle() & Font::StrikeOut, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font.GetName());
	HFONT	 holdfont = (HFONT) SelectObject(paintContext, hfont);

	SetBkMode(paintContext, TRANSPARENT);
	SetTextColor(paintContext, font.GetColor());

	/* Draw text line by line.
	 */
	const Array<String>	&lines = string.Explode("\n");

	if (lines.Length() > 1) lineHeight = font.GetScaledTextSizeY() + 3;

	/* Area to be copied to the window if not painting.
	 */
	Rect	 updateRect = rightToLeft.TranslateRect(Rect(rect.GetPosition(), Size(rect.GetWidth(), rect.GetHeight() + lineHeight * (lines.Length() - 1))));

	updateRect.left	 -= 10;
	updateRect.right += 10;

	foreach (const String &line, lines)
	{
		Int	 lineLength = line.Length();

		/* Check for right to left characters in text.
		 */
		Bool	 rtlCharacters = False;

		for (Int i = 0; i < lineLength; i++) if (line[i] >= 0x0590 && line[i] <= 0x08FF) { rtlCharacters = True; break; }

		/* Draw text, reordering if necessary.
		 */
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
			FriBidiChar	*visual = new FriBidiChar [lineLength + 1];
			FriBidiParType	 type = (rightToLeft.GetRightToLeft() ? FRIBIDI_PAR_RTL : FRIBIDI_PAR_LTR);

			fribidi_log2vis((FriBidiChar *) line.ConvertTo("UCS-4LE"), lineLength, &type, visual, NIL, NIL, NIL);

			visual[lineLength] = 0;

			visualLine.ImportFrom("UCS-4LE", (char *) visual);

			delete [] visual;

			GCP_RESULTS	 results;
			wchar_t		*glyphs = new wchar_t [lineLength + 1];

			ZeroMemory(&results, sizeof(results));

			results.lStructSize = sizeof(results);
			results.lpGlyphs    = glyphs;
			results.nGlyphs     = lineLength + 1;

			ZeroMemory(glyphs, 2 * (lineLength + 1));

			if (rightToLeft.GetRightToLeft()) SetTextAlign(paintContext, TA_RIGHT);
			else				  SetTextAlign(paintContext, TA_LEFT);

			GetCharacterPlacement(paintContext, visualLine, lineLength, 0, &results, 0);

			if (rightToLeft.GetRightToLeft()) wRect.left -= 10;
			else				  wRect.right += 10;

			ExtTextOut(paintContext, (rightToLeft.GetRightToLeft() ? wRect.right : wRect.left), wRect.top, ETO_CLIPPED | ETO_GLYPH_INDEX, &wRect, results.lpGlyphs, results.nGlyphs, NIL);

			delete [] glyphs;
		}
		else
		{
			/* Let Windows do any reordering and ligating.
			 * Works with Kanji, but RTL is only supported
			 * on XP and later versions of Windows and even
			 * later versions get the base direction wrong.
			 */
			SetTextAlign(paintContext, TA_LEFT);

			DrawTextEx(paintContext, line, -1, &wRect, DT_EXPANDTABS | DT_NOPREFIX | (rightToLeft.GetRightToLeft() ? DT_RIGHT : DT_LEFT) | (rtlCharacters ? DT_RTLREADING : 0), NIL);
		}

		rect.top += lineHeight;
	}

	SelectObject(paintContext, holdfont);

	::DeleteObject(hfont);

	if (!painting) PaintRect(updateRect);

	return Success();
}

S::Int S::GUI::SurfaceGDI::Gradient(const Rect &iRect, const Color &color1, const Color &color2, Int style)
{
	if (window == NIL) return Success();

	Rect	 rect = rightToLeft.TranslateRect(iRect);

	/* Setup colors.
	 */
	Color	 c1   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color2 : color1;
	Color	 c2   = (style == OR_HORZ && rightToLeft.GetRightToLeft()) ? color1 : color2;

	/* Setup GDI structures and draw gradient.
	 */
	GRADIENT_RECT	 rects[1]    = { { 0, 1 } };
	TRIVERTEX	 vertices[2] = { { rect.left,  rect.top,    COLOR16(c1.GetRed() << 8), COLOR16(c1.GetGreen() << 8), COLOR16(c1.GetBlue() << 8), 0 },
					 { rect.right, rect.bottom, COLOR16(c2.GetRed() << 8), COLOR16(c2.GetGreen() << 8), COLOR16(c2.GetBlue() << 8), 0 } };

	if (style == OR_HORZ) GdiGradientFill(paintContext, vertices, 2, rects, 1, GRADIENT_FILL_RECT_H);
	else		      GdiGradientFill(paintContext, vertices, 2, rects, 1, GRADIENT_FILL_RECT_V);

	if (!painting) PaintRect(rect);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(const Bitmap &bitmap, const Rect &srcRect, const Rect &iDestRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 destRect = rightToLeft.TranslateRect(iDestRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	HDC		 cdc	= CreateCompatibleDC(paintContext);
	BLENDFUNCTION	 blend	= { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	Bitmap	 premultipliedBitmap = bitmap;

	PremultiplyAlpha(premultipliedBitmap);

	if (destRect.GetSize() == srcRect.GetSize())
	{
		HBITMAP	 backup = (HBITMAP) SelectObject(cdc, premultipliedBitmap.GetSystemBitmap());

		if (bitmap.GetDepth() == 32) AlphaBlend(paintContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), cdc, srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight(), blend);
		else			     BitBlt(paintContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), cdc, srcRect.left, srcRect.top, SRCCOPY);

		SelectObject(cdc, backup);
	}
	else
	{
		Float	 scaleFactorX = Float(srcRect.GetWidth()) / Float(destRect.GetWidth());
		Float	 scaleFactorY = Float(srcRect.GetHeight()) / Float(destRect.GetHeight());

		Size		 srcSize   = premultipliedBitmap.GetSize();
		const Bitmap	&srcBitmap = premultipliedBitmap.Scale(Size(Float(srcSize.cx) / scaleFactorX, Float(srcSize.cy) / scaleFactorY));

		HBITMAP	 backup = (HBITMAP) SelectObject(cdc, srcBitmap.GetSystemBitmap());

		if (bitmap.GetDepth() == 32) AlphaBlend(paintContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), cdc, Float(srcRect.left) / scaleFactorX, Float(srcRect.top) / scaleFactorY, Float(srcRect.GetWidth()) / scaleFactorX, Float(srcRect.GetHeight()) / scaleFactorY, blend);
		else			     BitBlt(paintContext, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), cdc, Float(srcRect.left) / scaleFactorX, Float(srcRect.top) / scaleFactorY, SRCCOPY);

		SelectObject(cdc, backup);
	}

	DeleteDC(cdc);

	if (!painting) PaintRect(destRect);

	return Success();
}

S::Int S::GUI::SurfaceGDI::BlitToBitmap(const Rect &iSrcRect, Bitmap &bitmap, const Rect &destRect)
{
	if (window == NIL) return Success();
	if (bitmap == NIL) return Error();

	Rect	 srcRect = rightToLeft.TranslateRect(iSrcRect);

	if (srcRect.GetWidth()  == 0 || srcRect.GetHeight()  == 0 ||
	    destRect.GetWidth() == 0 || destRect.GetHeight() == 0) return Success();

	/* Copy the image.
	 */
	HDC	 cdc	 = CreateCompatibleDC(paintContext);
	HBITMAP	 backup	 = (HBITMAP) SelectObject(cdc, bitmap.GetSystemBitmap());

	if (destRect.GetSize() == srcRect.GetSize())
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), paintContext, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		SetStretchBltMode(cdc, HALFTONE);
		StretchBlt(cdc, destRect.left, destRect.top, destRect.GetWidth(), destRect.GetHeight(), paintContext, srcRect.left, srcRect.top, srcRect.GetWidth(), srcRect.GetHeight(), SRCCOPY);
	}

	SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success();
}
