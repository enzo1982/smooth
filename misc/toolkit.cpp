 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __TOOLKIT_
#define __TOOLKIT_

#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/string.h>
#include <smooth/loop.h>
#include <smooth/window.h>
#include <smooth/stk.h>
#include <smooth/array.h>

#include <math.h>
#include <picture.h>
#include <iconv.h>
#include <iolib-cxx.h>

#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif

SMOOTHArray<HBITMAP>	 oldBitmaps;
SMOOTHArray<HDC>	 contexts;
SMOOTHArray<int>	 contextCounts;

#ifndef __WIN32__
#ifdef __QNX__
#else
Display	*default_display = XOpenDisplay(NIL);

HWND::HWND()
{
	display	= default_display;
	window	= 0;
}

HWND::HWND(int val)
{
	display	= default_display;
	window	= val;
}

const HWND &HWND::operator =(int val)
{
	display	= default_display;
	window	= val;

	return (const HWND &) (*this);
}

bool HWND::operator ==(int val)
{
	if (window == (unsigned int) val)	return true;
	else					return false;
}

bool HWND::operator !=(int val)
{
	if (window != (unsigned int) val)	return true;
	else					return false;
}

bool HWND::operator ==(HWND wnd)
{
	if (window == wnd.window && display == wnd.display)	return true;
	else							return false;
}

bool HWND::operator !=(HWND wnd)
{
	if (window != wnd.window || display != wnd.display)	return true;
	else							return false;
}

HDC::HDC()
{
	display		= default_display;
	drawable	= 0;
	context		= 0;
}

HDC::HDC(int val)
{
	display		= default_display;
	drawable	= val;
	context		= 0;
}

const HDC &HDC::operator =(int val)
{
	display		= default_display;
	drawable	= val;
	context		= 0;

	return (const HDC &) (*this);
}

bool HDC::operator ==(int val)
{
	if (drawable == (unsigned int) val)	return true;
	else					return false;
}

bool HDC::operator !=(int val)
{
	if (drawable != (unsigned int) val)	return true;
	else					return false;
}

#endif
#endif

int MouseX(HWND hwnd, int wincl)
{
#if defined __WIN32__
	POINT			 point;
	RECT			 rect;

	GetWindowRect(hwnd, &rect);
	GetCursorPos(&point);
#elif defined __QNX__
	SMOOTHPoint		 point;
	SMOOTHRect		 rect;

	point.x = 0;
	rect.left = 0;
#else
	SMOOTHPoint		 point;
	SMOOTHRect		 rect;
	Window			 root_return;
	Window			 child_return;
	int			 root_x_return;
	int			 root_y_return;
	int			 win_x_return;
	int			 win_y_return;
	unsigned int		 mask_return;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);
	XQueryPointer(hwnd.display, hwnd.window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

	point.x = root_x_return;
	rect.left = window_attributes_return.x;
#endif

	if (wincl == SCREEN)		return point.x;
	else if (wincl == WINDOW)	return point.x-rect.left;
	else if (wincl == CLIENT)	return point.x-rect.left-2;

	return -1;
}

int MouseY(HWND hwnd, int wincl)
{
#if defined __WIN32__
	POINT			 point;
	RECT			 rect;

	GetWindowRect(hwnd, &rect);
	GetCursorPos(&point);
#elif defined __QNX__
	SMOOTHPoint		 point;
	SMOOTHRect		 rect;
	
	point.y = 0;
	rect.top = 0;
#else
	SMOOTHPoint		 point;
	SMOOTHRect		 rect;
	Window			 root_return;
	Window			 child_return;
	int			 root_x_return;
	int			 root_y_return;
	int			 win_x_return;
	int			 win_y_return;
	unsigned int		 mask_return;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);
	XQueryPointer(hwnd.display, hwnd.window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

	point.y = root_y_return;
	rect.top = window_attributes_return.y;
#endif

	if (wincl == SCREEN)		return point.y;
	else if (wincl == WINDOW)	return point.y-rect.top;
	else if (wincl == CLIENT)	return point.y-rect.top-2;

	return -1;
}

