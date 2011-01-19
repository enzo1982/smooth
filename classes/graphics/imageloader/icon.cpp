 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/imageloader/icon.h>
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
			if (Setup::enableUnicode)	icon = (HICON) LoadImageW(NIL, MAKEINTRESOURCEW(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else				icon = (HICON) LoadImageA(NIL, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}
		else
		{
			if (Setup::enableUnicode)	icon = (HICON) LoadImageW(hInstance, MAKEINTRESOURCEW(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
			else				icon = (HICON) LoadImageA(hInstance, MAKEINTRESOURCEA(iconID), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS | LR_SHARED);
		}

		Bitmap	 nBitmap(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 24);

		HDC	 dc = CreateCompatibleDC(NIL);
		HBRUSH	 brush = CreateSolidBrush(Setup::BackgroundColor);
		HBITMAP	 backup = (HBITMAP) SelectObject(dc, nBitmap.GetSystemBitmap());

		DrawIconEx(dc, 0, 0, icon, 0, 0, 0, brush, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

		bitmap.SetSystemBitmap((HBITMAP) SelectObject(dc, backup));

		::DeleteObject(brush);

		DeleteDC(dc);
	}

	return bitmap;
}
