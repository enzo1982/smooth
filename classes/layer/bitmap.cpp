 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BITMAP_
#define __OBJSMOOTH_BITMAP_

#include <smooth/bitmap.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_HBITMAP = SMOOTH::RequestObjectID();

SMOOTHBitmap::SMOOTHBitmap(HBITMAP bmp, SMOOTHPoint pos, SMOOTHSize size)
{
	type	= OBJ_HBITMAP;
	bitmap	= bmp;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0 && size.cy == 0)
	{
		objectProperties->size.cx = roundtoint(GetBitmapSizeX(bitmap) * SMOOTH::Setup::FontSize);
		objectProperties->size.cy = roundtoint(GetBitmapSizeY(bitmap) * SMOOTH::Setup::FontSize);
	}
	else
	{
		objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
}

SMOOTHBitmap::~SMOOTHBitmap()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHBitmap::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 bmpRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	bmpRect.left	= realPos.x;
	bmpRect.top	= realPos.y;
	bmpRect.right	= realPos.x + objectProperties->size.cx;
	bmpRect.bottom	= realPos.y + objectProperties->size.cy;

	PaintBitmap(dc, bmpRect, bitmap);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHBitmap::SetBitmap(HBITMAP newBmp)
{
	SMOOTHBool	 prevVisible = visible;

	if (visible) Hide();

	bitmap = newBmp;

	if (objectProperties->size.cx == 0 && objectProperties->size.cy == 0)
	{
		objectProperties->size.cx = roundtoint(GetBitmapSizeX(bitmap) * SMOOTH::Setup::FontSize);
		objectProperties->size.cy = roundtoint(GetBitmapSizeY(bitmap) * SMOOTH::Setup::FontSize);
	}

	if (prevVisible) Show();

	return SMOOTH::Success;
}

HBITMAP SMOOTHBitmap::GetBitmap()
{
	return bitmap;
}

#endif
