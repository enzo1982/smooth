 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/icon.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/init.win32.h>

S::GUI::ImageLoaderIcon::ImageLoaderIcon(const String &iFileName) : ImageLoader(iFileName)
{
}

S::GUI::ImageLoaderIcon::~ImageLoaderIcon()
{
}

const S::GUI::Bitmap &S::GUI::ImageLoaderIcon::Load()
{
	Int	 iconID = fileName.Tail(fileName.Length() - 5).ToInt();

	if (iconID == 0) return bitmap;

	/* Load icon resource.
	 */
	HICON	 icon = NIL;

	if	(iconID >= 32512 && iconID <= 32517) icon = (HICON) LoadImage(NIL,	    MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	else if (iconID >= 20000 && iconID <= 20999) icon = (HICON) LoadImage(hDllInstance, MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	else					     icon = (HICON) LoadImage(hInstance,    MAKEINTRESOURCE(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	if (icon == NIL) return bitmap;

	/* Draw icon to a 32 bit DIB section. We do this
	 * independently of the active graphics backend.
	 */
	Size		 size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));

	BITMAPINFO	 bmpInfo;

	ZeroMemory(&bmpInfo, sizeof(bmpInfo));

	bmpInfo.bmiHeader.biSize	= sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth	= size.cx;
	bmpInfo.bmiHeader.biHeight	= -size.cy;
	bmpInfo.bmiHeader.biPlanes	= 1;
	bmpInfo.bmiHeader.biBitCount	= 32;
	bmpInfo.bmiHeader.biCompression	= BI_RGB;

	UnsignedInt32	*pixels = NIL;
	HBITMAP		 dib	= CreateDIBSection(NIL, &bmpInfo, DIB_RGB_COLORS, (Void **) &pixels, NIL, 0);

	if (dib == NIL) return bitmap;

	HDC		 dc	= CreateCompatibleDC(NIL);
	HBITMAP		 backup	= (HBITMAP) SelectObject(dc, dib);
	Int		 count	= size.cx * size.cy;

	ZeroMemory(pixels, count * sizeof(UnsignedInt32));

	DrawIconEx(dc, 0, 0, icon, 0, 0, 0, NIL, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

	GdiFlush();

	/* Fix alpha channel for icons without one.
	 */
	Bool	 alphaSet = False;

	for (Int i = 0; i < count && !alphaSet; i++) if (pixels[i] >> 24 != 0) alphaSet = True;

	if (!alphaSet)
	{
		Color		 background = Setup::BackgroundColor;
		UnsignedInt32	 fill	    = background.GetRed() << 16 | background.GetGreen() << 8 | background.GetBlue();

		for (Int i = 0; i < count; i++) pixels[i] = fill;

		DrawIconEx(dc, 0, 0, icon, 0, 0, 0, NIL, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

		GdiFlush();

		for (Int i = 0; i < count; i++) pixels[i] |= 0xFF000000;
	}

	/* Copy pixels to the target bitmap.
	 */
	bitmap.CreateBitmap(size, 32);

	for (Int y = 0; y < size.cy; y++)
	{
		for (Int x = 0; x < size.cx; x++)
		{
			UnsignedInt32	 pixel = pixels[y * size.cx + x];

			bitmap.SetPixel(Point(x, y), Color((pixel & 0xFF000000) | (pixel & 0xFF) << 16 | (pixel & 0xFF00) | (pixel >> 16 & 0xFF), Color::RGBA));
		}
	}

	/* Clean up.
	 */
	SelectObject(dc, backup);
	DeleteDC(dc);
	DeleteObject(dib);

	return bitmap;
}
