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
#include <smooth/mdiwindow.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/divider.h>
#include <smooth/pciio.h>

S::Int S::SMOOTH::Setup::BackgroundColor = RGB(192, 192, 192);
S::Int S::SMOOTH::Setup::ClientColor = RGB(255, 255, 255);
S::Int S::SMOOTH::Setup::ClientTextColor = RGB(0, 0, 0);
S::Int S::SMOOTH::Setup::LightGrayColor;
S::Int S::SMOOTH::Setup::DividerLightColor;
S::Int S::SMOOTH::Setup::DividerDarkColor;
S::Int S::SMOOTH::Setup::TextColor = RGB(0, 0, 0);
S::Int S::SMOOTH::Setup::GrayTextColor = RGB(128, 128, 128);
S::Int S::SMOOTH::Setup::ShadowColor;
S::Int S::SMOOTH::Setup::GradientStartColor;
S::Int S::SMOOTH::Setup::GradientEndColor;
S::Int S::SMOOTH::Setup::GradientTextColor = RGB(255, 255, 255);
S::Int S::SMOOTH::Setup::InactiveGradientStartColor;
S::Int S::SMOOTH::Setup::InactiveGradientEndColor;
S::Int S::SMOOTH::Setup::InactiveGradientTextColor;
S::Int S::SMOOTH::Setup::TooltipColor;
S::Int S::SMOOTH::Setup::TooltipTextColor;

S::Bool S::SMOOTH::Setup::enableUnicode = S::False;

S::Float S::SMOOTH::Setup::FontSize = 1.00;

S::String S::SMOOTH::StartDirectory = NIL;

S::Object *S::SMOOTH::GetObject(Int objectHandle, Int objectType)
{
	Object	*retVal = mainObjectManager->RequestObject(objectHandle);

	if (retVal == NIL) return NIL;

	if (retVal->GetObjectType() == objectType)	return retVal;
	else						return NIL;
}

S::Window *S::SMOOTH::GetWindow(HWND wnd)
{
	Window	*rval;

	for (int i = 0; i < Object::objectCount; i++)
	{
		rval = (Window *) mainObjectManager->RequestObject(i);

		if (rval != (Window *) NIL)
		{
			if (rval->type == OBJ_WINDOW || rval->type == OBJ_MDIWINDOW || rval->type == OBJ_TOOLWINDOW)
			{
				if (rval->hwnd == wnd) return rval;
			}
		}
	}

	return NIL;
}

S::Bool S::SMOOTH::SetStartDirectory(String dir)
{
	int	 len = dir.Length() - 1;

	if (dir[len] != '\\') dir[++len] = '\\';

	SMOOTH::StartDirectory = dir;

	return Success;
}

S::Void S::SMOOTH::SendMessage(Window *window, Int message, Int wParam, Int lParam)
{
	if (window != NIL)
	{
		window->Process(message, wParam, lParam);
	}
	else
	{
		for (Int i = 0; i < Object::objectCount; i++)
		{
			Object	*object = mainObjectManager->RequestObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == OBJ_WINDOW || object->GetObjectType() == OBJ_MDIWINDOW || object->GetObjectType() == OBJ_TOOLWINDOW)
				{
					object->Process(message, wParam, lParam);
				}
			}
		}
	}
}

S::Bool S::Affected(Object *obj, Rect &urect)
{
	Rect	 trect;
	Point	 realpos = obj->GetObjectProperties()->pos;

	if (obj->GetObjectType() == OBJ_LAYER || obj->GetObjectType() == OBJ_DIVIDER) return true;

	if (obj->GetObjectProperties()->pos.x == 0 && obj->GetObjectProperties()->pos.y == 0 && obj->GetObjectProperties()->size.cx == 0 && obj->GetObjectProperties()->size.cy == 0) return true;

	if (obj->GetContainer() != NIL)
	{
		if (obj->GetContainer()->GetContainerObject()->GetObjectType() == OBJ_LAYER) realpos = obj->GetRealPosition();
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
	String	 startdir = SMOOTH::StartDirectory;

	if (pci->GetLastError() != IOLIB_ERROR_OK)
	{
		ClosePCI(pci);

		pci = OpenPCIForInput((char *) startdir.Append(file));

		if (pci->GetLastError() != IOLIB_ERROR_OK)
		{
			ClosePCI(pci);

			return NIL;
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
			col = GetPixel(cdc, Point(x, y));
			col = (GetRed(col) + GetGreen(col) + GetBlue(col)) / 3;
			col = RGB(col, col, col);
			PaintPixel(cdc, Point(x, y), col);
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
			if (GetPixel(cdc, Point(x, y)) == RGB(192, 192, 192)) PaintPixel(cdc, Point(x, y), SMOOTH::Setup::BackgroundColor);
		}
	}

	newbmp = BlitToBitmap(cdc, Rect(Point(0, 0), Size(isx - 1, isy - 1)));

	FreeCompatibleContext(cdc);
	FreeContext(0, dc);

	return newbmp;
}
