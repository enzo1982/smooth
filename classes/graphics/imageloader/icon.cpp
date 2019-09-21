 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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

	/* Create bitmap and draw icon.
	 */
	Size	 size(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));

	bitmap.CreateBitmap(size, 32);

	HDC	 dc = CreateCompatibleDC(NIL);
	HBITMAP	 backup = (HBITMAP) SelectObject(dc, bitmap.GetSystemBitmap());

	DrawIconEx(dc, 0, 0, icon, 0, 0, 0, NIL, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

	/* Fix alpha channel for 8 bit icons.
	 */
	if (!IsAlphaChannelSet())
	{
		SetBackgroundColor(Setup::BackgroundColor);

		DrawIconEx(dc, 0, 0, icon, 0, 0, 0, NIL, DI_DEFAULTSIZE | DI_IMAGE | DI_MASK);

		SetAlphaChannel(255);
	}

	/* Clean up.
	 */
	SelectObject(dc, backup);
	DeleteDC(dc);

	return bitmap;
}

S::Bool S::GUI::ImageLoaderIcon::IsAlphaChannelSet() const
{
	Size	 size = bitmap.GetSize();

	for (Int x = 0; x < size.cx; x++)
	{
		for (Int y = 0; y < size.cy; y++)
		{
			if (bitmap.GetPixel(Point(x, y)).GetAlpha() != 0) return True;
		}
	}

	return False;
}

S::Void S::GUI::ImageLoaderIcon::SetBackgroundColor(const Color &color)
{
	Size	 size = bitmap.GetSize();

	for (Int x = 0; x < size.cx; x++)
	{
		for (Int y = 0; y < size.cy; y++)
		{
			bitmap.SetPixel(Point(x, y), color);
		}
	}
}

S::Void S::GUI::ImageLoaderIcon::SetAlphaChannel(Int value)
{
	Size	 size = bitmap.GetSize();

	for (Int x = 0; x < size.cx; x++)
	{
		for (Int y = 0; y < size.cy; y++)
		{
			bitmap.SetPixel(Point(x, y), Color(value << 24 | bitmap.GetPixel(Point(x, y)), Color::RGBA));
		}
	}
}
