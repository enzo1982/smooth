 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LISTBOX_
#define __OBJSMOOTH_LISTBOX_

#include <smooth/listbox.h>
#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/binary.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/scrollbar.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_LISTBOX = SMOOTH::RequestObjectID();

SMOOTHListBox::SMOOTHListBox(SMOOTHPoint pos, SMOOTHSize size, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_LISTBOX;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	entryCount			= -1;

	needScrollbar		= SMOOTH::False;
	scrollbar		= NIL;
	scrollbarPos		= 0;
	lastScrollbarPos	= 0;
	allowReselect		= SMOOTH::False;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, SMOOTH::Setup::ClientTextColor, objectProperties->fontWeight);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(120 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHListBox::~SMOOTHListBox()
{
	if (needScrollbar)
	{
		if (myContainer != NIL) myContainer->UnregisterObject(scrollbar);

		delete scrollbar;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHListBox::AddEntry(SMOOTHString name, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	entryCount++;

	AddListEntry(entryCount, name, newProc, procParam);

	if (needScrollbar)
	{
		needScrollbar = SMOOTH::False;

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);

	return entryCount;
}

SMOOTHInt SMOOTHListBox::ModifyEntry(SMOOTHInt code, SMOOTHString name, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	if (ModifyListEntry(code, name, newProc, procParam) == SMOOTH::Success)
	{
		Paint(SP_PAINT);

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHInt SMOOTHListBox::RemoveEntry(SMOOTHInt number)
{
	RemoveListEntry(number);

	if (needScrollbar)
	{
		needScrollbar = SMOOTH::False;

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

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHListBox::Cleanup()
{
	CleanupList();

	if (needScrollbar)
	{
		needScrollbar = SMOOTH::False;
		scrollbarPos = 0;
		lastScrollbarPos = 0;

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);
}

SMOOTHInt SMOOTHListBox::SelectEntry(SMOOTHInt code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHListBox::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC			 dc = GetContext(wnd);
	SMOOTHPoint		 realPos = GetRealPosition();
	SMOOTHList::Entry	*operat;
	SMOOTHRect		 frame;
	SMOOTHPoint		 sbp;
	SMOOTHSize		 sbs;
	SMOOTHInt		 maxFrameY;
	SMOOTHFloat		 oldMeasurement;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

	Frame(dc, frame, FRAME_DOWN);

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
			needScrollbar = SMOOTH::True;

			sbp.x = frame.right - layer->GetObjectProperties()->pos.x - METRIC_LISTBOXSBOFFSET;
			sbp.y = frame.top - layer->GetObjectProperties()->pos.y;
			sbs.cx = METRIC_LISTBOXSBSIZE;
			sbs.cy = objectProperties->size.cy - 1;

			oldMeasurement = SMOOTH::Setup::FontSize;

			SMOOTHSetMeasurement(SMT_PIXELS);

			scrollbar = new SMOOTHScrollbar(sbp, sbs, OR_VERT, &scrollbarPos, 0, nOfEntries - (int) ((objectProperties->size.cy - 4) / METRIC_LISTBOXENTRYHEIGHT), SMOOTHProc(SMOOTHListBox, this, ScrollbarProc));

			SMOOTH::Setup::FontSize = oldMeasurement;

			layer->RegisterObject(scrollbar);

			scrollbar->Paint(SP_PAINT);
		}

		frame.right -= (METRIC_LISTBOXSBOFFSET + 1);
	}
	else
	{
		if (needScrollbar)
		{
			needScrollbar = SMOOTH::False;
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

		if (i >= scrollbarPos)
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

			frame.bottom = min(frame.bottom, maxFrameY);
		}
	}

	FreeContext(wnd, dc);

	if (needScrollbar) scrollbar->Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHListBox::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint		 realPos = GetRealPosition();
	SMOOTHInt		 retVal = SMOOTH::Success;
	SMOOTHList::Entry	*operat;
	SMOOTHRect		 frame;
	HDC			 dc;
	SMOOTHBool		 change = SMOOTH::False;
	SMOOTHInt		 maxFrameY;
	SMOOTHInt		 i;

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

					if (i >= scrollbarPos)
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

						frame.bottom = min(frame.bottom, maxFrameY);
					}
					else
					{
						operat->rect = SMOOTHRect(SMOOTHPoint(-1, -1), SMOOTHSize(0, 0));
					}
				}

				retVal = SMOOTH::Break;
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

					change = SMOOTH::True;
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
						operat->chk = SMOOTH::False;
						operat->clk = SMOOTH::False;
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
						operat->chk = SMOOTH::True;
						operat->clk = SMOOTH::True;
						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;

						SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
						SMOOTHProcCall(operat->proc, operat->procParam);
					}

					retVal = SMOOTH::Break;
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
						operat->chk = SMOOTH::False;
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
						operat->chk = SMOOTH::False;
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
						operat->chk = SMOOTH::True;
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

SMOOTHVoid SMOOTHListBox::ScrollbarProc()
{
	Process(SM_CHECKLISTBOXES, 0, 0);
}

#endif
