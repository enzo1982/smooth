 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/hyperlink.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_HYPERLINK = S::Object::RequestObjectID();

S::GUI::Hyperlink::Hyperlink()
{
	type		= OBJ_HYPERLINK;
	linkURL		= NIL;
	linkBitmap	= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);
}

S::GUI::Hyperlink::Hyperlink(String text, HBITMAP bitmap, String link, Point pos, Size size)
{
	type			= OBJ_HYPERLINK;
	objectProperties->text	= text;
	linkURL			= link;
	linkBitmap		= DetectTransparentRegions(bitmap);

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (linkBitmap != NIL)
	{
		if (size.cx == 0 && size.cy == 0)
		{
			objectProperties->size.cx = Math::Round(GetBitmapSizeX(linkBitmap) * SMOOTH::Setup::FontSize);
			objectProperties->size.cy = Math::Round(GetBitmapSizeY(linkBitmap) * SMOOTH::Setup::FontSize);
		}
		else
		{
			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);
		}
	}
	else
	{
		GetTextSize();

		objectProperties->size.cx = objectProperties->textSize.cx;
		objectProperties->size.cy = objectProperties->textSize.cy;
	}
}

S::GUI::Hyperlink::~Hyperlink()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Hyperlink::Hide()
{
	if (!visible) return Success;

	visible = False;

	if (!registered) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 rect;
	Point	 realPos = GetRealPosition();

	if (linkBitmap == NIL) objectProperties->size = objectProperties->textSize;

	rect.left	= realPos.x;
	rect.top	= realPos.y;
	rect.right	= realPos.x + objectProperties->size.cx + 1;
	rect.bottom	= realPos.y + objectProperties->size.cy + 1;

	surface->Box(rect, SMOOTH::Setup::BackgroundColor, FILLED);

	return Success;
}

S::Int S::GUI::Hyperlink::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Rect	 textRect;
	Point	 realPos = GetRealPosition();

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

		if (SMOOTH::Setup::enableUnicode)	hfont = CreateFontW(objectProperties->fontSize, 0, 0, 0, objectProperties->fontWeight, 0, True, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, objectProperties->font);
		else					hfont = CreateFontA(objectProperties->fontSize, 0, 0, 0, objectProperties->fontWeight, 0, True, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, objectProperties->font);

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

	return Success;
}

S::Int S::GUI::Hyperlink::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

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

				retVal = Break;
			}
			break;
		case SM_MOUSELEAVE:
			if (!IsMouseOn(wnd->hwnd, textRect, WINDOW) && objectProperties->checked)
			{
				objectProperties->checked = False;

				wnd->cursorset = False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				Paint(SP_MOUSEOUT);
			}
			break;
		case SM_MOUSEMOVE:
			if (IsMouseOn(wnd->hwnd, textRect, WINDOW) && !objectProperties->checked)
			{
				objectProperties->checked = True;

				wnd->cursorset = True;

				LiSASetMouseCursor(LiSA_MOUSE_HAND);

				Paint(SP_MOUSEIN);
			}
			else if (!IsMouseOn(wnd->hwnd, textRect, WINDOW) && objectProperties->checked)
			{
				objectProperties->checked = False;

				wnd->cursorset = False;

				LiSASetMouseCursor(LiSA_MOUSE_ARROW);

				Paint(SP_MOUSEOUT);
			}
			break;
	}

	return retVal;
}

HBITMAP S::GUI::Hyperlink::GetBitmap()
{
	if (linkBitmap != NIL)	return linkBitmap;
	else			return NIL;
}

S::String S::GUI::Hyperlink::GetURL()
{
	return linkURL;
}

S::Int S::GUI::Hyperlink::SetText(String newText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	return Object::SetText(newText);
}

S::Int S::GUI::Hyperlink::SetURL(String newUrl)
{
	linkURL = newUrl;

	return Success;
}

S::Int S::GUI::Hyperlink::SetBitmap(HBITMAP newBmp)
{
	return Error;
}
