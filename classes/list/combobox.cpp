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

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_COMBOBOX = S::Object::RequestObjectID();

S::GUI::ComboBox::ComboBox(Point pos, Size size)
{
	type		= OBJ_COMBOBOX;
	entryCount	= -1;

	closeListBox	= False;
	listBoxOpen	= False;
	listBox		= NIL;
	toolWindow	= NIL;
	layer		= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, Setup::ClientTextColor, objectProperties->fontWeight);

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
		delete listBox;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::List::Entry *S::GUI::ComboBox::AddEntry(String name)
{
	entryCount++;

	Entry *newEntry = AddListEntry(entryCount, name);

	if (entryCount == 0) entries.GetFirstEntry()->clk = True;

	Paint(SP_PAINT);

	return newEntry;
}

S::Int S::GUI::ComboBox::ModifyEntry(Int code, String name)
{
	if (ModifyListEntry(code, name) == Success)
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
	RemoveListEntry(number);

	Paint(SP_PAINT);

	return Success;
}

S::Void S::GUI::ComboBox::Cleanup()
{
	CleanupList();

	Paint(SP_PAINT);
}

S::Int S::GUI::ComboBox::SelectEntry(Int code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ComboBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface		*surface = myContainer->GetDrawSurface();
	Point		 realPos = GetRealPosition();
	List::Entry	*operat;
	Rect		 frame;
	Point		 lineStart;
	Point		 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
	else		surface->Box(frame, Setup::BackgroundColor, FILLED);

	surface->Frame(frame, FRAME_DOWN);

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

	for (Int j = 0; j < nOfEntries; j++)
	{
		operat = entries.GetNthEntry(j);

		if (operat->clk)
		{
			frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

			surface->SetText(operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

			frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
			frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
		}
	}

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
	Point		 realPos = GetRealPosition();
	Int		 retVal = Success;
	List::Entry	*operat;
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
				listBoxOpen = False;

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
				if ((wnd->IsMouseOn(frame) && listBoxOpen) || (!wnd->IsMouseOn(frame) && !wnd->IsMouseOn(lbframe) && listBoxOpen))
				{
					listBoxOpen = False;

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

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONDOWN:
			lbframe.top	= frame.bottom + 1;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= frame.right;
			lbframe.left	= frame.left;

			if (wnd->IsMouseOn(frame) && !listBoxOpen)
			{
				wnd->Process(SM_LOOSEFOCUS, handle, 0);

				listBoxOpen = True;

				lbp.x = frame.left - lay->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 - lay->GetObjectProperties()->pos.y;
				lbs.cx = objectProperties->size.cx;
				lbs.cy = min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);

				oldMeasurement = Setup::FontSize;

				SetMeasurement(SMT_PIXELS);

				layer		= new Layer();
				toolWindow	= new ToolWindow();
				listBox		= new ListBox(Point(0, 0), lbs);

				listBox->onClick.Connect(&ComboBox::ListBoxProc, this);

				lbp.x = frame.left + wnd->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 + wnd->GetObjectProperties()->pos.y;

				if (objectProperties->checked)
				{
					objectProperties->clicked = True;

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					surface->Frame(frame, FRAME_DOWN);
				}

				toolWindow->SetMetrics(lbp, lbs);

				Setup::FontSize = oldMeasurement;

				listBox->AllowReselect(True);

				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					listBox->AddEntry(operat->text);
				}

				listBox->SelectEntry(GetSelectedEntry());

				wnd->RegisterObject(toolWindow);
				toolWindow->RegisterObject(layer);
				layer->RegisterObject(listBox);

				toolWindow->SetOwner(this);

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

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					surface->Frame(frame, FRAME_DOWN);

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

				if (!wnd->IsMouseOn(frame) && !wnd->IsMouseOn(lbframe)) wnd->Process(SM_LBUTTONDOWN, 0, 0);

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (closeListBox)
			{
				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						surface->SetText(operat->text, frame, objectProperties->font, objectProperties->fontSize, Setup::ClientColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (GetSelectedEntry() != listBox->GetSelectedEntry()) executeProcs = True;

				SelectEntry(listBox->GetSelectedEntry());

				for (Int j = 0; j < nOfEntries; j++)
				{
					operat = entries.GetNthEntry(j);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						surface->SetText(operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (listBoxOpen)
				{
					listBoxOpen = False;

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

				closeListBox = False;

				retVal = Break;
			}

			if (executeProcs)
			{
				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						onClick.Emit();
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
					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					surface->Frame(frame, FRAME_UP);
				}
				else
				{
					frame.top	+= 3;
					frame.bottom	-= 2;
					frame.right	-= 2;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 3;

					surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				}
			}

			break;
		case SM_MOUSELEAVE:
			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

			if (!wnd->IsMouseOn(frame) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				surface->Box(frame, Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = False;
				objectProperties->clicked = False;
			}

			break;
		case SM_MOUSEMOVE:
			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

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
		case SM_CHECKCOMBOBOXES:
			if (listBox != NIL)
			{
				closeListBox = True;
				
				toolWindow->SetOwner(this);

				retVal = Break;
			}

			break;
	}

	return retVal;
}

S::Void S::GUI::ComboBox::ListBoxProc()
{
	Process(SM_CHECKCOMBOBOXES, 0, 0);
}
