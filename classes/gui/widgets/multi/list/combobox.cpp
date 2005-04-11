 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/combobox.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/misc/i18n.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::ComboBox::classID = S::Object::RequestClassID();

S::GUI::ComboBox::ComboBox(Point iPos, Size iSize)
{
	type		= classID;

	listBox		= NIL;
	toolWindow	= NIL;
	layer		= NIL;

	closeListBox	= False;

	possibleContainers.AddEntry(Layer::classID);

	SetFont(Font(font.GetName(), I18N_DEFAULTFONTSIZE, Setup::ClientTextColor));

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 19;
}

S::GUI::ComboBox::~ComboBox()
{
	CloseListBox();
}

S::Int S::GUI::ComboBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	if (GetSelectedEntry() == NIL && !(flags & CB_HOTSPOTONLY))
	{
		if (GetNthObject(0) != NIL) ((ListEntry *) GetNthObject(0))->clicked = True;
	}

	Surface		*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Point		 realPos	= GetRealPosition();
	Rect		 frame		= Rect(GetRealPosition(), size);
	Point		 lineStart;
	Point		 lineEnd;

	if (!(flags & CB_HOTSPOTONLY))
	{
		if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
		else		surface->Box(frame, Setup::BackgroundColor, FILLED);

		surface->Frame(frame, FRAME_DOWN);
	}

	frame.top++;
	frame.bottom--;
	frame.right--;
	frame.left = frame.right - 17;

	surface->Box(frame, Setup::BackgroundColor, FILLED);
	surface->Frame(frame, FRAME_UP);

	frame.top--;
	frame.bottom++;
	frame.right++;
	frame.left = realPos.x;

	lineStart.x = frame.right - 13 + (Setup::rightToLeft ? 1 : 0);
	lineStart.y = frame.top + 8;
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
		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			ListEntry	*operat = (ListEntry *) GetNthObject(j);

			if (operat->clicked)
			{
				frame.left	+= 3;
				frame.top	+= 3;
				frame.right	-= 18;

				String	 nText = operat->GetText();

				for (Int k = 0; k < operat->GetText().Length(); k++)
				{
					if (operat->GetText()[k] == '\t')	nText[k] = 0;
					else					nText[k] = operat->GetText()[k];
				}

				surface->SetText(nText, frame, font);

				frame.right	+= 18;
				frame.left	-= 3;
				frame.top	-= 3;
			}
		}
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ComboBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Layer		*lay		= (Layer *) container;
	Window		*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface		*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Point		 realPos	= GetRealPosition();
	Int		 retVal		= Success;
	ListEntry	*operat;
	Rect		 frame		= Rect(GetRealPosition(), size);
	Rect		 lbframe;
	Point		 lbp;
	Size		 lbs;
	Bool		 executeProcs	= False;

	if (flags & CB_HOTSPOTONLY)
	{
		frame.top	+= 3;
		frame.bottom	-= 3;
		frame.right	-= 3;
		frame.left	= frame.right - 13;
	}

	switch (message)
	{
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
			if (message == WM_ACTIVATE && toolWindow != NIL) if ((HWND) lParam == (HWND) toolWindow->GetSystemWindow()) break;
			if (message == WM_KILLFOCUS && toolWindow != NIL) if ((HWND) wParam == (HWND) toolWindow->GetSystemWindow()) break;

			if (listBox != NIL)
			{
				CloseListBox();

				if (!wnd->IsMouseOn(frame)) wnd->Process(SM_LBUTTONDOWN, 0, 0);
			}

			break;
		case SM_LOOSEFOCUS:
			lbframe.top	= realPos.y + size.cy;
			lbframe.bottom	= lbframe.top + Math::Min((Int) (15 * GetNOfObjects() + 4), 15 * 5 + 4);
			lbframe.right	= realPos.x + size.cx - 1;
			lbframe.left	= realPos.x;

			if (wParam != GetHandle())
			{
				if (listBox != NIL && (wnd->IsMouseOn(frame) || !wnd->IsMouseOn(lbframe)))
				{
					CloseListBox();

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			lbframe.top	= realPos.y + size.cy;
			lbframe.bottom	= lbframe.top + Math::Min((Int) (15 * GetNOfObjects() + 4), 15 * 5 + 4);
			lbframe.right	= realPos.x + size.cx - 1;
			lbframe.left	= realPos.x;

			if (wnd->IsMouseOn(frame) && listBox == NIL)
			{
				wnd->Process(SM_LOOSEFOCUS, GetHandle(), 0);

				lbp.x = lbframe.left - lay->pos.x;
				lbp.y = realPos.y + size.cy - lay->pos.y;
				lbs.cx = size.cx;
				lbs.cy = Math::Min((Int) (15 * GetNOfObjects() + 4), 15 * 5 + 4);

				layer		= new Layer();
				toolWindow	= new ToolWindow();
				listBox		= new ListBox(Point(0, 0), lbs);

				listBox->onClick.Connect(&ComboBox::ListBoxProc, this);

				lbp.x = lbframe.left + wnd->pos.x;
				lbp.y = realPos.y + size.cy + wnd->pos.y;

				if (checked)
				{
					clicked = True;

					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - 13;
					}

					surface->Frame(frame, FRAME_DOWN);
				}

				toolWindow->SetMetrics(lbp, lbs);

				listBox->SetFlags(LF_ALLOWRESELECT | LF_HIDEHEADER);
				listBox->AddTab("", 32768);

				prevSelectedEntry = GetSelectedEntry();

				for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
				{
					ListEntry	*entry = (ListEntry *) assocObjects.GetNthEntry(i);

					entry->SetRegisteredFlag(False);

					listBox->RegisterObject(entry);
				}

				wnd->RegisterObject(toolWindow);
				toolWindow->RegisterObject(layer);
				layer->RegisterObject(listBox);

				toolWindow->Show();

				listBox->Paint(SP_PAINT);

				retVal = Break;
			}
			else if (listBox != NIL && (wnd->IsMouseOn(frame) || !wnd->IsMouseOn(lbframe)))
			{
				if (checked)
				{
					clicked = True;

					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - 13;
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
				frame.bottom	= frame.top + Math::Min((Int) (15 * GetNOfObjects() + 4), 15 * 5 + 4);
				frame.right++;
	
				CloseListBox();
			}

			break;
		case SM_LBUTTONUP:
			if (closeListBox)
			{
				CloseListBox();

				closeListBox = False;

				if (!(flags & CB_HOTSPOTONLY))
				{
					frame.left	+= 3;
					frame.top	+= 3;
					frame.right	-= 18;

					String	 nText;

					if (prevSelectedEntry != NIL)
					{
						Font	 nFont = font;

						nFont.SetColor(Setup::ClientColor);

						nText = prevSelectedEntry->GetText();

						for (Int k = 0; k < prevSelectedEntry->GetText().Length(); k++)
						{
							if (prevSelectedEntry->GetText()[k] == '\t')	nText[k] = 0;
							else						nText[k] = prevSelectedEntry->GetText()[k];
						}

						surface->SetText(nText, frame, nFont);
					}

					ListEntry	*entry = GetSelectedEntry();

					nText = entry->GetText();

					for (Int l = 0; l < entry->GetText().Length(); l++)
					{
						if (entry->GetText()[l] == '\t')	nText[l] = 0;
						else					nText[l] = entry->GetText()[l];
					}

					surface->SetText(nText, frame, font);

					frame.right	+= 18;
					frame.left	-= 3;
					frame.top	-= 3;
				}

				if (prevSelectedEntry != GetSelectedEntry()) executeProcs = True;

				retVal = Break;
			}

			if (executeProcs)
			{
				for (Int i = 0; i < GetNOfObjects(); i++)
				{
					operat = (ListEntry *) GetNthObject(i);

					if (operat->clicked)
					{
						onClick.Emit(wnd->MouseX(), wnd->MouseY());
						operat->onClick.Emit(wnd->MouseX(), wnd->MouseY());

						break;
					}
				}
			}

			if (clicked)
			{
				clicked = False;

				if (checked)
				{
					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - 13;
					}

					surface->Frame(frame, FRAME_UP);
				}
				else
				{
					if (!(flags & CB_HOTSPOTONLY))
					{
						frame.top	+= 3;
						frame.bottom	-= 3;
						frame.right	-= 3;
						frame.left	= frame.right - 13;
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
				frame.left	= frame.right - 13;
			}

			if (wnd->IsMouseOn(frame) && !checked)
			{
				surface->Frame(frame, FRAME_UP);

				checked = True;
			}
			else if (!wnd->IsMouseOn(frame) && checked)
			{
				surface->Box(frame, Setup::BackgroundColor, OUTLINED);

				checked = False;
				clicked = False;
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

S::Void S::GUI::ComboBox::CloseListBox()
{
	if (!registered) return;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return;

	if (listBox != NIL)
	{
		for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
		{
			ListEntry	*entry = (ListEntry *) assocObjects.GetNthEntry(i);

			listBox->UnregisterObject(entry);

			entry->SetRegisteredFlag(True);
			entry->SetContainer(this);
		}

		toolWindow->FreeOwner();
		toolWindow->Close();

		wnd->UnregisterObject(toolWindow);
		toolWindow->UnregisterObject(layer);
		layer->UnregisterObject(listBox);

		DeleteObject(listBox);
		DeleteObject(layer);
		DeleteObject(toolWindow);

		listBox		= NIL;
		layer		= NIL;
		toolWindow	= NIL;
	}
}
