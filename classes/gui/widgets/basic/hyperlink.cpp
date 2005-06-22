 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/hyperlink.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/color.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Hyperlink::classID = S::Object::RequestClassID();

S::GUI::Hyperlink::Hyperlink()
{
	type		= classID;
	linkURL		= NIL;
	linkBitmap	= NIL;

	font.SetUnderline(True);

	possibleContainers.AddEntry(Layer::classID);
}

S::GUI::Hyperlink::Hyperlink(String iText, const Bitmap &bitmap, String link, Point iPos, Size iSize)
{
	type		= classID;
	text		= iText;
	linkURL		= link;
	linkBitmap	= bitmap;

	linkBitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	font.SetUnderline(True);

	possibleContainers.AddEntry(Layer::classID);

	pos	= iPos;
	size	= iSize;

	if (linkBitmap != NIL)
	{
		if (size.cx == 0 && size.cy == 0) size = linkBitmap.GetSize();
	}
	else
	{
		GetTextSize();

		size = textSize;
	}
}

S::GUI::Hyperlink::~Hyperlink()
{
}

S::Int S::GUI::Hyperlink::Hide()
{
	if (linkBitmap == NIL) size = textSize;

	return Widget::Hide();
}

S::Int S::GUI::Hyperlink::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Int	 textColor;

	textRect.left	= realPos.x;
	textRect.top	= realPos.y;

	if (linkBitmap == NIL)
	{
		textRect.right	= textRect.left + textSize.cx;
		textRect.bottom	= textRect.top + textSize.cy + 1;

		switch (message)
		{
			default:
			case SP_PAINT:
			case SP_MOUSEOUT:
				textColor = RGB(0, 0, 255);
				break;
			case SP_MOUSEIN:
				textColor = RGB(0, 128, 255);
				break;
		}

		Font	 nFont = font;

		nFont.SetColor(textColor);

		surface->SetText(text, textRect, nFont);
	}
	else
	{
		textRect.right	= textRect.left + size.cx;
		textRect.bottom	= textRect.top + size.cy;

		surface->BlitFromBitmap(linkBitmap, Rect(Point(0, 0), linkBitmap.GetSize()), textRect);
	}

	return Success;
}

S::Int S::GUI::Hyperlink::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Failure;
	if (!active || !IsVisible())	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	textRect.left	= realPos.x;
	textRect.top	= realPos.y;

	if (linkBitmap == NIL)
	{
		textRect.right	= textRect.left + textSize.cx;
		textRect.bottom	= textRect.top + textSize.cy + 1;
	}
	else
	{
		textRect.right	= textRect.left + size.cx - 1;
		textRect.bottom	= textRect.top + size.cy - 1;
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (checked)
			{
				if (Setup::enableUnicode)	LiSAOpenURLW(linkURL);
				else				LiSAOpenURLA(linkURL);

				wnd->Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}
			break;
		case SM_MOUSEMOVE:
			if (wnd->IsMouseOn(textRect) && !checked)
			{
				checked = True;

				LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_HAND);

				Paint(SP_MOUSEIN);
			}
			else if (!wnd->IsMouseOn(textRect) && checked)
			{
				checked = False;

				LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_ARROW);

				Paint(SP_MOUSEOUT);
			}
			break;
	}

	return retVal;
}

S::GUI::Bitmap &S::GUI::Hyperlink::GetBitmap()
{
	return linkBitmap;
}

S::String S::GUI::Hyperlink::GetURL()
{
	return linkURL;
}

S::Int S::GUI::Hyperlink::SetText(const String &newText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	return Widget::SetText(newText);
}

S::Int S::GUI::Hyperlink::SetURL(String newUrl)
{
	linkURL = newUrl;

	return Success;
}

S::Int S::GUI::Hyperlink::SetBitmap(const Bitmap &newBmp)
{
	return Failure;
}
