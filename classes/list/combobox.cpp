 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_COMBOBOX_
#define __OBJSMOOTH_COMBOBOX_

#include <smooth/combobox.h>
#include <smooth/listBox.h>
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
#include <smooth/toolwindow.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_COMBOBOX = SMOOTH::RequestObjectID();

SMOOTHComboBox::SMOOTHComboBox(SMOOTHPoint pos, SMOOTHSize size, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_COMBOBOX;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	entryCount			= -1;

	closeListBox	= SMOOTH::False;
	listBoxOpen	= SMOOTH::False;
	listBox		= NIL;
	toolWindow	= NIL;
	layer		= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, SMOOTH::Setup::ClientTextColor, objectProperties->fontWeight);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(19 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHComboBox::~SMOOTHComboBox()
{
	if (listBoxOpen)
	{
		if (listBox->registered && myContainer != NIL) myContainer->UnregisterObject(listBox);

		delete listBox;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHComboBox::AddEntry(SMOOTHString name, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	entryCount++;

	AddListEntry(entryCount, name, newProc, procParam);

	if (entryCount == 0) entries.GetFirstEntry()->clk = SMOOTH::True;

	Paint(SP_PAINT);

	return entryCount;
}

SMOOTHInt SMOOTHComboBox::ModifyEntry(SMOOTHInt code, SMOOTHString name, SMOOTHProcParam, SMOOTHVoid *procParam)
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

SMOOTHInt SMOOTHComboBox::RemoveEntry(SMOOTHInt number)
{
	RemoveListEntry(number);

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHComboBox::Cleanup()
{
	CleanupList();

	Paint(SP_PAINT);
}

SMOOTHInt SMOOTHComboBox::SelectEntry(SMOOTHInt code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHComboBox::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*lay = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) lay->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC			 dc = GetContext(wnd);
	SMOOTHPoint		 realPos = GetRealPosition();
	SMOOTHList::Entry	*operat;
	SMOOTHRect		 frame;
	SMOOTHPoint		 lineStart;
	SMOOTHPoint		 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

	Frame(dc, frame, FRAME_DOWN);

	frame.top++;
	frame.bottom--;
	frame.right--;
	frame.left = frame.right - METRIC_COMBOBOXOFFSETX;

	Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);
	Frame(dc, frame, FRAME_UP);

	frame.top--;
	frame.bottom++;
	frame.right++;
	frame.left = realPos.x;

	lineStart.x = frame.right - METRIC_COMBOBOXARROWOFFSETX;
	lineStart.y = frame.top + METRIC_COMBOBOXARROWOFFSETY;
	lineEnd.x = lineStart.x + 7;
	lineEnd.y = lineStart.y;

	for (SMOOTHInt i = 0; i < 4; i++)
	{
		if (active)	Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);
		else		Line(dc, lineStart, lineEnd, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

		lineStart.x++;
		lineStart.y++;
		lineEnd.x--;
		lineEnd.y++;
	}

	for (SMOOTHInt j = 0; j < nOfEntries; j++)
	{
		operat = entries.GetNthEntry(j);

		if (operat->clk)
		{
			frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

			::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

			frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
			frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
		}
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHComboBox::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*lay = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) lay->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint		 realPos = GetRealPosition();
	SMOOTHInt		 retVal = SMOOTH::Success;
	SMOOTHList::Entry	*operat;
	SMOOTHRect		 frame;
	SMOOTHRect		 lbframe;
	HDC			 dc;
	SMOOTHPoint		 lbp;
	SMOOTHSize		 lbs;
	SMOOTHBool		 executeProcs = SMOOTH::False;
	SMOOTHFloat		 oldMeasurement;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
#ifdef __WIN32__
		case WM_ACTIVATE:
			if (message == WM_ACTIVATE) if (toolWindow != NIL) if (((HWND) lParam) == toolWindow->hwnd) break;
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
			if (message == WM_KILLFOCUS) if (toolWindow != NIL) if (((HWND) wParam) == toolWindow->hwnd) break;

			if (listBoxOpen)
			{
				listBoxOpen = SMOOTH::False;

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				delete listBox;
				delete layer;
				delete toolWindow;

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;
			}

			break;
#endif
		case SM_LOOSEFOCUS:
			lbframe.top	= frame.bottom + 1;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= frame.right;
			lbframe.left	= frame.left;

			if (wParam != handle)
			{
				if ((IsMouseOn(wnd->hwnd, frame, WINDOW) && listBoxOpen) || (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW) && listBoxOpen))
				{
					listBoxOpen = SMOOTH::False;

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					delete listBox;
					delete layer;
					delete toolWindow;

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;

					retVal = SMOOTH::Break;
				}
			}

			break;
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			lbframe.top	= frame.bottom + 1;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= frame.right;
			lbframe.left	= frame.left;

			if (IsMouseOn(wnd->hwnd, frame, WINDOW) && !listBoxOpen)
			{
				wnd->Process(SM_LOOSEFOCUS, handle, 0);

				listBoxOpen = SMOOTH::True;

				lbp.x = frame.left - lay->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 - lay->GetObjectProperties()->pos.y;
				lbs.cx = objectProperties->size.cx;
				lbs.cy = min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);

				oldMeasurement = SMOOTH::Setup::FontSize;

				SMOOTHSetMeasurement(SMT_PIXELS);

				layer		= new SMOOTHLayer();
				toolWindow	= new SMOOTHToolWindow();
				listBox		= new SMOOTHListBox(SMOOTHPoint(0, 0), lbs, SMOOTHProc(SMOOTHComboBox, this, ListBoxProc));

				lbp.x = frame.left + wnd->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 + wnd->GetObjectProperties()->pos.y;

				if (objectProperties->checked)
				{
					objectProperties->clicked = SMOOTH::True;

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_DOWN);
				}

				toolWindow->SetMetrics(lbp, lbs);

				SMOOTH::Setup::FontSize = oldMeasurement;

				listBox->allowReselect = SMOOTH::True;

				for (SMOOTHInt i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					listBox->AddEntry(operat->text, NULLPROC);
				}

				listBox->SelectEntry(GetSelectedEntry());

				wnd->RegisterObject(toolWindow);
				toolWindow->RegisterObject(layer);
				layer->RegisterObject(listBox);

				toolWindow->SetOwner(this);

				toolWindow->Show();

				listBox->Paint(SP_PAINT);

				retVal = SMOOTH::Break;
			}
			else if ((IsMouseOn(wnd->hwnd, frame, WINDOW) && listBoxOpen) || (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW) && listBoxOpen))
			{
				listBoxOpen = SMOOTH::False;

				if (objectProperties->checked)
				{
					objectProperties->clicked = SMOOTH::True;

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_DOWN);

					frame.top	-= 3;
					frame.bottom	+= 3;
					frame.right	+= 3;
					frame.left	= realPos.x;
				}

				frame.top	= frame.bottom + 1;
				frame.bottom	= frame.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
				frame.right++;

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				delete listBox;
				delete layer;
				delete toolWindow;

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;

				frame.left	= realPos.x;
				frame.top	= realPos.y;
				frame.right	= realPos.x + objectProperties->size.cx - 1;
				frame.bottom	= realPos.y + objectProperties->size.cy - 1;

				if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW)) wnd->Process(SM_LBUTTONDOWN, 0, 0);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (closeListBox)
			{
				for (SMOOTHInt i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::ClientColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (GetSelectedEntry() != listBox->GetSelectedEntry()) executeProcs = SMOOTH::True;

				SelectEntry(listBox->GetSelectedEntry());

				for (SMOOTHInt j = 0; j < nOfEntries; j++)
				{
					operat = entries.GetNthEntry(j);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (listBoxOpen)
				{
					listBoxOpen = SMOOTH::False;

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					delete listBox;
					delete layer;
					delete toolWindow;

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;
				}

				closeListBox = SMOOTH::False;

				retVal = SMOOTH::Break;
			}

			if (executeProcs)
			{
				for (SMOOTHInt i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
						SMOOTHProcCall(operat->proc, operat->procParam);

						break;
					}
				}
			}

			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;

				if (objectProperties->checked)
				{
					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_UP);
				}
				else
				{
					frame.top	+= 3;
					frame.bottom	-= 2;
					frame.right	-= 2;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 3;

					Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

			if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

			if (IsMouseOn(wnd->hwnd, frame, WINDOW) && !objectProperties->checked)
			{
				Frame(dc, frame, FRAME_UP);

				objectProperties->checked = SMOOTH::True;
			}
			else if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = SMOOTH::False;
				objectProperties->clicked = SMOOTH::False;
			}

			FreeContext(wnd, dc);

			break;
		case SM_CHECKCOMBOBOXES:
			dc = GetContext(wnd);

			if (listBox != NIL)
			{
				closeListBox = SMOOTH::True;
				
				toolWindow->SetOwner(this);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

SMOOTHVoid SMOOTHComboBox::ListBoxProc()
{
	Process(SM_CHECKCOMBOBOXES, 0, 0);
}

#endif
