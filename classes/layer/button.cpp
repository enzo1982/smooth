 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BUTTON_
#define __OBJSMOOTH_BUTTON_

#include <smooth/button.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/tooltip.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_BUTTON = SMOOTH::RequestObjectID();

SMOOTHButton::SMOOTHButton(SMOOTHString text, HBITMAP bmp, SMOOTHPoint pos, SMOOTHSize size, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_BUTTON;
	objectProperties->text		= text;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	bitmap				= DetectTransparentRegions(bmp);
	tipTimer			= NIL;
	tooltip				= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(22 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);

	if (bitmap == NIL)
	{
		bmpSize.cx = 0;
		bmpSize.cy = 0;
	}
	else
	{
		bmpSize.cx = roundtoint(GetBitmapSizeX(bitmap) * SMOOTH::Setup::FontSize);
		bmpSize.cy = roundtoint(GetBitmapSizeY(bitmap) * SMOOTH::Setup::FontSize);
	}

	GetTextSize();
}

SMOOTHButton::~SMOOTHButton()
{
	SMOOTHWindow	*wnd = NIL;

	if (tipTimer != NIL)
	{
		if (tooltip != NIL)
		{
			wnd = (SMOOTHWindow *) tooltip->GetContainer()->GetContainerObject();

			tooltip->Hide();

			wnd->UnregisterObject(tooltip);

			SMOOTH::DeleteObject(tooltip);

			tooltip = NIL;
		}

		wnd = (SMOOTHWindow *) tipTimer->GetContainer()->GetContainerObject();

		tipTimer->Stop();

		wnd->UnregisterObject(tipTimer);

		SMOOTH::DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHButton::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	SMOOTHRect	 frame;
	SMOOTHRect	 textRect;
	SMOOTHRect	 bmpRect;
	SMOOTHPoint	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			surface->Frame(frame, FRAME_DOWN);

			frame.left++;
			frame.top++;
			frame.right--;
			frame.bottom--;

			surface->Frame(frame, FRAME_UP);

			if (objectProperties->text != NIL)
			{
				if (bitmap != NIL)
				{
					textRect.left	= frame.left + ((objectProperties->size.cx - objectProperties->textSize.cx - bmpSize.cx - 7) / 2) + bmpSize.cx + 6;
					textRect.top	= frame.top + ((objectProperties->size.cy - objectProperties->textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + objectProperties->textSize.cx + 1;
					textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);
				}
				else
				{
					textRect.left	= frame.left + ((objectProperties->size.cx - objectProperties->textSize.cx) / 2) - 1;
					textRect.top	= frame.top + ((objectProperties->size.cy - objectProperties->textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + objectProperties->textSize.cx + 1;
					textRect.bottom	= textRect.top + roundtoint(objectProperties->textSize.cy * 1.2);
				}

				if (active)	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
				else		surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);
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
	
				surface->BlitFromBitmap(bitmap, GetBitmapMetrics(bitmap), bmpRect);
			}

			if (objectProperties->checked)
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
			frame.left	= realPos.x + 4;
			frame.top	= realPos.y + 4;
			frame.right	= realPos.x + objectProperties->size.cx - 4;
			frame.bottom	= realPos.y + objectProperties->size.cy - 4;

			surface->Box(frame, SMOOTH::Setup::BackgroundColor, OUTLINED);

			break;
	}

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHButton::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHRect	 frame;
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;

	frame.left	= realPos.x + 4;
	frame.top	= realPos.y + 4;
	frame.right	= realPos.x + objectProperties->size.cx - 5;
	frame.bottom	= realPos.y + objectProperties->size.cy - 5;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (objectProperties->checked)
			{
				objectProperties->clicked = SMOOTH::True;

				Paint(SP_MOUSEDOWN);

				retVal = SMOOTH::Break;

				if (tipTimer != NIL)
				{
					if (tooltip != NIL)
					{
						tooltip->Hide();

						wnd->UnregisterObject(tooltip);

						SMOOTH::DeleteObject(tooltip);

						tooltip = NIL;
					}

					tipTimer->Stop();

					wnd->UnregisterObject(tipTimer);

					SMOOTH::DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}

			break;
		case SM_LBUTTONUP:
			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;
				objectProperties->checked = SMOOTH::False;

				Paint(SP_MOUSEUP);

				Process(SM_MOUSEMOVE, 0, 0);

				SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}

			break;
		case SM_MOUSELEAVE:
			if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;

				Paint(SP_MOUSEOUT);

				if (tipTimer != NIL)
				{
					if (tooltip != NIL)
					{
						tooltip->Hide();

						wnd->UnregisterObject(tooltip);

						SMOOTH::DeleteObject(tooltip);

						tooltip = NIL;
					}

					tipTimer->Stop();

					wnd->UnregisterObject(tipTimer);

					SMOOTH::DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}

			break;
		case SM_MOUSEMOVE:
			if (!objectProperties->checked && IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::True;

				Paint(SP_MOUSEIN);

				if (objectProperties->tooltip != NIL)
				{
					tipTimer = new SMOOTHTimer();

					wnd->RegisterObject(tipTimer);

					tipTimer->SetProc(SMOOTHProc(SMOOTHButton, this, ActivateTooltip));
					tipTimer->Start(500);
				}
			}
			else if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;

				Paint(SP_MOUSEOUT);

				if (tipTimer != NIL)
				{
					if (tooltip != NIL)
					{
						tooltip->Hide();

						wnd->UnregisterObject(tooltip);

						SMOOTH::DeleteObject(tooltip);

						tooltip = NIL;
					}

					tipTimer->Stop();

					wnd->UnregisterObject(tipTimer);

					SMOOTH::DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}
			else if (objectProperties->checked && IsMouseOn(wnd->hwnd, frame, WINDOW))
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

SMOOTHVoid SMOOTHButton::ActivateTooltip()
{
	if (tooltip != NIL) return;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	pos.x = MouseX(wnd->hwnd, WINDOW);
	pos.y = MouseY(wnd->hwnd, WINDOW);
	size.cx = 0;
	size.cy = 0;

	tooltip = new SMOOTHTooltip();

	tooltip->SetText(objectProperties->tooltip);
	tooltip->SetMetrics(pos, size);
	tooltip->SetTimeout(3000);

	wnd->RegisterObject(tooltip);
}

#endif
