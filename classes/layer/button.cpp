 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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
#include <smooth/surface.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_BUTTON = S::Object::RequestObjectID();

S::Button::Button(String text, HBITMAP bmp, Point pos, Size size, ProcParam, Void *procParam)
{
	type				= OBJ_BUTTON;
	objectProperties->text		= text;
	objectProperties->proc		= (ProcType) newProc;
	objectProperties->procParam	= procParam;
	bitmap				= DetectTransparentRegions(bmp);
	tipTimer			= NIL;
	tooltip				= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(22 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);

	if (bitmap == NIL)
	{
		bmpSize.cx = 0;
		bmpSize.cy = 0;
	}
	else
	{
		bmpSize.cx = Math::Round(GetBitmapSizeX(bitmap) * SMOOTH::Setup::FontSize);
		bmpSize.cy = Math::Round(GetBitmapSizeY(bitmap) * SMOOTH::Setup::FontSize);
	}

	GetTextSize();
}

S::Button::~Button()
{
	Window	*wnd = NIL;

	if (tipTimer != NIL)
	{
		if (tooltip != NIL)
		{
			wnd = (Window *) tooltip->GetContainer()->GetContainerObject();

			tooltip->Hide();

			wnd->UnregisterObject(tooltip);

			DeleteObject(tooltip);

			tooltip = NIL;
		}

		wnd = (Window *) tipTimer->GetContainer()->GetContainerObject();

		tipTimer->Stop();

		wnd->UnregisterObject(tipTimer);

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::Button::Paint(Int message)
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
					textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);
				}
				else
				{
					textRect.left	= frame.left + ((objectProperties->size.cx - objectProperties->textSize.cx) / 2) - 1;
					textRect.top	= frame.top + ((objectProperties->size.cy - objectProperties->textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + objectProperties->textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);
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

	return Success;
}

S::Int S::Button::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

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

					wnd->UnregisterObject(tipTimer);

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

				ProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = Break;
			}

			break;
		case SM_MOUSELEAVE:
			if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
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

					wnd->UnregisterObject(tipTimer);

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}
			}

			break;
		case SM_MOUSEMOVE:
			if (!objectProperties->checked && IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				objectProperties->checked = True;

				Paint(SP_MOUSEIN);

				if (objectProperties->tooltip != NIL)
				{
					tipTimer = new Timer();

					wnd->RegisterObject(tipTimer);

					tipTimer->SetProc(Proc(Button, this, ActivateTooltip));
					tipTimer->Start(500);
				}
			}
			else if (objectProperties->checked && !IsMouseOn(wnd->hwnd, frame, WINDOW))
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

					wnd->UnregisterObject(tipTimer);

					DeleteObject(tipTimer);

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

S::Void S::Button::ActivateTooltip()
{
	if (tooltip != NIL) return;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();
	Point	 pos;
	Size	 size;

	pos.x = MouseX(wnd->hwnd, WINDOW);
	pos.y = MouseY(wnd->hwnd, WINDOW);
	size.cx = 0;
	size.cy = 0;

	tooltip = new Tooltip();

	tooltip->SetText(objectProperties->tooltip);
	tooltip->SetMetrics(pos, size);
	tooltip->SetTimeout(3000);

	wnd->RegisterObject(tooltip);
}
