 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listboxheader.h>
#include <smooth/graphics/surface.h>
#include <smooth/listbox.h>
#include <smooth/window/window.h>
#include <smooth/layer.h>
#include <smooth/objectproperties.h>
#include <smooth/misc/math.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::ListBoxHeader::classID = S::Object::RequestClassID();

S::GUI::ListBoxHeader::ListBoxHeader(ListBox *iListBox)
{
	listBox		= iListBox;

	moveTab		= -1;
	innerLoop	= False;

	type		= classID;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->font.SetWeight(FW_BOLD);

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
	tabNames.RemoveAll();
	tabWidths.RemoveAll();
	tabChecked.RemoveAll();

	UpdateMetrics();

	return Success;
}

S::Int S::GUI::ListBoxHeader::UpdateMetrics()
{
	objectProperties->pos.x = listBox->GetObjectProperties()->pos.x + 1;
	objectProperties->pos.y = listBox->GetObjectProperties()->pos.y + 1;

	objectProperties->size.cx = listBox->GetObjectProperties()->size.cx - 3;
	objectProperties->size.cy = 15;

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

S::Int S::GUI::ListBoxHeader::GetNthTabWidth(Int n)
{
	return (Int) Math::Abs(tabWidths.GetNthEntry(n));
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

				Font	 font = objectProperties->font;

				if (!active) font.SetColor(Setup::GrayTextColor);

				surface->SetText(tabNames.GetNthEntry(i), frame, font);

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
	Int	 i = 0;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (innerLoop) break;

			frame.left = realPos.x - 3;

			for (i = 0; i < tabWidths.GetNOfEntries() - 1; i++)
			{
				frame.left += (Int) (Math::Abs(tabWidths.GetNthEntry(i)) + 1);
				frame.right = frame.left + 4;

				if (wnd->IsMouseOn(frame))
				{
					if (moveTab != i)
					{
						moveTab = i;

						LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_HSIZE);
					}
				}
				else if (moveTab == i)
				{
					moveTab = -1;

					LiSASetMouseCursor((HWND) wnd->GetSystemWindow(), LiSA_MOUSE_ARROW);
				}
			}

			frame.left = realPos.x;

			for (i = 0; i < tabWidths.GetNOfEntries(); i++)
			{
				frame.right = (Int) Math::Min(frame.left + Math::Abs(tabWidths.GetNthEntry(i)), realPos.x + objectProperties->size.cx);

				frame.left++;
				frame.top++;

				if (wnd->IsMouseOn(frame) && !tabChecked.GetNthEntry(i) && moveTab == -1)
				{
					surface->Box(frame, Setup::LightGrayColor, FILLED);

					frame.left += 2;
					surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font);
					frame.left -= 2;

					tabChecked.SetEntry(tabChecked.GetNthEntryIndex(i), True);
				}
				else if ((!wnd->IsMouseOn(frame) || moveTab != -1) && tabChecked.GetNthEntry(i))
				{
					surface->Box(frame, Setup::BackgroundColor, FILLED);

					frame.left += 2;
					surface->SetText(tabNames.GetNthEntry(i), frame, objectProperties->font);
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

				Int	 omx = wnd->MouseX();

				innerLoop = True;

				System::EventProcessor	*event = new System::EventProcessor();

				do
				{
					if (peekLoop > 0)	event->ProcessNextEvent(False);
					else			event->ProcessNextEvent(True);

					Int	 mx = wnd->MouseX();
					Int	 bias = omx - mx;

					if (bias != 0)
					{
						tabWidths.SetEntry(moveTab, (Int) Math::Max(Math::Abs(tabWidths.GetEntry(moveTab)) - bias, 1) * Math::Sign(tabWidths.GetEntry(moveTab)));
						tabWidths.SetEntry(tabWidths.GetNOfEntries() - 1, (Int) Math::Max(Math::Abs(tabWidths.GetEntry(tabWidths.GetNOfEntries() - 1)) + bias, 1) * Math::Sign(tabWidths.GetEntry(tabWidths.GetNOfEntries() - 1)));

						omx = mx;

						UpdateMetrics();

						surface->StartPaint(Rect(listBox->GetRealPosition(), listBox->GetObjectProperties()->size));
						listBox->Paint(SP_PAINT);
						surface->EndPaint();
					}
				}
				while (GetAsyncKeyState(leftButton) != 0);

				delete event;

				innerLoop = False;
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}