int WinX(HWND hwnd, int wincl)
{
#if defined __WIN32__
	RECT			 rect;

	if (wincl == WINDOW)	GetWindowRect(hwnd, &rect);
	else			GetClientRect(hwnd, &rect);
#elif defined __QNX__
	SMOOTHRect		 rect;
	
	rect.right = 0;
	rect.left = 0;
#else
	SMOOTHRect		 rect;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);

	rect.left	= window_attributes_return.x;
	rect.right	= rect.left + window_attributes_return.width;
#endif

	return (rect.right - rect.left);
}

int WinY(HWND hwnd, int wincl)
{
#if defined __WIN32__
	RECT			 rect;

	if (wincl == WINDOW)	GetWindowRect(hwnd, &rect);
	else			GetClientRect(hwnd, &rect);
#elif defined __QNX__
	SMOOTHRect		 rect;
	
	rect.bottom = 0;
	rect.top = 0;
#else
	SMOOTHRect		 rect;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);

	rect.top	= window_attributes_return.y;
	rect.bottom	= rect.left + window_attributes_return.height;
#endif

	return (rect.bottom - rect.top);
}

int GetBitmapSizeX(HBITMAP bmp)
{
#ifdef __WIN32__
	BITMAP	 bitmap;

	GetObjectA(bmp, sizeof(bitmap), &bitmap);

	return bitmap.bmWidth;
#else
	return 0;
#endif
}

int GetBitmapSizeY(HBITMAP bmp)
{
#ifdef __WIN32__
	BITMAP	 bitmap;

	GetObjectA(bmp, sizeof(bitmap), &bitmap);

	return bitmap.bmHeight;
#else
	return 0;
#endif
}

SMOOTHRect GetBitmapMetrics(HBITMAP bmp)
{
	SMOOTHRect	 rect;

	rect.left	= 0;
	rect.top	= 0;
	rect.right	= GetBitmapSizeX(bmp);
	rect.bottom	= GetBitmapSizeY(bmp);

	return rect;
}

HDC GetContext(SMOOTHWindow *wnd)
{
#if defined __WIN32__
	int	 code = 0;

	if (wnd != NIL) code = (int) wnd->hwnd;

	if (contextCounts.GetEntry(code) == 0)
	{
		contexts.AddEntry(GetWindowDC((HWND) code), code);
		contextCounts.AddEntry(1, code);
	}
	else
	{
		contextCounts.SetEntry(code, contextCounts.GetEntry(code) + 1);
	}

	return contexts.GetEntry(code);
#elif defined __QNX__
	return NIL;
#else
	HDC		 hdc;
	HWND		 hwnd;
	XGCValues	 gc_values;

	if (wnd == NIL)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}
	else
	{
		hwnd = wnd->hwnd;
	}

	hdc.display = default_display;
	hdc.drawable = hwnd.window;
	hdc.context = XCreateGC(hdc.display, hdc.drawable, 0, &gc_values);

	return hdc;
#endif
}

void FreeContext(SMOOTHWindow *wnd, HDC hdc)
{
#if defined __WIN32__
	int	 code = 0;

	if (wnd != NIL) code = (int) wnd->hwnd;

	contextCounts.SetEntry(code, contextCounts.GetEntry(code) - 1);

	if (contextCounts.GetEntry(code) == 0)
	{
		ReleaseDC((HWND) code, contexts.GetEntry(code));
		DeleteDC(contexts.GetEntry(code));

		contexts.DeleteEntry(code);
		contextCounts.DeleteEntry(code);
	}
#elif defined __QNX__
#else
	XFreeGC(hdc.display, hdc.context);
#endif
}

HDC CreateCompatibleContext(HDC origdc, SMOOTHSize bmpsize)
{
#ifdef __WIN32__
	HDC	 cdc = CreateCompatibleDC(origdc);
	HBITMAP	 bmp = CreateBitmap(bmpsize.cx, bmpsize.cy, GetDeviceCaps(cdc, PLANES), GetDeviceCaps(cdc, BITSPIXEL), NIL);
	HBITMAP	 oldbmp;

	oldbmp = (HBITMAP) SelectObject(cdc, bmp);

	oldBitmaps.AddEntry(oldbmp, (int) cdc);

	return cdc;
#else
	return NIL;
#endif
}

