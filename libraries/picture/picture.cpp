 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

// this class is to be deprecated by smooth::Bitmap in the near future

#include <smooth/definitions.h>
#include <picture.h>
#include <smooth/toolkit.h>

picture::picture(bool toHDC)
{
	bmPic		= NIL;
	bmOld		= NIL;
	hdc		= NIL;
	width		= 0;
	height		= 0;
	keepHDC		= toHDC;
	bytes		= NIL;
	bytewidth	= 0;
}

picture::picture(unsigned long cx, unsigned long cy, unsigned char bpp, bool toHDC)
{
	bmPic		= NIL;
	bmOld		= NIL;
	hdc		= NIL;
	width		= 0;
	height		= 0;
	keepHDC		= toHDC;
	bytes		= NIL;
	bytewidth	= 0;

	CreateBitmap(cx, cy, bpp, toHDC);
}

picture::~picture()
{
	DeleteBitmap();
}

bool picture::DeleteBitmap()
{
	int ret = 0;

	if (hdc)
	{
		::SelectObject(hdc, bmOld);
		ret += ::DeleteDC(hdc);
	}

	if(bmPic) ret += ::DeleteObject(bmPic);

	hdc = NIL;
	width = 0;
	height = 0;
	bmPic = NIL;
	bytewidth = 0;
	bytes = NIL;

	if (ret)	return true;
	else		return false;
}

bool picture::CreateBitmap(unsigned long cx, unsigned long cy, unsigned char bpp, bool toHDC)
{
	BITMAP		 bm;
	BITMAPINFO	*pBMI;
	DWORD		*pBmBits;

	DeleteBitmap();

	int		 iPalSize = (bpp == 8) ? 256 : 1;
	unsigned char	*buf = new unsigned char[sizeof(BITMAPINFOHEADER) + iPalSize * sizeof(RGBQUAD)];

	if (!buf) return false;

	BITMAPINFOHEADER	&bmih = *(BITMAPINFOHEADER *) buf;

	ZeroMemory(buf, sizeof(BITMAPINFOHEADER) + iPalSize * sizeof(RGBQUAD));

	bmih.biSize		= sizeof(BITMAPINFOHEADER);
	bmih.biWidth		= cx;
	bmih.biHeight		= cy;
	bmih.biPlanes		= 1;
	bmih.biBitCount		= bpp;
	bmih.biCompression	= BI_RGB;
	pBMI			= (BITMAPINFO *) buf;

	HBITMAP hbm = ::CreateDIBSection(NIL, pBMI, DIB_RGB_COLORS, (void **) &pBmBits, NIL, 0);

	delete buf; 

	if (!hbm) return false;

	if (::GetObjectA(hbm, sizeof(bm), &bm) != sizeof(bm)) return false;

	bmPic = hbm;

	width = bm.bmWidth;
	height = bm.bmHeight;

	bytes = (unsigned char *) (bm.bmBits);
	bytewidth = bm.bmWidthBytes;

	if (toHDC)
	{
		HDC dc = ::GetWindowDC(NIL); 
		hdc = ::CreateCompatibleDC(dc);
		bmOld = (HBITMAP) ::SelectObject(hdc, bmPic);
		::ReleaseDC(NIL, dc);
		keepHDC = true;
	}

	return true;
}

bool picture::BlitToDC(HDC dc, RECT *rcTrg, RECT *rcSrc)
{
	int retcode = 0;
	RECT rc = { 0, 0, width, height }; 
	SIZE szS;
	SIZE szT;

	if (bmPic)
	{
		if (!rcSrc) rcSrc = &rc;
		if (!rcTrg) rcTrg = rcSrc;

		szS.cx = rcSrc->right-rcSrc->left;
		szS.cy = rcSrc->bottom-rcSrc->top;
		szT.cx = rcTrg->right-rcTrg->left;
		szT.cy = rcTrg->bottom-rcTrg->top;

		HBITMAP	 hbm = NIL;
		HDC	 memdc = hdc;

		if (!keepHDC)
		{
			memdc = ::CreateCompatibleDC(dc);
			hbm = (HBITMAP) ::SelectObject(memdc, bmPic);
		}

		if (szT.cx == szS.cx && szT.cy == szS.cy) retcode = ::BitBlt(dc, rcTrg->left, rcTrg->top, szT.cx, szT.cy, memdc, rcSrc->left, rcSrc->top, SRCCOPY);
		else
		{
			::SetStretchBltMode(dc, COLORONCOLOR);
			retcode = ::StretchBlt(dc, rcTrg->left, rcTrg->top, szT.cx, szT.cy, memdc, rcSrc->left, rcSrc->top, szS.cx, szS.cy, SRCCOPY);
		}

		if (!keepHDC)
		{
			::SelectObject(memdc, hbm);
			::DeleteDC(memdc);
		}
	}

	if (retcode)	return true;
	else		return false;
}

bool picture::BlitToDC(picture *pic, RECT *rcTrg, RECT *rcSrc)
{
	if (pic->hdc == NIL) return false;

	return BlitToDC(pic->hdc, rcTrg, rcSrc);
}

bool picture::BlitFromDC(HDC dc, RECT *rcSrc, RECT *rcTrg)
{
	RECT rc = { 0, 0, width, height }; 

	if (!keepHDC) return false;

	bool	 retcode = false;
	HDC	 hdcTEMP = hdc;

	hdc = dc;

	if (!rcTrg) rcTrg = &rc;

	retcode	= BlitToDC(hdcTEMP, rcTrg, rcSrc);
	hdc	= hdcTEMP;

	return retcode;
}
