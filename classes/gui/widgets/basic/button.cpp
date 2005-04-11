 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/button.h>
#include <smooth/misc/math.h>
#include <smooth/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/layer.h>

const S::Int	 S::GUI::Button::classID = S::Object::RequestClassID();

S::GUI::Button::Button(String iText, const Bitmap &iBitmap, Point iPos, Size iSize)
{
	type		= classID;
	text		= iText;
	bitmap		= iBitmap;
	backgroundColor	= -1;

	bitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 22;

	if (bitmap == NIL)	bmpSize = Size(0, 0);
	else			bmpSize = bitmap.GetSize();

	borderWidth	= 4;

	GetTextSize();
}

S::GUI::Button::~Button()
{
}

S::Int S::GUI::Button::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame		= Rect(GetRealPosition(), size);
	Rect	 highlightFrame	= Rect(GetRealPosition() + Point(4, 4), size - Size(8, 8));

	switch (message)
	{
		default:
		case SP_PAINT:
			if (!(flags & BF_NOFRAME))
			{
				surface->Frame(frame, FRAME_DOWN);
				surface->Frame(Rect(GetRealPosition() + Point(1, 1), size - Size(2, 2)), FRAME_UP);

				if (backgroundColor >= 0) surface->Box(frame, backgroundColor, FILLED);
			}
			else
			{
				if (backgroundColor >= 0) surface->Box(highlightFrame, backgroundColor, FILLED);
			}

			if (text != NIL)
			{
				Rect	 textRect;

				if (bitmap != NIL)
				{
					textRect.left	= frame.left + ((size.cx - textSize.cx - bmpSize.cx - 7) / 2) + bmpSize.cx + 7;
					textRect.top	= frame.top + ((size.cy - textSize.cy) / 2) - 1;
					textRect.right	= textRect.left + textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);
				}
				else
				{
					textRect.left	= frame.left + ((size.cx - textSize.cx) / 2);
					textRect.top	= frame.top + ((size.cy - textSize.cy) / 2) - 1;
					textRect.right	= textRect.left + textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);
				}

				Font	 nFont = font;

				if (!active) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect, nFont);
			}

			if (bitmap != NIL)
			{
				Rect	 bmpRect;

				if (text != NIL)
				{
					bmpRect.left	= frame.left + (frame.right - frame.left - bmpSize.cx - textSize.cx - 7) / 2;
					bmpRect.top	= frame.top + (frame.bottom - frame.top - bmpSize.cy) / 2;
					bmpRect.right	= bmpRect.left + bmpSize.cx;
					bmpRect.bottom	= bmpRect.top + bmpSize.cy;
				}
				else
				{
					bmpRect.left	= frame.left + (frame.right - frame.left - bmpSize.cx) / 2;
					bmpRect.top	= frame.top + (frame.bottom - frame.top - bmpSize.cy) / 2;
					bmpRect.right	= bmpRect.left + bmpSize.cx;
					bmpRect.bottom	= bmpRect.top + bmpSize.cy;
				}
	
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
			}

			if (flags & BF_SHOWHIGHLIGHT) surface->Frame(highlightFrame, FRAME_UP);

			break;
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			surface->Frame(highlightFrame, FRAME_UP);

			break;
		case SP_MOUSEDOWN:
			surface->Frame(highlightFrame, FRAME_DOWN);

			break;
		case SP_MOUSEOUT:
			if (flags & BF_SHOWHIGHLIGHT)	surface->Frame(highlightFrame, FRAME_UP);
			else				surface->Box(highlightFrame, backgroundColor >= 0 ? backgroundColor : Setup::BackgroundColor, OUTLINED);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Button::SetBackgroundColor(Int nColor)
{
	backgroundColor = nColor;

	return Success;
}
