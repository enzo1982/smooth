 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_HYPERLINK_
#define __OBJSMOOTH_HYPERLINK_

#include <smooth/hyperlink.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_HYPERLINK = SMOOTH::RequestObjectID();

SMOOTHHyperlink::SMOOTHHyperlink()
{
	type		= OBJ_HYPERLINK;
	linkURL		= NIL;
	linkBitmap	= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);
}

SMOOTHHyperlink::SMOOTHHyperlink(SMOOTHString text, HBITMAP bitmap, SMOOTHString link, SMOOTHPoint pos, SMOOTHSize size)
{
	type			= OBJ_HYPERLINK;
	objectProperties->text	= text;
	linkURL			= link;
	linkBitmap		= DetectTransparentRegions(bitmap);

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (linkBitmap != NIL)
	{
		if (size.cx == 0 && size.cy == 0)
		{
			objectProperties->size.cx = roundtoint(GetBitmapSizeX(linkBitmap) * SMOOTH::Setup::FontSize);
			objectProperties->size.cy = roundtoint(GetBitmapSizeY(linkBitmap) * SMOOTH::Setup::FontSize);
		}
		else
		{
			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
		}
	}
	else
	{
		GetTextSize();

		objectProperties->size.cx = objectProperties->textSize.cx;
		objectProperties->size.cy = objectProperties->textSize.cy;
	}
}

SMOOTHHyperlink::~SMOOTHHyperlink()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHHyperlink::Hide()
{
	if (!visible) return SMOOTH::Success;

	visible = SMOOTH::False;

	if (!registered) return SMOOTH::Success;

	SMOOTHSurface	*surface = myContainer->GetDrawSurface();
	SMOOTHRect	 rect;
	SMOOTHPoint	 realPos = GetRealPosition();

	if (linkBitmap == NIL) objectProperties->size = objectProperties->textSize;

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHHyperlink::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();

#ifdef __WIN32__
	HFONT		 hfont;
	HFONT		 holdfont;
#endif

	textRect.left	= realPos.x;
	textRect.top	= realPos.y;

	if (linkBitmap == NIL)
	{
		textRect.right	= textRect.left + objectProperties->textSize.cx;
		textRect.bottom	= textRect.top + objectProperties->textSize.cy + 1;

#ifdef __WIN32__
		RECT	 trect = textRect;

		SetBkMode(dc, TRANSPARENT);

		switch (message)
		{
			default:
			case SP_PAINT:
			case SP_MOUSEOUT:
				SetTextColor(dc, RGB(0, 0, 255));
				break;
			case SP_MOUSEIN:
				SetTextColor(dc, RGB(0, 128, 255));
				break;
		}

		if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(objectProperties->fontSize, 0, 0, 0, objectProperties->fontWeight, 0, SMOOTH::True, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, objectProperties->font);
		else					hfont = CreateFontA(objectProperties->fontSize, 0, 0, 0, objectProperties->fontWeight, 0, SMOOTH::True, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, objectProperties->font);

		holdfont = (HFONT) SelectObject(dc, hfont);

		if (SMOOTH::Setup::enableUnicode)	DrawTextW(dc, objectProperties->text, -1, &trect, DT_LEFT);
		else					DrawTextA(dc, objectProperties->text, -1, &trect, DT_LEFT);

		SelectObject(dc, holdfont);
		::DeleteObject(hfont);
#endif
	}
	else
	{
		textRect.right	= textRect.left + objectProperties->size.cx;
		textRect.bottom	= textRect.top + objectProperties->size.cy;

		PaintBitmap(dc, textRect, linkBitmap);
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHHyperlink::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHRect	 textRect;
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;

	textRect.left	= realPos.x;
	textRect.top	= realPos.y;

	if (linkBitmap == NIL)
	{
		textRect.right	= textRect.left + objectProperties->textSize.cx;
		textRect.bottom	= textRect.top + objectProperties->textSize.cy + 1;
	}
	else
	{
		textRect.right	= textRect.left + objectProperties->size.cx - 1;
		textRect.bottom	= textRect.top + objectProperties->size.cy - 1;
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (objectProperties->checked)
			{
				if (SMOOTH::Setup::enableUnicode)	LiSAOpenURLW(linkURL);
				else					LiSAOpenURLA(linkURL);

				wnd->Process(SM_MOUSEMOVE, 0, 0);

				retVal = SMOOTH::Break;
			}
			break;
		case SM_MOUSELEAVE:
			if (!IsMouseOn(wnd->hwnd, textRect, WINDOW) && objectProperties->checked)
			{
				objectProperties->checked = SMOOTH::False;

				wnd->cursorset = SMOOTH::False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				Paint(SP_MOUSEOUT);
			}
			break;
		case SM_MOUSEMOVE:
			if (IsMouseOn(wnd->hwnd, textRect, WINDOW) && !objectProperties->checked)
			{
				objectProperties->checked = SMOOTH::True;

				wnd->cursorset = SMOOTH::True;

				LiSASetMouseCursor(LiSA_MOUSE_HAND);

				Paint(SP_MOUSEIN);
			}
			else if (!IsMouseOn(wnd->hwnd, textRect, WINDOW) && objectProperties->checked)
			{
				objectProperties->checked = SMOOTH::False;

				wnd->cursorset = SMOOTH::False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				Paint(SP_MOUSEOUT);
			}
			break;
	}

	return retVal;
}

HBITMAP SMOOTHHyperlink::GetBitmap()
{
	if (linkBitmap != NIL)	return linkBitmap;
	else			return NIL;
}

SMOOTHString SMOOTHHyperlink::GetURL()
{
	return linkURL;
}

SMOOTHInt SMOOTHHyperlink::SetText(SMOOTHString newText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	return SMOOTHObject::SetText(newText);
}

SMOOTHInt SMOOTHHyperlink::SetURL(SMOOTHString newUrl)
{
	linkURL = newUrl;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHHyperlink::SetBitmap(HBITMAP newBmp)
{
	return SMOOTH::Error;
}

#endif
