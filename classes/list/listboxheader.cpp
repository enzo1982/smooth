 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listboxheader.h>
#include <smooth/surface.h>
#include <smooth/listbox.h>
#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/objectproperties.h>
#include <smooth/toolkit.h>
#include <smooth/metrics.h>
#include <smooth/math.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_LISTBOXHEADER = S::Object::RequestObjectID();

S::GUI::ListBoxHeader::ListBoxHeader(ListBox *iListBox)
{
	listBox	= iListBox;

	moveTab	= -1;
	innerLoop = False;

	type	= OBJ_LISTBOXHEADER;

	possibleContainers.AddEntry(OBJ_LAYER);

	UpdateMetrics();
}

S::GUI::ListBoxHeader::~ListBoxHeader()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::ListBoxHeader::AddTab(String tabName, Int iTabWidth)
{
	tabNames.AddEntry(tabName);
	tabWidths.AddEntry(iTabWidth);
	tabChecked.AddEntry(False);

	UpdateMetrics();

	return Success;
}

S::Int S::GUI::ListBoxHeader::ClearTabs()
{
	tabNames.DeleteAll();
	tabWidths.DeleteAll();
	tabChecked.DeleteAll();

	UpdateMetrics();

	return Success;
}

S::Int S::GUI::ListBoxHeader::UpdateMetrics()
{
	objectProperties->pos.x = listBox->GetObjectProperties()->pos.x + 1;
	objectProperties->pos.y = listBox->GetObjectProperties()->pos.y + 1;

	objectProperties->size.cx = listBox->GetObjectProperties()->size.cx - 3;
	objectProperties->size.cy = METRIC_LISTBOXENTRYHEIGHT;

	Int	 varSizeTabs = 0;
	Int	 sumFixedTabSizes = 0;

	for (Int i = 0; i < tabWidths.GetNOfEntries(); i++)
	{
		if (tabWidths.GetNthEntry(i) <= 0)	varSizeTabs++;
		else					sumFixedTabSizes += tabWidths.GetNthEntry(i);
	}

	for (Int j = 0; j < tabWidths.GetNOfEntries(); j++)
	{
		if (tabWidths.GetNthEntry(j) <= 0) tabWidths.SetEntry(tabWidths.GetNthEntryIndex(j), -max(0, (objectProperties->size.cx - sumFixedTabSizes) / varSizeTabs));
	}

	return Success;
}

S::Int S::GUI::ListBoxHeader::GetNOfTabs()
{
	return tabWidths.GetNOfEntries();
}

S::Int S::GUI::ListBoxHeader::GetNthTabOffset(Int n)
{
	if (n >= tabWidths.GetNOfEntries()) return -1;

	Int	 offset = 0;

	for (Int i = 0; i < n; i++)
	{
		offset += (Int) Math::Abs(tabWidths.GetNthEntry(i));
	}

	return offset;
}

S::Int S::GUI::ListBoxHeader::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx;
			frame.bottom	= realPos.y + objectProperties->size.cy;

			surface->Box(frame, Setup::BackgroundColor, FILLED);
			surface->Frame(frame, FRAME_UP);

			for (Int i = 0; i < tabWidths.GetNOfEntries(); i++)
			{
				frame.right = (Int) Math::Min(frame.left + Math::Abs(tabWidths.GetNthEntry(i)), realPos.x + objectProperties->size.cx);

				surface->Box(frame, Setup::BackgroundColor, FILLED);
				surface->Frame(frame, FRAME_UP);

				frame.left += 3;
				frame.top += 1;

				if (active)	surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, FW_BOLD);
				else		surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font, objectProperties->fontSize, Setup::GrayTextColor, FW_BOLD);

				frame.top -= 1;
				frame.left += (Int) (Math::Abs(tabWidths.GetNthEntry(i)) - 2);
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ListBoxHeader::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (innerLoop) break;

			frame.left = realPos.x - 3;

			for (Int j = 0; j < tabWidths.GetNOfEntries() - 1; j++)
			{
				frame.left += (Int) (Math::Abs(tabWidths.GetNthEntry(j)) + 1);
				frame.right = frame.left + 4;

				if (wnd->IsMouseOn(frame))
				{
					if (moveTab != j)
					{
						moveTab = j;

						wnd->cursorset = True;

						LiSASetMouseCursor(LiSA_MOUSE_HSIZE);
					}
				}
				else if (moveTab == j)
				{
					moveTab = -1;

					wnd->cursorset = False;

					LiSASetMouseCursor(LiSA_MOUSE_ARROW);
				}
			}

			frame.left = realPos.x;

			for (Int i = 0; i < tabWidths.GetNOfEntries(); i++)
			{
				frame.right = (Int) Math::Min(frame.left + Math::Abs(tabWidths.GetNthEntry(i)), realPos.x + objectProperties->size.cx);

				frame.left++;
				frame.top++;

				if (wnd->IsMouseOn(frame) && !tabChecked.GetNthEntry(i) && moveTab == -1)
				{
					surface->Box(frame, Setup::LightGrayColor, FILLED);

					frame.left += 2;
					surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, FW_BOLD);
					frame.left -= 2;

					tabChecked.SetEntry(tabChecked.GetNthEntryIndex(i), True);
				}
				else if ((!wnd->IsMouseOn(frame) || moveTab != -1) && tabChecked.GetNthEntry(i))
				{
					surface->Box(frame, Setup::BackgroundColor, FILLED);

					frame.left += 2;
					surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, FW_BOLD);
					frame.left -= 2;

					tabChecked.SetEntry(tabChecked.GetNthEntryIndex(i), False);
				}

				frame.left--;
				frame.top--;

				frame.left += (Int) (Math::Abs(tabWidths.GetNthEntry(i)) + 1);
			}

			break;
		case SM_LBUTTONDOWN:
			if (moveTab != -1)
			{
				Int	 leftButton;

				if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
				else					leftButton = VK_LBUTTON;

				Point	 om;

				{
					POINT	 mp = om;

					GetCursorPos(&mp);

					om = mp;
				}

				innerLoop = True;

				do
				{
					MSG	 msg;

					if (peekLoop > 0)
					{
						if (Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
						else				PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);
					}
					else
					{
						if (Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
						else				GetMessageA(&msg, NIL, 0, 0);
					}

					TranslateMessage(&msg);

					if (Setup::enableUnicode)	DispatchMessageW(&msg);
					else				DispatchMessageA(&msg);

					if (peekLoop > 0)
					{
						if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
						else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
					}

					Point	 m;

					{
						POINT	 mp = m;

						GetCursorPos(&mp);

						m = mp;
					}

					Int	 bias = om.x - m.x;

					if (bias != 0)
					{
						tabWidths.SetEntry(moveTab, (Int) Math::Abs(tabWidths.GetEntry(moveTab)) - bias);
						tabWidths.SetEntry(moveTab + 1, (Int) Math::Abs(tabWidths.GetEntry(moveTab + 1)) + bias);

						om.x = m.x;

						UpdateMetrics();

						surface->StartPaint();
						listBox->Paint(SP_PAINT);
						surface->EndPaint();
					}
				}
				while (GetAsyncKeyState(leftButton) != 0);

				innerLoop = False;
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}
