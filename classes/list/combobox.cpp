 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/combobox.h>
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
#include <smooth/toolwindow.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

const S::Int	 S::GUI::ComboBox::classID = S::Object::RequestClassID();

S::GUI::ComboBox::ComboBox(Point pos, Size size)
{
	type		= classID;
	entryCount	= -1;

	closeListBox	= False;
	listBoxOpen	= False;
	listBox		= NIL;
	toolWindow	= NIL;
	layer		= NIL;

	possibleContainers.AddEntry(Layer::classID);

	SetFont(Font(objectProperties->font.GetName(), I18N_SMALLFONTSIZE, Setup::ClientTextColor));

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(19 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::ComboBox::~ComboBox()
{
	if (listBoxOpen)
	{
		toolWindow->Close();

		Window	*wnd = NIL;

		if (myContainer != NIL) myContainer->GetContainerWindow();

		if (wnd != NIL) wnd->UnregisterObject(toolWindow);

		toolWindow->UnregisterObject(layer);
		layer->UnregisterObject(listBox);

		toolWindow->FreeOwner();

		DeleteObject(listBox);
		DeleteObject(layer);
		DeleteObject(toolWindow);
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::ListEntry *S::GUI::ComboBox::AddEntry(String name, Int id)
{
	if (id >= 0 && GetEntry(id) != NIL) return NIL;

	if (id == -1) id = ++entryCount;

	ListEntry *newEntry = List::AddEntry(name, id);

	if (entryCount == 0 && newEntry != NIL && !(flags & CB_HOTSPOTONLY)) GetFirstEntry()->clicked = True;

	Paint(SP_PAINT);

	return newEntry;
}

S::Int S::GUI::ComboBox::ModifyEntry(Int code, String name)
{
	if (List::ModifyEntry(code, name) == Success)
	{
		Paint(SP_PAINT);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::GUI::ComboBox::RemoveEntry(Int number)
{
	if (List::RemoveEntry(number) == Error) return Error;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ComboBox::RemoveAll()
{
	if (List::RemoveAll() == Error) return Error;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ComboBox::SelectEntry(Int id)
{
	List::SelectEntry(id);

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ComboBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface		*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Point		 realPos = GetRealPosition();
	ListEntry	*operat;
	Rect		 frame;
	Point		 lineStart;
	Point		 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (!(flags & CB_HOTSPOTONLY))
	{
		if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
		else		surface->Box(frame, Setup::BackgroundColor, FILLED);

		surface->Frame(frame, FRAME_DOWN);
	}

	frame.top++;
	frame.bottom--;
	frame.right--;
	frame.left = frame.right - METRIC_COMBOBOXOFFSETX;

	surface->Box(frame, Setup::BackgroundColor, FILLED);
	surface->Frame(frame, FRAME_UP);

	frame.top--;
	frame.bottom++;
	frame.right++;
	frame.left = realPos.x;

	lineStart.x = frame.right - METRIC_COMBOBOXARROWOFFSETX;
	lineStart.y = frame.top + METRIC_COMBOBOXARROWOFFSETY;
	lineEnd.x = lineStart.x + 7;
	lineEnd.y = lineStart.y;

	for (Int i = 0; i < 4; i++)
	{
		if (active)	surface->Line(lineStart, lineEnd, Setup::TextColor);
		else		surface->Line(lineStart, lineEnd, Setup::GrayTextColor);

		lineStart.x++;
		lineStart.y++;
		lineEnd.x--;
		lineEnd.y++;
	}

	if (!(flags & CB_HOTSPOTONLY))
	{
		if (!IsListSane()) SynchronizeList();

		for (Int j = 0; j < GetNOfEntries(); j++)
		{
			operat = GetNthEntry(j);

			if (operat->clicked)
			{
				frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
				frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
				frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

				surface->SetText(operat->name, frame, objectProperties->font);

				frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
				frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
				frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
			}
		}
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ComboBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer		*lay = (Layer *) myContainer->GetContainerObject();
	Window		*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface		*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Point		 realPos = GetRealPosition();
	Int		 retVal = Success;
	ListEntry	*operat;
	Rect		 frame;
	Rect		 lbframe;
	Point		 lbp;
	Size		 lbs;
	Bool		 executeProcs = False;
	Float		 oldMeasurement;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (flags & CB_HOTSPOTONLY)
	{
		frame.top	+= 3;
		frame.bottom	-= 3;
		frame.right	-= 3;
		frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;
	}

	switch (message)
	{
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
			if (message == WM_ACTIVATE && toolWindow != NIL) if (((HWND) lParam) == toolWindow->hwnd) break;
			if (message == WM_KILLFOCUS && toolWindow != NIL) if (((HWND) wParam) == toolWindow->hwnd) break;

			if (listBoxOpen)
			{
				listBoxOpen = False;

				toolWindow->Close();

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				DeleteObject(listBox);
				DeleteObject(layer);
				DeleteObject(toolWindow);

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;

				if (!wnd->IsMouseOn(frame)) wnd->Process(SM_LBUTTONDOWN, 0, 0);
			}

			break;
		case SM_LOOSEFOCUS:
			lbframe.top	= realPos.y + objectProperties->size.cy;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * GetNOfEntries() + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= realPos.x + objectProperties->size.cx - 1;
			lbframe.left	= realPos.x;

			if (wParam != handle)
			{
				if ((wnd->IsMouseOn(frame) && listBoxOpen) || (!wnd->IsMouseOn(frame) && !wnd->IsMouseOn(lbframe) && listBoxOpen))
				{
					listBoxOpen = False;

					toolWindow->Close();

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					DeleteObject(listBox);
					DeleteObject(layer);
					DeleteObject(toolWindow);

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			lbframe.top	= realPos.y + objectProperties->size.cy;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * GetNOfEntries() + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= realPos.x + objectProperties->size.cx - 1;
			lbframe.left	= realPos.x;

			if (wnd->IsMouseOn(frame) && !listBoxOpen)
			{
				wnd->Process(SM_LOOSEFOCUS, handle, 0);

				listBoxOpen = True;

				lbp.x = lbframe.left - lay->GetObjectProperties()->pos.x;
				lbp.y = realPos.y + objectProperties->size.cy - lay->GetObjectProperties()->pos.y;
				lbs.cx = objectProperties->size.cx;
				lbs.cy = min(METRIC_LISTBOXENTRYHEIGHT * GetNOfEntries() + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);

				oldMeasurement = Setup::FontSize;

				SetMeasurement(SMT_PIXELS);

				layer		= new Layer();
				toolWindow	= new ToolWindow();
				listBox		= new ListBox(Point(0, 0), lbs);

				listBox->onClick.Connect(&ComboBox::ListBoxProc, this);

				lbp.x = lbframe.left + wnd->GetObjectProperties()->pos.x;
				lbp.y = realPos.y + objectProperties->size.cy + wnd->GetObjectProperties()->pos.y;

				if (objectProperties->checked)
				{
					objectProperties->clicked = True;

					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;
					}

					surface->Frame(frame, FRAME_DOWN);
				}

				toolWindow->SetMetrics(lbp, lbs);

				Setup::FontSize = oldMeasurement;

				if (!IsListSane()) SynchronizeList();

				listBox->SetFlags(LF_ALLOWRESELECT);
				listBox->SetReferenceList(this);

				if (GetSelectedEntry() != NIL) listBox->SelectEntry(GetSelectedEntry()->id);

				wnd->RegisterObject(toolWindow);
				toolWindow->RegisterObject(layer);
				layer->RegisterObject(listBox);

				toolWindow->Show();

				listBox->Paint(SP_PAINT);

				retVal = Break;
			}
			else if ((wnd->IsMouseOn(frame) && listBoxOpen) || (!wnd->IsMouseOn(frame) && !wnd->IsMouseOn(lbframe) && listBoxOpen))
			{
				listBoxOpen = False;

				if (objectProperties->checked)
				{
					objectProperties->clicked = True;

					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;
					}

					surface->Frame(frame, FRAME_DOWN);

					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	-= 3;
						frame.bottom	+= 3;
						frame.right	+= 3;
						frame.left	= realPos.x;
					}
				}

				frame.top	= frame.bottom + 1;
				frame.bottom	= frame.top + min(METRIC_LISTBOXENTRYHEIGHT * GetNOfEntries() + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
				frame.right++;

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				DeleteObject(listBox);
				DeleteObject(layer);
				DeleteObject(toolWindow);

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;

				frame.left	= realPos.x;
				frame.top	= realPos.y;
				frame.right	= realPos.x + objectProperties->size.cx - 1;
				frame.bottom	= realPos.y + objectProperties->size.cy - 1;
			}

			break;
		case SM_LBUTTONUP:
			if (closeListBox)
			{
				if (!(flags & CB_HOTSPOTONLY))
				{
					for (Int i = 0; i < GetNOfEntries(); i++)
					{
						operat = GetNthEntry(i);

						if (operat->clicked)
						{
							frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

							Font	 font = objectProperties->font;

							font.SetColor(Setup::ClientColor);

							surface->SetText(operat->name, frame, font);

							frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
							frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
						}
					}
				}

				if (GetSelectedEntry() != listBox->GetSelectedEntry()) executeProcs = True;

				if (listBox->GetSelectedEntry() != NIL) SelectEntry(listBox->GetSelectedEntry()->id);

				if (!(flags & CB_HOTSPOTONLY))
				{
					for (Int j = 0; j < GetNOfEntries(); j++)
					{
						operat = GetNthEntry(j);

						if (operat->clicked)
						{
							frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

							surface->SetText(operat->name, frame, objectProperties->font);

							frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
							frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
							frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
						}
					}
				}

				if (listBoxOpen)
				{
					listBoxOpen = False;

					toolWindow->Close();

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					DeleteObject(listBox);
					DeleteObject(layer);
					DeleteObject(toolWindow);

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;
				}

				closeListBox = False;

				retVal = Break;
			}

			if (executeProcs)
			{
				for (Int i = 0; i < GetNOfEntries(); i++)
				{
					operat = GetNthEntry(i);

					if (operat->clicked)
					{
						onClick.Emit(wnd->MouseX(), wnd->MouseY());
						operat->onClick.Emit();

						break;
					}
				}
			}

			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;

				if (objectProperties->checked)
				{
					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;
					}

					surface->Frame(frame, FRAME_UP);
				}
				else
				{
					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 2;
						frame.right	-= 2;
						frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 3;
					}

					surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				}
			}

			break;
		case SM_MOUSEMOVE:
			if (!(flags & CB_HOTSPOTONLY))
			{
				frame.top	+= 3;
				frame.bottom	-= 3;
				frame.right	-= 3;
				frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;
			}

			if (wnd->IsMouseOn(frame) && !objectProperties->checked)
			{
				surface->Frame(frame, FRAME_UP);

				objectProperties->checked = True;
			}
			else if (!wnd->IsMouseOn(frame) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				surface->Box(frame, Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = False;
				objectProperties->clicked = False;
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::ComboBox::ListBoxProc()
{
	if (listBox != NIL)
	{
		closeListBox = True;
				
		toolWindow->SetOwner(this);
	}
}
