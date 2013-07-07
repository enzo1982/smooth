 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

	if (iconID != 0)
	{
		HICON	 icon = NIL;

		if (iconID >= 32512 && iconID <= 32517)
		{
			if (Setup::enableUnicode) icon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else			  icon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}
		else if (iconID >= 20000 && iconID <= 20999)
		{
			if (Setup::enableUnicode) icon = (HICON) LoadImageW(hDllInstance, MAKEINTRESOURCEW(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else			  icon = (HICON) LoadImageA(hDllInstance, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}
		else
		{
			if (Setup::enableUnicode) icon = (HICON) LoadImageW(hInstance, MAKEINTRESOURCEW(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else			  icon = (HICON) LoadImageA(hInstance, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}

		Size	 size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));

		bitmap.CreateBitmap(size.cx, size.cy, 32);

		for (Int x = 0; x < size.cx; x++)
		{
			for (Int y = 0; y < size.cy; y++)
			{
				bitmap.SetPixel(Point(x, y), Setup::BackgroundColor);
			}
		}

		HDC	 dc = CreateCompatibleDC(NIL);
		HBITMAP	 backup = (HBITMAP) SelectObject(dc, bitmap.GetSystemBitmap());

		DrawIconEx(dc, 0, 0, icon, 0, 0, 0, NIL, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

		bitmap.SetSystemBitmap((HBITMAP) SelectObject(dc, backup));

		DeleteDC(dc);
	}

	return bitmap;
}
