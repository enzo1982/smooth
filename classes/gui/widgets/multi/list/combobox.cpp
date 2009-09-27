 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/combobox.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/misc/math.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::ComboBox::classID = S::Object::RequestClassID();

S::GUI::ComboBox::ComboBox(const Point &iPos, const Size &iSize)
{
	type		= classID;

	listBox		= NIL;
	toolWindow	= NIL;

	SetFont(Font(font.GetName(), Font::DefaultSize, Font::Normal, Font::Normal, Setup::ClientTextColor));

	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(19);

	hotspot		= new Hotspot(Point(1, 1), GetSize() - Size(19, 2));

	hotspot->onLeftButtonDown.Connect(&ComboBox::OpenListBox, this);
	hotspot->onLoseFocus.Connect(&ComboBox::CloseListBox, this);

	buttonHotspot	= new HotspotSimpleButton(Point(16, 3), Size(13, GetHeight() - 6));
	buttonHotspot->SetOrientation(OR_UPPERRIGHT);

	buttonHotspot->onLeftButtonDown.Connect(&ComboBox::OpenListBox, this);
	buttonHotspot->onLoseFocus.Connect(&ComboBox::CloseListBox, this);

	Add(hotspot);
	Add(buttonHotspot);

	onChangeSize.Connect(&ComboBox::OnChangeSize, this);
	onSelectEntry.Connect(&ComboBox::OnSelectEntry, this);
}

S::GUI::ComboBox::~ComboBox()
{
	DeleteObject(hotspot);
	DeleteObject(buttonHotspot);

	CloseListBox();
}

S::Int S::GUI::ComboBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	if (GetSelectedEntry() == NIL && !(flags & CB_HOTSPOTONLY))
	{
		if (GetNthEntry(0) != NIL) ((ListEntry *) GetNthEntry(0))->Select();
	}

	if (flags & CB_HOTSPOTONLY)	hotspot->Deactivate();
	else				hotspot->Activate();

	Surface		*surface	= GetDrawSurface();
	Rect		 frame		= Rect(GetRealPosition(), GetSize());
	Point		 lineStart;
	Point		 lineEnd;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (!(flags & CB_HOTSPOTONLY))
			{
				if (IsActive())	surface->Box(frame, Setup::ClientColor, Rect::Filled);
				else		surface->Box(frame, Setup::BackgroundColor, Rect::Filled);

				surface->Frame(frame, FRAME_DOWN);
			}

			surface->Box(frame + Point(GetWidth() - 18, 1) - Size(GetWidth() - 17, 2), Setup::BackgroundColor, Rect::Filled);
			surface->Frame(frame + Point(GetWidth() - 18, 1) - Size(GetWidth() - 17, 2), FRAME_UP);

			lineStart	= Point(frame.right - 13 + (IsRightToLeft() ? 1 : 0), frame.top + 8);
			lineEnd		= lineStart + Point(7, 0);

			for (Int i = 0; i < 4; i++)
			{
				if (IsActive())	surface->Line(lineStart, lineEnd, Setup::TextColor);
				else		surface->Line(lineStart, lineEnd, Setup::GrayTextColor);

				lineStart += Point(1, 1);
				lineEnd += Point(-1, 1);
			}

			if (!(flags & CB_HOTSPOTONLY))
			{
				for (Int j = 0; j < GetNOfObjects(); j++)
				{
					if (GetNthObject(j)->GetObjectType() != ListEntry::classID) continue;

					ListEntry	*operat = (ListEntry *) GetNthObject(j);

					if (operat->IsSelected())
					{
						String	 nText = operat->GetText();

						for (Int k = 0; k < operat->GetText().Length(); k++)
						{
							if (operat->GetText()[k] == '\t')	nText[k] = 0;
							else					nText[k] = operat->GetText()[k];
						}

						surface->SetText(nText, frame + Point(3, 3) - Size(21, 0), font);
					}
				}
			}

			break;
	}

	return Success();
}

S::Void S::GUI::ComboBox::OnSelectEntry(ListEntry *entry)
{
	if (listBox != NIL)
	{
		CloseListBox();
	}

	if (prevSelectedEntry != entry)
	{
		Paint(SP_PAINT);

		if (flags & CB_HOTSPOTONLY) entry->Deselect();

		prevSelectedEntry = entry;
	}
}

S::Void S::GUI::ComboBox::OpenListBox()
{
	if (listBox == NIL)
	{
		Widget	*window		= container->GetContainerWindow();
 
		Size	 listBoxSize	= Size(GetWidth(), 15 * Math::Min(Length(), Math::Max(5, Math::Min(15, Length() / 3))) + 4);
		Point	 listBoxPos	= Point(GetRealPosition() + Point(0, GetHeight()));

		if (window->GetY() + listBoxPos.y + listBoxSize.cy >= LiSAGetDisplaySizeY()) listBoxPos = Point(GetRealPosition() - Point(0, listBoxSize.cy));

		listBox		= new ListBox(Point(), listBoxSize);
		listBox->onSelectEntry.Connect(&onSelectEntry);

		toolWindow	= new ToolWindow(listBoxPos, listBoxSize);
		toolWindow->onLoseFocus.Connect(&ComboBox::CloseListBox, this);

		listBox->SetFlags(LF_ALLOWRESELECT | LF_HIDEHEADER);
		listBox->AddTab(NIL, 32768);

		prevSelectedEntry = GetSelectedEntry();

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

			ListEntry	*entry = (ListEntry *) GetNthObject(i);

			entry->SetRegisteredFlag(False);

			listBox->Add(entry);

			entry->Activate();
		}

		toolWindow->Add(listBox);

		Add(toolWindow);
	}
}

S::Void S::GUI::ComboBox::CloseListBox()
{
	if (listBox != NIL)
	{
		listBox->RemoveAllEntries();

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

			ListEntry	*entry = (ListEntry *) GetNthObject(i);

			entry->Hide();
			entry->Deactivate();

			entry->SetRegisteredFlag(True);
			entry->SetContainer(this);
		}

		toolWindow->Remove(listBox);

		DeleteObject(listBox);

		listBox		= NIL;

		toolWindow->Close();

		Remove(toolWindow);

		DeleteObject(toolWindow);

		toolWindow	= NIL;
	}
}

S::Void S::GUI::ComboBox::OnChangeSize(const Size &nSize)
{
	hotspot->SetSize(nSize - Size(19, 2));

	buttonHotspot->SetHeight(nSize.cy - 6);
}
