 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/surfacegdi.h>
#include <smooth/stk.h>
#include <smooth/toolkit.h>
#include <smooth/color.h>
#include <smooth/math.h>

S::GUI::SurfaceGDI::SurfaceGDI(HDC iDc)
{
	gdi_dc = iDc;
	real_dc = iDc;

	size.cx	= GetDeviceCaps(gdi_dc, HORZRES) + 2;
	size.cy	= GetDeviceCaps(gdi_dc, VERTRES) + 2;

	bmp_dc = CreateCompatibleDC(gdi_dc);

	HBITMAP	 bitmap = CreateCompatibleBitmap(gdi_dc, size.cx, size.cy);

	BlitToBitmap(Rect(Point(0, 0), size), bitmap, Rect(Point(0, 0), size));

	cDc_bitmap = (HBITMAP) SelectObject(bmp_dc, bitmap);
}

S::GUI::SurfaceGDI::~SurfaceGDI()
{
	HBITMAP	 bitmap = (HBITMAP) SelectObject(bmp_dc, cDc_bitmap);

	DeleteDC(bmp_dc);
	::DeleteObject(bitmap);
}

S::Int S::GUI::SurfaceGDI::PaintRect(Rect pRect)
{
	if (painting) return Error;

	BitBlt(gdi_dc, pRect.left, pRect.top, pRect.right - pRect.left, pRect.bottom - pRect.top, bmp_dc, pRect.left, pRect.top, SRCCOPY);

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
		gdi_dc = CreateCompatibleDC(real_dc);
	}

	painting++;

	return Success;
}

S::Int S::GUI::SurfaceGDI::EndPaint()
{
	if (!painting) return Error;

	painting--;

	if (painting == 0)
	{
		DeleteDC(gdi_dc);

		gdi_dc = real_dc;

		PaintRect(paintRect);

		paintRect.left = -1;
		paintRect.top = -1;
		paintRect.right = -1;
		paintRect.bottom = -1;
	}

	return Success;
}

HDC S::GUI::SurfaceGDI::GetContext()
{
	if (painting)	return bmp_dc;
	else		return gdi_dc;
}

S::Int S::GUI::SurfaceGDI::SetPixel(Int x, Int y, Int color)
{
	::SetPixel(gdi_dc, x, y, color);
	::SetPixel(bmp_dc, x, y, color);

	return Success;
}

S::Int S::GUI::SurfaceGDI::GetPixel(Int x, Int y)
{
	return ::GetPixel(gdi_dc, x, y);
}

S::Int S::GUI::SurfaceGDI::Line(Point pos1, Point pos2, Int color)
{
	HPEN	 hPen = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = (HPEN) SelectObject(gdi_dc, hPen);

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
	else
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

S::Int S::GUI::SurfaceGDI::SetText(String string, Rect rect, Font font)
{
	if (string == NIL) return Error;

	HFONT	 hfont;
	HFONT	 holdfont;
	HFONT	 holdfont2;
	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 height = GetLineSizeY(string, font.GetName(), font.GetSize(), font.GetWeight()) + 3;
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
			}
		}

		RECT	 Rect = rect;

		if (Setup::enableUnicode)	DrawTextExW(gdi_dc, line, -1, &Rect, DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX, NIL);
		else				DrawTextExA(gdi_dc, line, -1, &Rect, DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX, NIL);

		if (Setup::enableUnicode)	DrawTextExW(bmp_dc, line, -1, &Rect, DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX, NIL);
		else				DrawTextExA(bmp_dc, line, -1, &Rect, DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX, NIL);

		rect.top += height;
	}

	SelectObject(gdi_dc, holdfont);
	SelectObject(bmp_dc, holdfont2);

	::DeleteObject(hfont);

	return Success;
}

S::Int S::GUI::SurfaceGDI::BlitFromBitmap(HBITMAP bitmap, Rect srcRect, Rect destRect)
{
	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap);

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

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success;
}

S::Int S::GUI::SurfaceGDI::BlitToBitmap(Rect srcRect, HBITMAP bitmap, Rect destRect)
{
	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap);

	if ((destRect.right - destRect.left == srcRect.right - srcRect.left) && (destRect.bottom - destRect.top == srcRect.bottom - srcRect.top))
	{
		BitBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, SRCCOPY);
	}
	else
	{
		StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left, destRect.bottom - destRect.top, gdi_dc, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, SRCCOPY);
	}

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);

	return Success;
}
