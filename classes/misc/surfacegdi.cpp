 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SURFACEGDI_
#define __OBJSMOOTH_SURFACEGDI_

#include <smooth/surfacegdi.h>
#include <smooth/stk.h>
#include <smooth/toolkit.h>

SMOOTHSurfaceGDI::SMOOTHSurfaceGDI(HDC iDc)
{
	gdi_dc = iDc;

	size.cx	= 0;
	size.cy	= 0;
}

SMOOTHSurfaceGDI::~SMOOTHSurfaceGDI()
{
}

SMOOTHInt SMOOTHSurfaceGDI::SetPixel(SMOOTHInt x, SMOOTHInt y, SMOOTHInt color)
{
	::SetPixel(gdi_dc, x, y, color);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::GetPixel(SMOOTHInt x, SMOOTHInt y)
{
	return ::GetPixel(gdi_dc, x, y);
}

SMOOTHInt SMOOTHSurfaceGDI::Line(SMOOTHPoint pos1, SMOOTHPoint pos2, SMOOTHInt color)
{
	HPEN	 hPen = CreatePen(PS_SOLID, 1, color);
	HPEN	 hOldPen = (HPEN) SelectObject(gdi_dc, hPen);

	MoveToEx(gdi_dc, pos1.x, pos1.y, NIL);
	LineTo(gdi_dc, pos2.x, pos2.y);

	hPen = (HPEN) SelectObject(gdi_dc, hOldPen);
	DeleteObject(hPen);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::Frame(SMOOTHRect rect, SMOOTHInt style)
{
	long		 color1 = 0;
	long		 color2 = 0;
	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	SMOOTHPoint	 p3;
	SMOOTHPoint	 p4;

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
			color1 = RGB(min(GetRed(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetGreen(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetBlue(SMOOTH::Setup::BackgroundColor) + 64, 255));
			color2 = RGB(max(GetRed(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetGreen(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetBlue(SMOOTH::Setup::BackgroundColor) - 64, 0));
		}
		break;

		case FRAME_DOWN: // down
		{
			color1 = RGB(max(GetRed(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetGreen(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetBlue(SMOOTH::Setup::BackgroundColor) - 64, 0));
			color2 = RGB(min(GetRed(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetGreen(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetBlue(SMOOTH::Setup::BackgroundColor) + 64, 255));
		}
		break;
	}

	Line(p1, p2, color1);
	Line(p1, p3, color1);
	Line(p2, p4, color2);
	p4.x++;
	Line(p3, p4, color2);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::Box(SMOOTHRect rect, SMOOTHInt color, SMOOTHInt style)
{
	HBRUSH	 brush = CreateSolidBrush(color);
	RECT	 wRect = rect;

	if (style == FILLED)		FillRect(gdi_dc, &wRect, brush);
	else if (style == OUTLINED)	FrameRect(gdi_dc, &wRect, brush);
	else
	{
		bool	 dot = false;
		int	 x;
		int	 y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == true)
			{
				::SetPixel(gdi_dc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == true)
			{
				::SetPixel(gdi_dc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == true)
			{
				::SetPixel(gdi_dc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == true)
			{
				::SetPixel(gdi_dc, x, y, color);
				dot = false;
			}
			else dot = true;
		}
	}

	DeleteObject(brush);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::SetText(SMOOTHString string, SMOOTHRect rect, SMOOTHString font, SMOOTHInt size, SMOOTHInt color, SMOOTHInt weight)
{
	HFONT		 hfont;
	HFONT		 holdfont;
	int		 lines = 1;
	int		 offset = 0;
	int		 origoffset;
	int		 height = GetLineSizeY(gdi_dc, string, font, size, weight) + 3;
	int		 txtsize = string.Length();
	int		 i;
	SMOOTHString	 line;

	for (i = 0; i < txtsize; i++) if (string[i] == 10) lines++;

	SetBkMode(gdi_dc, TRANSPARENT);
	SetTextColor(gdi_dc, color);

	if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else					hfont = CreateFontA(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(gdi_dc, hfont);

	for (i = 0; i < lines; i++)
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

		if (SMOOTH::Setup::enableUnicode)	DrawTextW(gdi_dc, line, -1, &Rect, DT_LEFT);
		else					DrawTextA(gdi_dc, line, -1, &Rect, DT_LEFT);

		rect.top += height;
	}

	SelectObject(gdi_dc, holdfont);
	DeleteObject(hfont);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::BlitFromBitmap(HBITMAP bitmap, SMOOTHRect srcRect, SMOOTHRect destRect)
{
	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap);

	StretchBlt(gdi_dc, destRect.left, destRect.top, destRect.right - destRect.left + 1, destRect.bottom - destRect.top + 1, cdc, srcRect.left, srcRect.top, srcRect.right - srcRect.left + 1, srcRect.bottom - srcRect.top + 1, SRCCOPY);

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	DestroyBitmap(backup);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSurfaceGDI::BlitToBitmap(SMOOTHRect srcRect, HBITMAP bitmap, SMOOTHRect destRect)
{
	HDC	 cdc = CreateCompatibleDC(gdi_dc);
	HBITMAP	 backup = (HBITMAP) SelectObject(cdc, bitmap);

	StretchBlt(cdc, destRect.left, destRect.top, destRect.right - destRect.left + 1, destRect.bottom - destRect.top + 1, gdi_dc, srcRect.left, srcRect.top, srcRect.right - srcRect.left + 1, srcRect.bottom - srcRect.top + 1, SRCCOPY);

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	DestroyBitmap(backup);

	return SMOOTH::Success;
}

#endif