void FreeCompatibleContext(HDC cdc)
{
#ifdef __WIN32__
	HBITMAP	 bmp = (HBITMAP) SelectObject(cdc, oldBitmaps.GetEntry((int) cdc));

	oldBitmaps.DeleteEntry((int) cdc);

	DestroyBitmap(bmp);
	DeleteDC(cdc);
#endif
}

void Line(HDC hdc, SMOOTHPoint p1, SMOOTHPoint p2, long color, int style, int width)
{
#ifdef __WIN32__
	HPEN	 hpen;
	HPEN	 holdpen;

	hpen = CreatePen(style, width, color);
	holdpen = (HPEN) SelectObject(hdc, hpen);
	MoveToEx(hdc, p1.x, p1.y, NIL);
	LineTo(hdc, p2.x, p2.y);
	SelectObject(hdc, holdpen);
	DeleteObject(hpen);
#endif
}

void Frame(HDC hdc, SMOOTHRect rect, int style)
{
#ifdef __WIN32__
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

	Line(hdc, p1, p2, color1, PS_SOLID, 1);
	Line(hdc, p1, p3, color1, PS_SOLID, 1);
	Line(hdc, p2, p4, color2, PS_SOLID, 1);
	p4.x++;
	Line(hdc, p3, p4, color2, PS_SOLID, 1);
#endif
}

void Gradient(HDC hdc, SMOOTHRect rect, long col1, long col2, int style)
{
#ifdef __WIN32__
	double		 red1 = GetRed(col1);
	double		 green1 = GetGreen(col1);
	double		 blue1 = GetBlue(col1);
	double		 red2 = GetRed(col2);
	double		 green2 = GetGreen(col2);
	double		 blue2 = GetBlue(col2);
	double		 biasr;
	double		 biasg;
	double		 biasb;
	int		 xmax;
	int		 ymax;
	SMOOTHRect	 srect;
	picture		*pic = new picture(rect.right-rect.left, rect.bottom-rect.top, 24);

	xmax = rect.right-rect.left;
	ymax = rect.bottom-rect.top;

	switch (style)
	{
		case GRADIENT_LR: // left->right
		{
			biasr = (red2-red1) / xmax;
			biasg = (green2-green1) / xmax;
			biasb = (blue2-blue1) / xmax;

			for (int x=0; x<xmax; x++)
			{
				red1=red1+biasr;
				green1=green1+biasg;
				blue1=blue1+biasb;
				for (int y=0; y<ymax; y++) pic->SetPixel(x, y, RGB(red1, green1, blue1));
			}
		}
		break;

		case GRADIENT_TB: // top->bottom
		{
			biasr = (red2-red1) / ymax;
			biasg = (green2-green1) / ymax;
			biasb = (blue2-blue1) / ymax;

			for (int y=0; y<ymax; y++)
			{
				red1=red1+biasr;
				green1=green1+biasg;
				blue1=blue1+biasb;
				for (int x=0; x<xmax; x++) pic->SetPixel(x, y, RGB(red1, green1, blue1));
			}
		}
		break;
	}

	srect.left = 0;
	srect.top = 0;
	srect.right = rect.right-rect.left;
	srect.bottom = rect.bottom-rect.top;

	{
		RECT	 Rect = rect;
		RECT	 sRect = srect;

		pic->BlitToDC(hdc, &Rect, &sRect);
	}

	delete pic;
#endif
}

