 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listbox.h>
#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/scrollbar.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_LISTBOX = S::Object::RequestObjectID();

S::ListBox::ListBox(Point pos, Size size, ProcParam, Void *procParam)
{
	type				= OBJ_LISTBOX;
	objectProperties->proc		= (ProcType) newProc;
	objectProperties->procParam	= procParam;
	entryCount			= -1;

	needScrollbar		= False;
	scrollbar		= NIL;
	scrollbarPos		= 0;
	lastScrollbarPos	= 0;
	allowReselect		= False;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, SMOOTH::Setup::ClientTextColor, objectProperties->fontWeight);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(120 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);
}

S::ListBox::~ListBox()
{
	if (needScrollbar)
	{
		if (myContainer != NIL) myContainer->UnregisterObject(scrollbar);

		delete scrollbar;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::ListBox::AllowReselect(Bool value)
{
	allowReselect = value;

	return Success;
}

S::List::Entry *S::ListBox::AddEntry(String name, ProcParam, Void *procParam)
{
	entryCount++;

	Entry *newEntry = AddListEntry(entryCount, name, newProc, procParam);

	Paint(SP_UPDATE);

	return newEntry;
}

S::Int S::ListBox::ModifyEntry(Int code, String name, ProcParam, Void *procParam)
{
	if (ModifyListEntry(code, name, newProc, procParam) == Success)
	{
		Paint(SP_PAINT);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::ListBox::RemoveEntry(Int number)
{
	RemoveListEntry(number);

	if (needScrollbar)
	{
		needScrollbar = False;

		if (METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4 <= objectProperties->size.cy)
		{
			scrollbarPos = 0;
			lastScrollbarPos = 0;
		}

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);

	return Success;
}

S::Void S::ListBox::Cleanup()
{
	CleanupList();

	if (needScrollbar)
	{
		needScrollbar = False;
		scrollbarPos = 0;
		lastScrollbarPos = 0;

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);
}

S::Int S::ListBox::SelectEntry(Int code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return Success;
}

S::Int S::ListBox::Show()
{
	if (visible)	return Success;

	if (needScrollbar)
	{
		Layer	*layer = (Layer *) myContainer->GetContainerObject();
		Point	 realPos = GetRealPosition();
		Point	 sbp = Point(realPos.x + objectProperties->size.cx - 2 - layer->GetObjectProperties()->pos.x - METRIC_LISTBOXSBOFFSET, realPos.y + 1 - layer->GetObjectProperties()->pos.y);
		Size	 sbs = Size(METRIC_LISTBOXSBSIZE, objectProperties->size.cy - 1);
		Float	 oldMeasurement = SMOOTH::Setup::FontSize;

		SetMeasurement(SMT_PIXELS);

		scrollbar->SetMetrics(sbp, sbs);
		scrollbar->SetRange(0, nOfEntries - (int) ((objectProperties->size.cy - 4) / METRIC_LISTBOXENTRYHEIGHT));

		SMOOTH::Setup::FontSize = oldMeasurement;

		scrollbar->Show();
	}

	return Object::Show();
}

S::Int S::ListBox::Hide()
{
	if (!visible)	return Success;

	if (needScrollbar) scrollbar->Hide();

	return Object::Hide();
}

S::Int S::ListBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Layer		*layer = (Layer *) myContainer->GetContainerObject();
	Point		 realPos = GetRealPosition();
	List::Entry	*operat;
	Rect		 frame;
	Point		 sbp;
	Size		 sbs;
	Int		 maxFrameY;
	Float		 oldMeasurement;

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			if (message != SP_UPDATE)
			{
				if (active)	surface->Box(frame, SMOOTH::Setup::ClientColor, FILLED);
				else		surface->Box(frame, SMOOTH::Setup::BackgroundColor, FILLED);

				surface->Frame(frame, FRAME_DOWN);
			}

			maxFrameY = frame.bottom - 1;

			frame.left++;
			frame.top++;
			frame.right--;
			frame.bottom = frame.top + METRIC_LISTBOXENTRYHEIGHT;

			frame.bottom = min(frame.bottom, maxFrameY);

			if (METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4 > objectProperties->size.cy)
			{
				if (!needScrollbar)
				{
					needScrollbar = True;

					sbp.x = frame.right - layer->GetObjectProperties()->pos.x - METRIC_LISTBOXSBOFFSET;
					sbp.y = frame.top - layer->GetObjectProperties()->pos.y;
					sbs.cx = METRIC_LISTBOXSBSIZE;
					sbs.cy = objectProperties->size.cy - 1;

					oldMeasurement = SMOOTH::Setup::FontSize;

					SetMeasurement(SMT_PIXELS);

					scrollbar = new Scrollbar(sbp, sbs, OR_VERT, &scrollbarPos, 0, nOfEntries - (int) ((objectProperties->size.cy - 4) / METRIC_LISTBOXENTRYHEIGHT), Proc(&ListBox::ScrollbarProc), this);

					SMOOTH::Setup::FontSize = oldMeasurement;

					layer->RegisterObject(scrollbar);

					scrollbar->Paint(SP_PAINT);
				}
				else
				{
					scrollbar->SetRange(0, nOfEntries - (int) ((objectProperties->size.cy - 4) / METRIC_LISTBOXENTRYHEIGHT));
				}

				frame.right -= (METRIC_LISTBOXSBOFFSET + 1);
			}
			else
			{
				if (needScrollbar)
				{
					needScrollbar = False;
					scrollbarPos = 0;
					lastScrollbarPos = 0;

					layer->UnregisterObject(scrollbar);

					delete scrollbar;
					scrollbar = NIL;
				}
			}

			lastScrollbarPos = scrollbarPos;

			for (int i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (i >= scrollbarPos && frame.top < maxFrameY)
				{
					operat->rect = frame;

					operat->rect.left++;
					operat->rect.top++;
					operat->rect.right--;

					if (i == (nOfEntries - 1) || message != SP_UPDATE)
					{
						frame.left += METRIC_LISTBOXTEXTOFFSETXY;
						frame.top += METRIC_LISTBOXTEXTOFFSETXY;
						surface->SetText(operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
						frame.left -= METRIC_LISTBOXTEXTOFFSETXY;
						frame.top -= METRIC_LISTBOXTEXTOFFSETXY;
					}

					if (operat->clk && frame.top < frame.bottom)
					{
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;
					}

					frame.top += METRIC_LISTBOXENTRYHEIGHT;
					frame.bottom += METRIC_LISTBOXENTRYHEIGHT;

					frame.top = min(frame.top, maxFrameY);
					frame.bottom = min(frame.bottom, maxFrameY);
				}
				else
				{
					operat->rect = Rect(Point(-1, -1), Size(0, 0));
				}
			}

			break;
	}

	LeaveProtectedRegion();

	if (needScrollbar) scrollbar->Paint(SP_PAINT);

	return Success;
}

S::Int S::ListBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point		 realPos = GetRealPosition();
	Int		 retVal = Success;
	List::Entry	*operat;
	Rect		 frame;
	HDC		 dc;
	Bool		 change = False;
	Int		 maxFrameY;
	Int		 i;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_CHECKLISTBOXES:
			dc = GetContext(wnd);

			if (scrollbarPos != lastScrollbarPos)
			{
				lastScrollbarPos = scrollbarPos;

				frame.left++;
				frame.top++;
				frame.right--;
				frame.bottom--;

				frame.right -= (METRIC_LISTBOXSBOFFSET + 1);

				Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);

				frame.left	= realPos.x;
				frame.top	= realPos.y;
				frame.right	= realPos.x + objectProperties->size.cx - 1;
				frame.bottom	= realPos.y + objectProperties->size.cy - 1;

				maxFrameY = frame.bottom - 1;

				frame.left++;
				frame.top++;
				frame.right	-= (METRIC_LISTBOXSBOFFSET + 2);
				frame.bottom	= frame.top + METRIC_LISTBOXENTRYHEIGHT;

				frame.bottom = min(frame.bottom, maxFrameY);

				for (i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat == NIL) break;

					if (i >= scrollbarPos && frame.top < maxFrameY)
					{
						operat->rect = frame;

						operat->rect.left++;
						operat->rect.top++;
						operat->rect.right--;

						frame.left += METRIC_LISTBOXTEXTOFFSETXY;
						frame.top += METRIC_LISTBOXTEXTOFFSETXY;
						::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
						frame.left -= METRIC_LISTBOXTEXTOFFSETXY;
						frame.top -= METRIC_LISTBOXTEXTOFFSETXY;

						if (operat->clk && frame.top < frame.bottom)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}

						frame.top += METRIC_LISTBOXENTRYHEIGHT;
						frame.bottom += METRIC_LISTBOXENTRYHEIGHT;

						frame.top = min(frame.top, maxFrameY);
						frame.bottom = min(frame.bottom, maxFrameY);
					}
					else
					{
						operat->rect = Rect(Point(-1, -1), Size(0, 0));
					}
				}

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					wnd->Process(SM_LOOSEFOCUS, handle, 0);

					change = True;
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					if (operat->clk && change)
					{
						operat->chk = False;
						operat->clk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::ClientColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					if (!operat->clk || allowReselect)
					{
						operat->chk = True;
						operat->clk = True;
						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;

						ProcCall(objectProperties->proc, objectProperties->procParam);
						ProcCall(operat->proc, operat->procParam);
					}

					retVal = Break;
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					if (operat->chk)
					{
						operat->chk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::ClientColor, FILLED);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						::SetText(dc, operat->text, operat->rect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					if (operat->chk)
					{
						operat->chk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::ClientColor, FILLED);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						::SetText(dc, operat->text, operat->rect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (IsMouseOn(wnd->hwnd, operat->rect, WINDOW))
				{
					if (!operat->chk)
					{
						operat->chk = True;
						operat->rect.right++;
						operat->rect.bottom++;
						Gradient(dc, operat->rect, SMOOTH::Setup::GradientStartColor, SMOOTH::Setup::GradientEndColor, GRADIENT_LR);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						::SetText(dc, operat->text, operat->rect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GradientTextColor, objectProperties->fontWeight);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

S::Void S::ListBox::ScrollbarProc()
{
	Process(SM_CHECKLISTBOXES, 0, 0);
}
