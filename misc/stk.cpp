 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/application.h>
#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/definitions.h>
#include <smooth/objectmanager.h>
#include <smooth/stk.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/version.h>
#include <smooth/loop.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/divider.h>
#include <smooth/pciio.h>
#include <smooth/mdiwindow.h>

S::Void S::SMOOTH::SendMessage(GUI::Window *window, Int message, Int wParam, Int lParam)
{
	if (window != NIL)
	{
		window->Process(message, wParam, lParam);
	}
	else
	{
		for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
		{
			Object	*object = mainObjectManager->GetNthObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == GUI::Window::classID || object->GetObjectType() == GUI::MDIWindow::classID || object->GetObjectType() == GUI::ToolWindow::classID)
				{
					((GUI::Window *) object)->Process(message, wParam, lParam);
				}
			}
		}
	}
}

S::Bool S::Affected(GUI::Widget *obj, Rect &urect)
{
	Rect	 trect;
	Point	 realpos = obj->GetObjectProperties()->pos;

	if (obj->GetObjectType() == GUI::Layer::classID || obj->GetObjectType() == GUI::Divider::classID) return true;

	if (obj->GetObjectProperties()->pos.x == 0 && obj->GetObjectProperties()->pos.y == 0 && obj->GetObjectProperties()->size.cx == 0 && obj->GetObjectProperties()->size.cy == 0) return true;

	if (obj->GetContainer() != NIL)
	{
		if (obj->GetContainer()->GetContainerObject()->GetObjectType() == GUI::Layer::classID) realpos = obj->GetRealPosition();
	}

	trect.left	= realpos.x - 10;
	trect.top	= realpos.y - 10;
	trect.right	= realpos.x + obj->GetObjectProperties()->size.cx + 10;
	trect.bottom	= realpos.y + obj->GetObjectProperties()->size.cy + 10;

	if (!DoRectsOverlap(urect, trect)) return false;

	return true;
}

HBITMAP S::SMOOTH::LoadImage(String file, Int id, String name)
{
	HBITMAP	 bmp;
	PCIIn	 pci = OpenPCIForInput((char *) file);

	if (pci->GetLastError() != IOLIB_ERROR_OK)
	{
		ClosePCI(pci);

		pci = OpenPCIForInput((char *) Application::GetStartupDirectory().Append(file));

		if (pci->GetLastError() != IOLIB_ERROR_OK)
		{
			ClosePCI(pci);

			pci = OpenPCIForInput((char *) Application::GetApplicationDirectory().Append(file));

			if (pci->GetLastError() != IOLIB_ERROR_OK)
			{
				ClosePCI(pci);

				return NIL;
			}
		}
	}

	PCIIO	*pio = new PCIIO();

	if (id >= 0)		pio->SelectImage(id);
	else if (name != NIL)	pio->SelectImage((char *) name);

	ReadPCI(pci, *pio);

	ClosePCI(pci);

	bmp = pio->GetBitmap();

	delete pio;

	if (bmp == NIL) return NIL;

	return bmp;
}

HBITMAP S::GrayscaleBitmap(HBITMAP bmp)
{
	if (bmp == NIL) return NIL;

	int	 isx = GetBitmapSizeX(bmp);
	int	 isy = GetBitmapSizeY(bmp);
	HDC	 dc = GetContext(0);
	HDC	 cdc = CreateCompatibleContext(dc, Size(isx, isy));
	HBITMAP	 newbmp;
	int	 col = 0;

	BlitFromBitmap(Rect(Point(0, 0), Size(isx - 1, isy - 1)), bmp, Rect(Point(0, 0), Size(isx - 1, isy - 1)), cdc);

	for (int y = 0; y < isx; y++)
	{
		for (int x = 0; x < isy; x++)
		{
			col = GetPixel(cdc, x, y);
			col = (GetRed(col) + GetGreen(col) + GetBlue(col)) / 3;
			col = RGB(col, col, col);
			SetPixel(cdc, x, y, col);
		}
	}

	newbmp = BlitToBitmap(cdc, Rect(Point(0, 0), Size(isx - 1, isy - 1)));

	FreeCompatibleContext(cdc);
	FreeContext(0, dc);

	return newbmp;
}

HBITMAP S::DetectTransparentRegions(HBITMAP bmp)
{
	if (bmp == NIL) return NIL;

	int		 isx = GetBitmapSizeX(bmp);
	int		 isy = GetBitmapSizeY(bmp);
	HDC		 dc = GetContext(0);
	HDC		 cdc = CreateCompatibleContext(dc, Size(isx, isy));
	HBITMAP		 newbmp;

	BlitFromBitmap(Rect(Point(0, 0), Size(isx - 1, isy - 1)), bmp, Rect(Point(0, 0), Size(isx - 1, isy - 1)), cdc);

	for (int y = 0; y < isx; y++)
	{
		for (int x = 0; x < isy; x++)
		{
			if (GetPixel(cdc, x, y) == RGB(192, 192, 192)) SetPixel(cdc, x, y, Setup::BackgroundColor);
		}
	}

	newbmp = BlitToBitmap(cdc, Rect(Point(0, 0), Size(isx - 1, isy - 1)));

	FreeCompatibleContext(cdc);
	FreeContext(0, dc);

	return newbmp;
}
