 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/string.h>
#include <smooth/loop.h>
#include <smooth/window.h>
#include <smooth/stk.h>
#include <smooth/array.h>
#include <smooth/shlobjmini.h>

#include <picture.h>
#include <iolib-cxx.h>

#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace smooth;

Array<HBITMAP>	 oldBitmaps;
Array<HDC>	 contexts;
Array<int>	 contextCounts;

#ifndef __WIN32__
#ifdef __QNX__
#else
Display	*default_display = XOpenDisplay(NIL);
#endif
#endif

LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderA)(PBROWSEINFOA);
LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderW)(PBROWSEINFOW);
BOOL		 (WINAPI *ex_SHGetPathFromIDListA)(LPCITEMIDLIST, LPSTR);
BOOL		 (WINAPI *ex_SHGetPathFromIDListW)(LPCITEMIDLIST, LPWSTR);

HMODULE	 shelldll;

void SHLObjMini_Init()
{
	shelldll = LoadLibraryA("shell32.dll");

	ex_SHBrowseForFolderA = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOA)) GetProcAddress(shelldll, "SHBrowseForFolderA");
	ex_SHBrowseForFolderW = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOW)) GetProcAddress(shelldll, "SHBrowseForFolderW");
	ex_SHGetPathFromIDListA = (BOOL (WINAPI *)(LPCITEMIDLIST, LPSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListA");
	ex_SHGetPathFromIDListW = (BOOL (WINAPI *)(LPCITEMIDLIST, LPWSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListW");
}

void SHLObjMini_Deinit()
{
	FreeLibrary(shelldll);
}

int GetBitmapSizeX(HBITMAP bmp)
{
	BITMAP	 bitmap;

	GetObjectA(bmp, sizeof(bitmap), &bitmap);

	return bitmap.bmWidth;
}

int GetBitmapSizeY(HBITMAP bmp)
{
	BITMAP	 bitmap;

	GetObjectA(bmp, sizeof(bitmap), &bitmap);

	return bitmap.bmHeight;
}

HDC GetContext(GUI::Window *wnd)
{
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
}

void FreeContext(GUI::Window *wnd, HDC hdc)
{
	int	 code = 0;

	if (wnd != NIL) code = (int) wnd->hwnd;

	contextCounts.SetEntry(code, contextCounts.GetEntry(code) - 1);

	if (contextCounts.GetEntry(code) == 0)
	{
		ReleaseDC((HWND) code, contexts.GetEntry(code));

		contexts.RemoveEntry(code);
		contextCounts.RemoveEntry(code);
	}
}

HDC CreateCompatibleContext(HDC origdc, Size bmpsize)
{
	HDC	 cdc = CreateCompatibleDC(origdc);
	HBITMAP	 bmp = CreateCompatibleBitmap(origdc, bmpsize.cx, bmpsize.cy);
	HBITMAP	 oldbmp;

	oldbmp = (HBITMAP) SelectObject(cdc, bmp);

	oldBitmaps.AddEntry(oldbmp, (int) cdc);

	return cdc;
}

void FreeCompatibleContext(HDC cdc)
{
	HBITMAP	 bmp = (HBITMAP) SelectObject(cdc, oldBitmaps.GetEntry((int) cdc));

	oldBitmaps.RemoveEntry((int) cdc);

	::DeleteObject(bmp);
	DeleteDC(cdc);
}

int GetTextSizeX(String text, String font, int size, int weight)
{
	if (text == NIL) return 0;

	return GetTextSizeX(text, text.Length(), font, size, weight);
}

int GetTextSizeX(String text, int nofchars, String font, int size, int weight)
{
	if (text == NIL) return 0;
	if (nofchars == 0) return 0;

	int	 sizex = 0;
	int	 lines = 1;
	int	 offset = 0;
	int	 origoffset;
	int	 txtlen = nofchars;
	String	 line;
	int	 i;

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

		sizex = max(sizex, GetLineSizeX(line, line.Length(), font, size, weight));
	}

	return sizex;
}

int GetLineSizeX(String text, int nofchars, String font, int size, int weight)
{
	if (text == NIL) return 0;
	if (nofchars == 0) return 0;

	HDC	 cdc = CreateCompatibleDC(0);

	size = -MulDiv(size, GetDeviceCaps(cdc, LOGPIXELSY), 72);

	HFONT	 hfont;
	HFONT	 holdfont;

	if (Setup::enableUnicode)	hfont = CreateFontW(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else				hfont = CreateFontA(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(cdc, hfont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, nofchars, &tSize);
	else				GetTextExtentPoint32A(cdc, text, nofchars, &tSize);

	SelectObject(cdc, holdfont);
	::DeleteObject(hfont);

	DeleteDC(cdc);

	return tSize.cx;
}

int GetTextSizeY(String text, String font, int size, int weight)
{
	if (text == NIL) return 0;

	int	 lines = 1;
	int	 txtsize = text.Length();

	for (int i = 0; i < txtsize; i++)
	{
		if (text[i] == 10) lines++;
	}

	return (lines * GetLineSizeY(text, font, size, weight)) + (lines - 1) * 3;
}

int GetLineSizeY(String text, String font, int size, int weight)
{
	if (text == NIL) return 0;

	HDC	 cdc = CreateCompatibleDC(0);

	size = -MulDiv(size, GetDeviceCaps(cdc, LOGPIXELSY), 72);

	HFONT	 hfont;
	HFONT	 holdfont;

	if (Setup::enableUnicode)	hfont = CreateFontW(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);
	else				hfont = CreateFontA(size, 0, 0, 0, weight, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, font);

	holdfont = (HFONT) SelectObject(cdc, hfont);

	SIZE	 tSize;

	if (Setup::enableUnicode)	GetTextExtentPoint32W(cdc, text, text.Length(), &tSize);
	else				GetTextExtentPoint32A(cdc, text, text.Length(), &tSize);

	SelectObject(cdc, holdfont);
	::DeleteObject(hfont);

	DeleteDC(cdc);

	return tSize.cy - 1;
}

bool DoRectsOverlap(Rect rect1, Rect rect2)
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

Rect OverlapRect(Rect rect1, Rect rect2)
{
	Rect orect;

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

HBITMAP BlitToBitmap(HDC dc, Rect rect)
{
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 newbmp;
	HBITMAP	 backup;

	newbmp = CreateCompatibleBitmap(dc, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
	backup = (HBITMAP) SelectObject(cdc, newbmp);

	BitBlt(cdc, 0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1, dc, rect.left, rect.top, SRCCOPY);

	newbmp = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	::DeleteObject(backup);

	return newbmp;
}

bool BlitToBitmap(Rect srcrect, HDC dc, Rect destrect, HBITMAP bmp)
{
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 backup;

	backup = (HBITMAP) SelectObject(cdc, bmp);

	StretchBlt(cdc, destrect.left, destrect.top, destrect.right - destrect.left + 1, destrect.bottom - destrect.top + 1, dc, srcrect.left, srcrect.top, srcrect.right - srcrect.left + 1, srcrect.bottom - srcrect.top + 1, SRCCOPY);

	bmp = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	::DeleteObject(backup);

	return true;
}

bool BlitFromBitmap(Rect srcrect, HBITMAP bitmap, Rect destrect, HDC dc)
{
	HDC	 cdc = CreateCompatibleDC(dc);
	HBITMAP	 backup;

	backup = (HBITMAP) SelectObject(cdc, bitmap);

	StretchBlt(dc, destrect.left, destrect.top, destrect.right - destrect.left + 1, destrect.bottom - destrect.top + 1, cdc, srcrect.left, srcrect.top, srcrect.right - srcrect.left + 1, srcrect.bottom - srcrect.top + 1, SRCCOPY);

	bitmap = (HBITMAP) SelectObject(cdc, backup);

	DeleteDC(cdc);
	::DeleteObject(backup);

	return true;
}
