 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_STK_
#define __OBJSMOOTH_STK_

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
#include <smooth/divisionbar.h>

#include <pciio.h>

SMOOTHInt SMOOTH::nextID = 0;
SMOOTHInt SMOOTH::nextGUID = 0;

SMOOTHInt SMOOTH::Setup::BackgroundColor = RGB(192, 192, 192);
SMOOTHInt SMOOTH::Setup::ClientColor = RGB(255, 255, 255);
SMOOTHInt SMOOTH::Setup::ClientTextColor = RGB(0, 0, 0);
SMOOTHInt SMOOTH::Setup::LightGrayColor;
SMOOTHInt SMOOTH::Setup::DividerLightColor;
SMOOTHInt SMOOTH::Setup::DividerDarkColor;
SMOOTHInt SMOOTH::Setup::TextColor = RGB(0, 0, 0);
SMOOTHInt SMOOTH::Setup::GrayTextColor = RGB(128, 128, 128);
SMOOTHInt SMOOTH::Setup::ShadowColor;
SMOOTHInt SMOOTH::Setup::GradientStartColor;
SMOOTHInt SMOOTH::Setup::GradientEndColor;
SMOOTHInt SMOOTH::Setup::GradientTextColor = RGB(255, 255, 255);
SMOOTHInt SMOOTH::Setup::InactiveGradientStartColor;
SMOOTHInt SMOOTH::Setup::InactiveGradientEndColor;
SMOOTHInt SMOOTH::Setup::InactiveGradientTextColor;
SMOOTHInt SMOOTH::Setup::TooltipColor;
SMOOTHInt SMOOTH::Setup::TooltipTextColor;

SMOOTHBool SMOOTH::Setup::enableUnicode = SMOOTH::False;

SMOOTHFloat SMOOTH::Setup::FontSize = 1.00;

SMOOTHInt SMOOTH::Success	= 1;
SMOOTHInt SMOOTH::Error		= 0;
SMOOTHInt SMOOTH::Break		= -1;

SMOOTHBool SMOOTH::True		= true;
SMOOTHBool SMOOTH::False	= false;

SMOOTHString SMOOTH::StartDirectory = NIL;

SMOOTHVoid SMOOTH::CloseWindow(SMOOTHWindow *wnd)
{
	if (wnd == NIL) return;

#ifdef __WIN32__
	if (wnd->hwnd != NIL)
	{
		if (SMOOTH::Setup::enableUnicode)	::SendMessageW(wnd->hwnd, WM_CLOSE, 0, 0);
		else					::SendMessageA(wnd->hwnd, WM_CLOSE, 0, 0);
	}
#endif
}

SMOOTHObject *SMOOTH::GetObject(SMOOTHInt objectHandle, SMOOTHInt objectType)
{
	SMOOTHObject	*retVal = mainObjectManager->RequestObject(objectHandle);

	if (retVal == NIL) return NIL;

	if (retVal->GetObjectType() == objectType)	return retVal;
	else						return NIL;
}

SMOOTHWindow *SMOOTH::GetWindow(HWND wnd)
{
	SMOOTHWindow	*rval;

	for (int i = 0; i < SMOOTHObject::objectCount; i++)
	{
		rval = (SMOOTHWindow *) mainObjectManager->RequestObject(i);

		if (rval != (SMOOTHWindow *) NIL)
		{
			if (rval->type == OBJ_WINDOW || rval->type == OBJ_MDIWINDOW || rval->type == OBJ_TOOLWINDOW)
			{
				if (rval->hwnd == wnd) return rval;
			}
		}
	}

	return NIL;
}

SMOOTHInt SMOOTH::DeleteObject(SMOOTHObject *object)
{
	if (object != NIL)
	{
		if (!object->IsObjectInUse())
		{
			delete object;

			return SMOOTH::Success;
		}

		return object->DeleteObject();
	}

	return SMOOTH::Error;
}

SMOOTHBool SMOOTH::SetStartDirectory(SMOOTHString dir)
{
	int	 len = dir.Length() - 1;

	if (dir[len] != '\\') dir[++len] = '\\';

	SMOOTH::StartDirectory = dir;

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTH::SendMessage(SMOOTHWindow *window, SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (window != NIL)
	{
		window->Process(message, wParam, lParam);
	}
	else
	{
		for (SMOOTHInt i = 0; i < SMOOTHObject::objectCount; i++)
		{
			SMOOTHObject	*object = mainObjectManager->RequestObject(i);

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

SMOOTHBool Affected(SMOOTHObject *obj, SMOOTHRect &urect)
{
	SMOOTHRect	 trect;
	SMOOTHPoint	 realpos = obj->GetObjectProperties()->pos;

	if (obj->GetObjectType() == OBJ_LAYER || obj->GetObjectType() == OBJ_DIVISIONBAR) return true;

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

HBITMAP SMOOTH::LoadImage(SMOOTHString file, SMOOTHInt id, SMOOTHString name)
{
	HBITMAP		 bmp;
	PCIIn		 pci = OpenPCIForInput((char *) file);
	SMOOTHString	 startdir = SMOOTH::StartDirectory;

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

HBITMAP GrayscaleBitmap(HBITMAP bmp)
{
	if (bmp == NIL) return NIL;

	int		 isx = GetBitmapSizeX(bmp);
	int		 isy = GetBitmapSizeY(bmp);
	HDC		 dc = GetContext(0);
	HDC		 cdc = CreateCompatibleContext(dc, SMOOTHSize(isx, isy));
	HBITMAP		 newbmp;
	int		 col = 0;

	BlitFromBitmap(SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)), bmp, SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)), cdc);

	for (int y = 0; y < isx; y++)
	{
		for (int x = 0; x < isy; x++)
		{
			col = GetPixel(cdc, SMOOTHPoint(x, y));
			col = (GetRed(col) + GetGreen(col) + GetBlue(col)) / 3;
			col = RGB(col, col, col);
			PaintPixel(cdc, SMOOTHPoint(x, y), col);
		}
	}

	newbmp = BlitToBitmap(cdc, SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)));

	FreeCompatibleContext(cdc);
	FreeContext(0, dc);

	return newbmp;
}

HBITMAP DetectTransparentRegions(HBITMAP bmp)
{
	if (bmp == NIL) return NIL;

	int		 isx = GetBitmapSizeX(bmp);
	int		 isy = GetBitmapSizeY(bmp);
	HDC		 dc = GetContext(0);
	HDC		 cdc = CreateCompatibleContext(dc, SMOOTHSize(isx, isy));
	HBITMAP		 newbmp;

	BlitFromBitmap(SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)), bmp, SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)), cdc);

	for (int y = 0; y < isx; y++)
	{
		for (int x = 0; x < isy; x++)
		{
			if (GetPixel(cdc, SMOOTHPoint(x, y)) == RGB(192, 192, 192)) PaintPixel(cdc, SMOOTHPoint(x, y), SMOOTH::Setup::BackgroundColor);
		}
	}

	newbmp = BlitToBitmap(cdc, SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(isx - 1, isy - 1)));

	FreeCompatibleContext(cdc);
	FreeContext(0, dc);

	return newbmp;
}

SMOOTHString SMOOTH::GetVersionString()
{
	return SMOOTH_VERSION;
}

SMOOTHInt SMOOTH::RequestObjectID()
{
	return nextID++;
}

SMOOTHInt SMOOTH::RequestObjectHandle()
{
	return 	SMOOTHObject::objectCount++;
}

SMOOTHInt SMOOTH::RequestGUID()
{
	return nextGUID++;
}

#endif
