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
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/system/timer.h>

const S::Int	 S::GUI::Button::classID = S::Object::RequestClassID();

S::GUI::Button::Button(String iText, const Bitmap &bmp, Point iPos, Size iSize)
{
	type		= classID;
	text		= iText;
	bitmap		= bmp;
	tipTimer	= NIL;
	tooltip		= NIL;
	backgroundColor	= -1;

	bitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 22;

	if (bitmap == NIL)	bmpSize = Size(0, 0);
	else			bmpSize = bitmap.GetSize();

	GetTextSize();
}

S::GUI::Button::~Button()
{
	Window	*wnd = NIL;

	if (tipTimer != NIL)
	{
		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (tooltip != NIL)
	{
		wnd = tooltip->GetContainer()->GetContainerWindow();

		tooltip->Hide();

		wnd->UnregisterObject(tooltip);

		DeleteObject(tooltip);

		tooltip = NIL;
	}

	if (registered && container != NIL) container->UnregisterObject(this);
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

	Surface	*surface = container->GetDrawSurface();

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
			frame.right	= realPos.x + size.cx - 1;
			frame.bottom	= realPos.y + size.cy - 1;

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
				iFrame.right	= realPos.x + size.cx - 4;
				iFrame.bottom	= realPos.y + size.cy - 4;

				surface->Box(iFrame, backgroundColor, FILLED);
			}

			if (text != NIL)
			{
				if (bitmap != NIL)
				{
					textRect.left	= frame.left + ((size.cx - textSize.cx - bmpSize.cx - 7) / 2) + bmpSize.cx + 6;
					textRect.top	= frame.top + ((size.cy - textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);
				}
				else
				{
					textRect.left	= frame.left + ((size.cx - textSize.cx) / 2) - 1;
					textRect.top	= frame.top + ((size.cy - textSize.cy) / 2) - 2;
					textRect.right	= textRect.left + textSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);
				}

				Font	 nFont = font;

				if (!active) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect, nFont);
			}

			if (bitmap != NIL)
			{
				if (text != NIL)
				{
					bmpRect.left	= frame.left + (frame.right - frame.left - bmpSize.cx - textSize.cx - 7) / 2 + 1;
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
	
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
			}

			if (checked || (flags & BF_SHOWHIGHLIGHT))
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + size.cx - 5;
				frame.bottom	= realPos.y + size.cy - 5;

				if (clicked)	surface->Frame(frame, FRAME_DOWN);
				else		surface->Frame(frame, FRAME_UP);
			}

			break;
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			frame.left	= realPos.x + 4;
			frame.top	= realPos.y + 4;
			frame.right	= realPos.x + size.cx - 5;
			frame.bottom	= realPos.y + size.cy - 5;

			surface->Frame(frame, FRAME_UP);

			break;
		case SP_MOUSEDOWN:
			frame.left	= realPos.x + 4;
			frame.top	= realPos.y + 4;
			frame.right	= realPos.x + size.cx - 5;
			frame.bottom	= realPos.y + size.cy - 5;

			surface->Frame(frame, FRAME_DOWN);

			break;
		case SP_MOUSEOUT:
			if (flags & BF_SHOWHIGHLIGHT)
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + size.cx - 5;
				frame.bottom	= realPos.y + size.cy - 5;

				surface->Frame(frame, FRAME_UP);
			}
			else
			{
				frame.left	= realPos.x + 4;
				frame.top	= realPos.y + 4;
				frame.right	= realPos.x + size.cx - 4;
				frame.bottom	= realPos.y + size.cy - 4;

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

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	frame.left	= realPos.x + 4;
	frame.top	= realPos.y + 4;
	frame.right	= realPos.x + size.cx - 5;
	frame.bottom	= realPos.y + size.cy - 5;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (checked)
			{
				clicked = True;

				Paint(SP_MOUSEDOWN);

				retVal = Break;

				if (tipTimer != NIL)
				{
					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}

				if (tooltip != NIL)
				{
					tooltip->Hide();

					wnd->UnregisterObject(tooltip);

					DeleteObject(tooltip);

					tooltip = NIL;
				}
			}

			break;
		case SM_LBUTTONUP:
			if (clicked)
			{
				clicked = False;
				checked = False;

				Paint(SP_MOUSEUP);
				Process(SM_MOUSEMOVE, 0, 0);

				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			if (!checked && wnd->IsMouseOn(frame))
			{
				checked = True;

				Paint(SP_MOUSEIN);

				if (tooltipText != NIL)
				{
					tipTimer = new System::Timer();

					tipTimer->onInterval.Connect(&Button::ActivateTooltip, this);
					tipTimer->Start(500);
				}
			}
			else if (checked && !wnd->IsMouseOn(frame))
			{
				checked = False;
				clicked = False;

				Paint(SP_MOUSEOUT);

				if (tipTimer != NIL)
				{
					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}

				if (tooltip != NIL)
				{
					tooltip->Hide();

					wnd->UnregisterObject(tooltip);

					DeleteObject(tooltip);

					tooltip = NIL;
				}
			}
			else if (checked && wnd->IsMouseOn(frame))
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

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer = NIL;

	Window	*wnd	= container->GetContainerWindow();
	Point	 nPos	= Point(wnd->MouseX(), wnd->MouseY());
	Size	 nSize	= Size(0, 0);

	tooltip = new Tooltip();

	tooltip->SetText(tooltipText);
	tooltip->SetMetrics(nPos, nSize);
	tooltip->SetTimeout(3000);

	wnd->RegisterObject(tooltip);
}