void Box(HDC hdc, SMOOTHRect rect, int color, int style)
{
#ifdef __WIN32__
	HBRUSH	brush = CreateSolidBrush(color);
	bool	dot = false;
	int	x;
	int	y;

	RECT	 Rect = rect;

	if (style == FILLED)		FillRect(hdc, &Rect, brush);
	else if (style == OUTLINED)	FrameRect(hdc, &Rect, brush);
	else
	{
		dot = false;
		y = rect.top;

		for (x = rect.left; x < rect.right - 1; x++)
		{
			if (dot == true)
			{
				SetPixel(hdc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		x = rect.right - 1;

		for (y = rect.top; y < rect.bottom; y++)
		{
			if (dot == true)
			{
				SetPixel(hdc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		y = rect.bottom - 1;

		for (x = rect.right - 2; x >= rect.left; x--)
		{
			if (dot == true)
			{
				SetPixel(hdc, x, y, color);
				dot = false;
			}
			else dot = true;
		}

		x = rect.left;

		for (y = rect.bottom - 2; y >= rect.top; y--)
		{
			if (dot == true)
			{
				SetPixel(hdc, x, y, color);
				dot = false;
			}
			else dot = true;
		}
	}

	DeleteObject(brush);
#endif
}

void HBar(HDC hdc, SMOOTHPoint p1, SMOOTHPoint p2)
{
#ifdef __WIN32__
	Line(hdc, p1, p2, RGB(max(GetRed(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetGreen(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetBlue(SMOOTH::Setup::BackgroundColor) - 64, 0)), PS_SOLID, 1);
	SetPixel(hdc, p2.x, p2.y, RGB(min(GetRed(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetGreen(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetBlue(SMOOTH::Setup::BackgroundColor) + 64, 255)));
	p1.y++;
	p2.y++;
	p2.x++;
	Line(hdc, p1, p2, RGB(min(GetRed(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetGreen(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetBlue(SMOOTH::Setup::BackgroundColor) + 64, 255)), PS_SOLID, 1);
#endif
}

void VBar(HDC hdc, SMOOTHPoint p1, SMOOTHPoint p2)
{
#ifdef __WIN32__
	p2.y++;
	Line(hdc, p1, p2, RGB(max(GetRed(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetGreen(SMOOTH::Setup::BackgroundColor) - 64, 0), max(GetBlue(SMOOTH::Setup::BackgroundColor) - 64, 0)), PS_SOLID, 1);
	p1.x++;
	p2.x++;
	Line(hdc, p1, p2, RGB(min(GetRed(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetGreen(SMOOTH::Setup::BackgroundColor) + 64, 255), min(GetBlue(SMOOTH::Setup::BackgroundColor) + 64, 255)), PS_SOLID, 1);
#endif
}

void PaintPixel(HDC hdc, SMOOTHPoint point, int color)
{
#if defined __WIN32__
	SetPixel(hdc, point.x, point.y, color);
#elif defined __QNX__
#else
	XSetForeground(hdc.display, hdc.context, color);
	XSetFunction(hdc.display, hdc.context, GXcopy);
	XDrawPoint(hdc.display, hdc.drawable, hdc.context, point.x, point.y);
#endif
}

int GetPixel(HDC hdc, SMOOTHPoint point)
{
#ifdef __WIN32__
	return GetPixel(hdc, point.x, point.y);
#else
	return 0;
#endif
}

void PaintBitmap(HDC hdc, SMOOTHRect iconrect, HBITMAP iconmap)
{
#ifdef __WIN32__
	HDC	 icondc;
	HBITMAP	 oldbmp;
	BITMAP	 bmp;

	GetObjectA(iconmap, sizeof(bmp), &bmp);

	icondc = (HDC) CreateCompatibleDC(hdc);
	oldbmp = (HBITMAP) SelectObject(icondc, iconmap);

	StretchBlt(hdc, iconrect.left, iconrect.top, iconrect.right - iconrect.left, iconrect.bottom - iconrect.top, icondc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	iconmap = (HBITMAP) SelectObject(icondc, oldbmp);
	DeleteDC(icondc);
#endif
}

void SetShadowedText(HDC hdc, SMOOTHString string, SMOOTHRect rect, SMOOTHString font, int size, int color, int weight)
{
#ifdef __WIN32__
	if (string == NIL) return;

	rect.left += 2;
	rect.top += 2;
	rect.right += 2;
	rect.bottom += 2;

	SetText(hdc, string, rect, font, size, RGB(((double) GetRed(SMOOTH::Setup::BackgroundColor))/3*2, ((double) GetGreen(SMOOTH::Setup::BackgroundColor))/3*2, ((double) GetBlue(SMOOTH::Setup::BackgroundColor))/3*2), weight);

	rect.left -= 2;
	rect.top -= 2;
	rect.right -= 2;
	rect.bottom -= 2;

	SetText(hdc, string, rect, font, size, color, weight);
#endif
}

void SetText(HDC hdc, SMOOTHString string, SMOOTHRect rect, SMOOTHString font, int size, int color, int weight)
{
#ifdef __WIN32__
	if (string == NIL) return;

	HFONT		 hfont;
	HFONT		 holdfont;
	int		 lines = 1;
	int		 offset = 0;
	int		 origoffset;
	int		 height = GetLineSizeY(hdc, string, font, size, weight) + 3;
	int		 txtsize = string.Length();
	int		 i;
	SMOOTHString	 line;

	for (i = 0; i < txtsize; i++) if (string[i] == 10) lines++;

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, color);

	if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else					hfont = CreateFontA(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(hdc, hfont);

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

		if (SMOOTH::Setup::enableUnicode)	DrawTextW(hdc, line, -1, &Rect, DT_LEFT);
		else					DrawTextA(hdc, line, -1, &Rect, DT_LEFT);

		rect.top += height;
	}

	SelectObject(hdc, holdfont);
	DeleteObject(hfont);
#endif
}

bool IsMouseOn(HWND hwnd, SMOOTHRect rect, int wincl)
{
#ifdef __WIN32__
	SMOOTHPoint	mousepos;
	SMOOTHRect	winrect;

	if (wincl == WINDOW)
	{
		RECT	 wRect = winrect;
		POINT	 mPos = mousepos;

		GetWindowRect(hwnd, &wRect);
		GetCursorPos(&mPos);

		winrect = wRect;
		mousepos = mPos;

		mousepos.x = mousepos.x - winrect.left;
		mousepos.y = mousepos.y - winrect.top;
	}
	else
	{
		RECT	 wRect = winrect;
		POINT	 mPos = mousepos;

		GetClientRect(hwnd, &wRect);
		GetCursorPos(&mPos);

		winrect = wRect;
		mousepos = mPos;

		mousepos.x = mousepos.x - winrect.left;
		mousepos.y = mousepos.y - winrect.top;
	}

	if ((mousepos.x >= rect.left) && (mousepos.x <= rect.right) && (mousepos.y >= rect.top) && (mousepos.y <= rect.bottom))
	{
		return true;
	}

	return false;
#else
	return false;
#endif
}

int GetTextSizeXNoExtend(HDC hdc, SMOOTHString text, SMOOTHString font, int size, int weight)
{
	if (text == NIL) return 0;

	return GetTextSizeX(hdc, text, text.Length(), font, size, weight, false);
}

int GetTextSizeXNoExtend(HDC hdc, SMOOTHString text, int nofchars, SMOOTHString font, int size, int weight)
{
	if (text == NIL) return 0;

	return GetTextSizeX(hdc, text, nofchars, font, size, weight, false);
}

int GetTextSizeX(HDC hdc, SMOOTHString text, int nofchars, SMOOTHString font, int size, int weight)
{
	if (text == NIL) return 0;

	return GetTextSizeX(hdc, text, nofchars, font, size, weight, true);
}

int GetTextSizeX(HDC hdc, SMOOTHString text, SMOOTHString font, int size, int weight)
{
	if (text == NIL) return 0;

	return GetTextSizeX(hdc, text, text.Length(), font, size, weight, true);
}

int GetTextSizeX(HDC hdc, SMOOTHString text, int nofchars, SMOOTHString font, int size, int weight, bool extend)
{
	if (text == NIL) return 0;
	if (nofchars == 0) return -1;

	int		 sizex = 0;
	int		 lines = 1;
	int		 offset = 0;
	int		 origoffset;
	int		 txtlen = nofchars;
	SMOOTHString	 line;
	int		 i;

	for (i = 0; i < txtlen; i++) if (text[i] == 10) lines++;

	for (i = 0; i < lines; i++)
	{
		origoffset = offset;

		for (int j = 0; j <= txtlen; j++)
		{
			if (j + origoffset == txtlen)
			{
				line[j] = 0;
				break;
			}

			if (text[j + origoffset] == 10 || text[j + origoffset] == 0)
			{
				offset++;
				line[j] = 0;
				break;
			}
			else
			{
				offset++;
				line[j] = text[j + origoffset];
			}
		}

		sizex = max(sizex, GetLineSizeX(hdc, line, line.Length(), font, size, weight, extend));
	}

	return sizex;
}

int GetLineSizeX(HDC hdc, SMOOTHString text, int nofchars, SMOOTHString font, int size, int weight, bool extend)
{
#ifdef __WIN32__
	if (text == NIL) return 0;
	if (nofchars == 0) return -1;

	SMOOTHRect	 rect;
	SMOOTHSize	 tsize;
	HDC		 cdc;
	HFONT		 hfont;
	HFONT		 holdfont;
	HBITMAP		 bmpbmp;
	HBITMAP		 newbmp;
	int		 txsize = size;
	SMOOTHString	 filtered;
	int		 bias = 0;

	// filtering out '&'s

	for (int i = 0; i < nofchars; i++)
	{
		filtered[i] = 0;

		if (text[i] == '&')	bias++;
		else			filtered[i-bias] = text[i];
	}

	filtered[nofchars] = 0;

	if (size < 0) size = size - (2*size);

	cdc = CreateCompatibleDC(hdc);

	if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(txsize, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else					hfont = CreateFontA(txsize, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(cdc, hfont);

	SIZE	 ts = tsize;

	if (SMOOTH::Setup::enableUnicode)	GetTextExtentPoint32W(cdc, filtered, nofchars-bias, &ts);
	else					GetTextExtentPoint32A(cdc, filtered, nofchars-bias, &ts);

	tsize = ts;

	rect.left = 0;
	rect.top = 0;
	rect.right = tsize.cx + 1;
	rect.bottom = size;

	bmpbmp = CreateBitmap(tsize.cx + 1, size, GetDeviceCaps(cdc, PLANES), GetDeviceCaps(cdc, BITSPIXEL), NIL);
	newbmp = (HBITMAP) SelectObject(cdc, bmpbmp);
	Box(cdc, rect, 0, FILLED);
	SetTextColor(cdc, RGB(128, 128, 128));
	SetBkMode(cdc, OPAQUE);

	RECT	 Rect = rect;

	if (SMOOTH::Setup::enableUnicode)	DrawTextW(cdc, filtered, -1, &Rect, DT_LEFT);
	else					DrawTextA(cdc, filtered, -1, &Rect, DT_LEFT);

	rect = Rect;

	SelectObject(cdc, holdfont);
	DeleteObject(hfont);

	if (extend)
	{
		for (int y = 0; y < size; y++)
		{
			if (GetPixel(cdc, tsize.cx - 1, y) == RGB(128, 128, 128))
			{
				tsize.cx++;
				break;
			}
		}
	}

	DeleteDC(cdc);

	DeleteObject(bmpbmp);
	DeleteObject(newbmp);

	return tsize.cx - 1;
#else
	return 0;
#endif
}

int GetTextSizeY(HDC hdc, SMOOTHString text, SMOOTHString font, int size, int weight)
{
	if (text == NIL) return 0;

	int	 lines = 1;
	int	 txtsize = text.Length();

	for (int i = 0; i < txtsize; i++)
	{
		if (text[i] == 10) lines++;
	}

	return (lines * GetLineSizeY(hdc, text, font, size, weight)) + (lines - 1) * 3;
}

int GetLineSizeY(HDC hdc, SMOOTHString text, SMOOTHString font, int size, int weight)
{
#ifdef __WIN32__
	if (text == NIL) return 0;

	SMOOTHSize	 tsize;
	HDC		 cdc;
	HFONT		 hfont;
	HFONT		 holdfont;
	int		 txsize = size;
	int		 nofchars = text.Length();

	if (size < 0) size = size - (2*size);

	cdc = CreateCompatibleDC(hdc);

	if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(txsize, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else					hfont = CreateFontA(txsize, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(cdc, hfont);

	SIZE	 ts = tsize;

	if (SMOOTH::Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, nofchars, &ts);
	else					GetTextExtentPoint32A(cdc, text, nofchars, &ts);

	tsize = ts;

	SelectObject(cdc, holdfont);
	DeleteObject(hfont);

	DeleteDC(cdc);

	return tsize.cy - 1;
#else
	return 0;
#endif
}

bool DoRectsOverlap(SMOOTHRect rect1, SMOOTHRect rect2)
{
	if ((rect1.left < rect2.right) && (rect1.right > rect2.left) && (rect1.top < rect2.bottom) && (rect1.bottom > rect2.top))
	{
		return true;
	}
	else
	{
		return false;
	}
}

SMOOTHRect OverlapRect(SMOOTHRect rect1, SMOOTHRect rect2)
{
	SMOOTHRect orect;

	orect.left	= 0;
	orect.top	= 0;
	orect.right	= 0;
	orect.bottom	= 0;

	if (DoRectsOverlap(rect1, rect2))
	{
		orect.left	= max(rect1.left, rect2.left);
		orect.top	= max(rect1.top, rect2.top);
		orect.right	= min(rect1.right, rect2.right);
		orect.bottom	= min(rect1.bottom, rect2.bottom);
	}

	return orect;
}

HBITMAP BlitToBitmap(HDC dc, SMOOTHRect rect)
{
#ifdef __WIN32__
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 newbmp;
	HBITMAP	 backup;

	newbmp = CreateBitmap(rect.right - rect.left + 1, rect.bottom - rect.top + 1, GetDeviceCaps(dc, PLANES), GetDeviceCaps(dc, BITSPIXEL), NIL);
	backup = (HBITMAP) SelectObject(cdc, newbmp);

	BitBlt(cdc, 0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1, dc, rect.left, rect.top, SRCCOPY);

	newbmp = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	DestroyBitmap(backup);

	return newbmp;
#else
	return NIL;
#endif
}

bool BlitToBitmap(SMOOTHRect srcrect, HDC dc, SMOOTHRect destrect, HBITMAP bmp)
{
#ifdef __WIN32__
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 backup;

	backup = (HBITMAP) SelectObject(cdc, bmp);

	StretchBlt(cdc, destrect.left, destrect.top, destrect.right - destrect.left + 1, destrect.bottom - destrect.top + 1, dc, srcrect.left, srcrect.top, srcrect.right - srcrect.left + 1, srcrect.bottom - srcrect.top + 1, SRCCOPY);

	bmp = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	DestroyBitmap(backup);

	return true;
#else
	return false;
#endif
}

bool BlitFromBitmap(SMOOTHRect srcrect, HBITMAP bitmap, SMOOTHRect destrect, HDC dc)
{
#ifdef __WIN32__
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 backup;

	backup = (HBITMAP) SelectObject(cdc, bitmap);

	StretchBlt(dc, destrect.left, destrect.top, destrect.right - destrect.left + 1, destrect.bottom - destrect.top + 1, cdc, srcrect.left, srcrect.top, srcrect.right - srcrect.left + 1, srcrect.bottom - srcrect.top + 1, SRCCOPY);

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	DestroyBitmap(backup);

	return true;
#else
	return false;
#endif
}

bool DestroyBitmap(HBITMAP bitmap)
{
#ifdef __WIN32__
	DeleteObject(bitmap);

	return true;
#else
	return false;
#endif
}

HWND CreateSimpleWindow(SMOOTHRect wndrect, SMOOTHString title, SMOOTHString className, HICON icon, int style, int exstyle)
{
#if defined __WIN32__
	WNDCLASSEXW	 wndclassw;
	WNDCLASSEXA	 wndclassa;
	HWND		 hwnd;

	wndclassw.cbSize	= sizeof(wndclassw);
	wndclassw.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclassw.lpfnWndProc	= SMOOTHWindowProc;
	wndclassw.cbClsExtra	= 0;
	wndclassw.cbWndExtra	= 0;
	wndclassw.hInstance	= hInstance;
	wndclassw.hIcon		= icon;
	wndclassw.hCursor	= NIL;
	wndclassw.hbrBackground	= (HBRUSH) (COLOR_BTNFACE + 1);
	wndclassw.lpszMenuName	= NIL;
	wndclassw.lpszClassName	= className;
	wndclassw.hIconSm	= icon;

	wndclassa.cbSize	= sizeof(wndclassa);
	wndclassa.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclassa.lpfnWndProc	= SMOOTHWindowProc;
	wndclassa.cbClsExtra	= 0;
	wndclassa.cbWndExtra	= 0;
	wndclassa.hInstance	= hInstance;
	wndclassa.hIcon		= icon;
	wndclassa.hCursor	= NIL;
	wndclassa.hbrBackground	= (HBRUSH) (COLOR_BTNFACE + 1);
	wndclassa.lpszMenuName	= NIL;
	wndclassa.lpszClassName	= className;
	wndclassa.hIconSm	= icon;

	if (SMOOTH::Setup::enableUnicode)	RegisterClassExW(&wndclassw);
	else					RegisterClassExA(&wndclassa);

	if (SMOOTH::Setup::enableUnicode)	hwnd = CreateWindowExW(exstyle, className, title, style, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, NIL, NIL, hInstance, NIL);
	else					hwnd = CreateWindowExA(exstyle, className, title, style, wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, NIL, NIL, hInstance, NIL);

	return hwnd;

#elif defined __QNX__
	return NIL;
#else
	HWND	 hwnd;

	hwnd = XCreateSimpleWindow(hwnd.display, DefaultRootWindow(hwnd.display), wndrect.left, wndrect.top, wndrect.right - wndrect.left, wndrect.bottom - wndrect.top, 0, BlackPixel(hwnd.display, DefaultScreen(hwnd.display)), WhitePixel(hwnd.display, DefaultScreen(hwnd.display)));

	XSelectInput(hwnd.display, hwnd.window, StructureNotifyMask);
	XMapWindow(hwnd.display, hwnd.window);

	for (;;)
	{
		XEvent	 e;

		XNextEvent(hwnd.display, &e);

		if (e.type == MapNotify) break;
	}

	return hwnd;
#endif
}

int ConvertString(const char *inbuffer, int inbytes, const char *inencoding, char *outbuffer, int outbytes, const char *outencoding)
{
	bool	 delbuffer = false;

	if (outbuffer == NIL)
	{
		delbuffer = true;
		outbytes = inbytes * 8;
		outbuffer = new char [outbytes];
	}

	for (int i = 0; i < outbytes; i++) outbuffer[i] = 0;

	InStream	*in	= new InStream(STREAM_BUFFER, (void *) inbuffer, inbytes);
	OutStream	*out	= new OutStream(STREAM_BUFFER, (void *) outbuffer, outbytes);
	iconv_t		 cd	= iconv_open(outencoding, inencoding);
	int		 size	= 0;

	iconv(cd, NULL, NULL, NULL, NULL);

	char		 inbuf[4096 + 4096];
	size_t		 inbufrest = 0;
	char		 outbuf[4096];

	for (;;)
	{
		size_t	 inbufsize = min(in->Size() - in->GetPos(), 4096);

		in->InputData((void *) (inbuf + 4096), inbufsize);

		if (inbufsize == 0)
		{
			if (inbufrest == 0)
			{
				break;
			}
			else
			{
				iconv_close(cd);

				delete in;
				delete out;

				return 0;
			}
		}
		else
		{
			const char	*inptr	= inbuf + 4096 - inbufrest;
			size_t		 insize	= inbufrest + inbufsize;

			inbufrest = 0;

			while (insize > 0)
			{
				char	*outptr		= outbuf;
				size_t	 outsize	= sizeof(outbuf);

				iconv(cd, (const char **) &inptr, &insize, &outptr, &outsize);

				if (outptr != outbuf)
				{
					out->OutputData((void *) outbuf, outptr - outbuf);
					size += (outptr - outbuf);
				}
			}
		}
	}

	char	*outptr		= outbuf;
	size_t	 outsize	= sizeof(outbuf);

	iconv(cd, NULL, NULL, &outptr, &outsize);

	if (outptr != outbuf)
	{
		out->OutputData((void *) outbuf, outptr - outbuf);
		size += (outptr - outbuf);
	}

	iconv_close(cd);

	if (size >= outbytes) size = 0;

	delete in;
	delete out;

	if (delbuffer) delete [] outbuffer;

	return size;
}

#endif
