 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/button.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/tooltip.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/gdi/bitmapgdi.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

const S::Int	 S::GUI::Button::classID = S::Object::RequestClassID();

S::GUI::Button::Button(String text, HBITMAP bmp, Point pos, Size size)
{
	type			= classID;
	objectProperties->text	= text;
	bitmap			= DetectTransparentRegions(bmp);
	tipTimer		= NIL;
	tooltip			= NIL;
	backgroundColor		= -1;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(22 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);

	if (bitmap == NIL)
	{
		bmpSize.cx = 0;
		bmpSize.cy = 0;
	}
	else
	{
		bmpSize.cx = Math::Round(GetBitmapSizeX(bitmap) * Setup::FontSize);
		bmpSize.cy = Math::Round(GetBitmapSizeY(bitmap) * Setup::FontSize);
	}

	GetTextSize();
}

S::GUI::Button::~Button()
{
	Window	*wnd = NIL;

	if (tipTimer != NIL)
	{
		if (tooltip != NIL)
		{
			wnd = tooltip->GetContainer()->GetContainerWindow();

			tooltip->Hide();

			wnd->UnregisterObject(tooltip);

			DeleteObject(tooltip);

			tooltip = NIL;
		}

		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Button::SetBackgroundColor(Int nColor)
{
	backgroundColor = nColor;

	return Success;
}

S::Int S::GUI::Button::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Rect	 textRect;
	Rect	 bmpRect;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			if (!(flags & BF_NOFRAME)) surface->Frame(frame, FRAME_DOWN);

			frame.left++;
			frame.top++;
			frame.right--;
			frame.bottom--;

			if (!(flags & BF_NOFRAME)) surface->Frame(frame, FRAME_UP);

			if (backgroundColor >= 0)
			{
				Rect	 iFrame = frame;

				iFrame.left	= realPos.x + 4;
				iFrame.top	= realPos.y + 4;
				iFrame.right	= realPos.x + objectProperties->size.cx - 4;
				iFrame.bottom	= realPos.y + objectProperties->size.cy - 4;

				surface->Box(iFrame, backgroundColor, FILLED);
			}

			if (objectProperties->text != NIL)
			{
				if (bitmap != NIL)
				{
					textRect.left	= frame.left + ((objectProperties->size.cx - objectProperties->textSize.cx - bmpSize.cx - 7) / 2) + bmpSize.cx + 6;
					textRect.top	= frame.top + ((objectProperties->size.cy - objectProperties->textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + objectProperties->textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);
				}
				else
				{
					textRect.left	= frame.left + ((objectProperties->size.cx - objectProperties->textSize.cx) / 2) - 1;
					textRect.top	= frame.top + ((objectProperties->size.cy - objectProperties->textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + objectProperties->textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);
				}

				Font	 font = objectProperties->font;

				if (!active) font.SetColor(Setup::GrayTextColor);

				surface->SetText(objectProperties->text, textRect, font);
			}

			if (bitmap != NIL)
			{
				if (objectProperties->text != NIL)
				{
					bmpRect.left	= frame.left + (frame.right - frame.left - bmpSize.cx - objectProperties->textSize.cx - 7) / 2 + 1;
					bmpRect.top	= frame.top + (frame.bottom - frame.top - bmpSize.cy) / 2 + 1;
					bmpRect.right	= bmpRect.left + bmpSize.cx;
					bmpRect.bottom	= bmpRect.top + bmpSize.cy;
				}
				else
				{
					bmpRect.left	= frame.left + (frame.right - frame.left - bmpSize.cx) / 2 + 1;
					bmpRect.top	= frame.top + (frame.bottom - frame.top - bmpSize.cy) / 2 + 1;
					bmpRect.right	= bmpRect.left + bmpSize.cx;
					bmpRect.bottom	= bmpRect.top + bmpSize.cy;
				}

				BitmapGDI	*bmpGDI = new BitmapGDI(bitmap);
	
				surface->BlitFromBitmap(bmpGDI, Rect(Point(0, 0), Size(GetBitmapSizeX(bitmap), GetBitmapSizeY(bitmap))), bmpRect);

				delete bmpGDI;
			}

			if (objectProperties->checked || (flags & BF_SHOWHIGHLIGHT))
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + objectProperties->size.cx - 5;
				frame.bottom	= realPos.y + objectProperties->size.cy - 5;

				if (objectProperties->clicked)	surface->Frame(frame, FRAME_DOWN);
				else				surface->Frame(frame, FRAME_UP);
			}

			break;
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			frame.left	= realPos.x + 4;
			frame.top	= realPos.y + 4;
			frame.right	= realPos.x + objectProperties->size.cx - 5;
			frame.bottom	= realPos.y + objectProperties->size.cy - 5;

			surface->Frame(frame, FRAME_UP);

			break;
		case SP_MOUSEDOWN:
			frame.left	= realPos.x + 4;
			frame.top	= realPos.y + 4;
			frame.right	= realPos.x + objectProperties->size.cx - 5;
			frame.bottom	= realPos.y + objectProperties->size.cy - 5;

			surface->Frame(frame, FRAME_DOWN);

			break;
		case SP_MOUSEOUT:
			if (flags & BF_SHOWHIGHLIGHT)
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + objectProperties->size.cx - 5;
				frame.bottom	= realPos.y + objectProperties->size.cy - 5;

				surface->Frame(frame, FRAME_UP);
			}
			else
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + objectProperties->size.cx - 4;
				frame.bottom	= realPos.y + objectProperties->size.cy - 4;

				surface->Box(frame, backgroundColor >= 0 ? backgroundColor : Setup::BackgroundColor, OUTLINED);
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Button::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	frame.left	= realPos.x + 4;
	frame.top	= realPos.y + 4;
	frame.right	= realPos.x + objectProperties->size.cx - 5;
	frame.bottom	= realPos.y + objectProperties->size.cy - 5;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (objectProperties->checked)
			{
				objectProperties->clicked = True;

				Paint(SP_MOUSEDOWN);

				retVal = Break;

				if (tipTimer != NIL)
				{
					if (tooltip != NIL)
					{
						tooltip->Hide();

						wnd->UnregisterObject(tooltip);

						DeleteObject(tooltip);

						tooltip = NIL;
					}

					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}

			break;
		case SM_LBUTTONUP:
			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;
				objectProperties->checked = False;

				Paint(SP_MOUSEUP);
				Process(SM_MOUSEMOVE, 0, 0);

				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			if (!objectProperties->checked && wnd->IsMouseOn(frame))
			{
				objectProperties->checked = True;

				Paint(SP_MOUSEIN);

				if (objectProperties->tooltip != NIL)
				{
					tipTimer = new Timer();

					tipTimer->onInterval.Connect(&Button::ActivateTooltip, this);
					tipTimer->Start(500);
				}
			}
			else if (objectProperties->checked && !wnd->IsMouseOn(frame))
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

				Paint(SP_MOUSEOUT);

				if (tipTimer != NIL)
				{
					if (tooltip != NIL)
					{
						tooltip->Hide();

						wnd->UnregisterObject(tooltip);

						DeleteObject(tooltip);

						tooltip = NIL;
					}

					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}
			else if (objectProperties->checked && wnd->IsMouseOn(frame))
			{
				if (tipTimer != NIL && wParam == 0)
				{
					tipTimer->Stop();
					tipTimer->Start(500);
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::Button::ActivateTooltip()
{
	if (tooltip != NIL) return;

	Window	*wnd = myContainer->GetContainerWindow();
	Point	 pos;
	Size	 size;

	pos.x	= wnd->MouseX();
	pos.y	= wnd->MouseY();
	size.cx	= 0;
	size.cy	= 0;

	tooltip = new Tooltip();

	tooltip->SetText(objectProperties->tooltip);
	tooltip->SetMetrics(pos, size);
	tooltip->SetTimeout(3000);

	wnd->RegisterObject(tooltip);
}
