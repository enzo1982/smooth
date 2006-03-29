 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

	SetFont(Font(font.GetName(), I18N_DEFAULTFONTSIZE, Setup::ClientTextColor));

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

	RegisterObject(hotspot);
	RegisterObject(buttonHotspot);

	onChangeSize.Connect(&ComboBox::OnChangeSize, this);
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

	Surface		*surface	= container->GetDrawSurface();
	Rect		 frame		= Rect(GetRealPosition(), GetSize());
	Point		 lineStart;
	Point		 lineEnd;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (!(flags & CB_HOTSPOTONLY))
			{
				if (IsActive())	surface->Box(frame, Setup::ClientColor, FILLED);
				else		surface->Box(frame, Setup::BackgroundColor, FILLED);

				surface->Frame(frame, FRAME_DOWN);
			}

			surface->Box(frame + Point(GetWidth() - 18, 1) - Size(GetWidth() - 17, 2), Setup::BackgroundColor, FILLED);
			surface->Frame(frame + Point(GetWidth() - 18, 1) - Size(GetWidth() - 17, 2), FRAME_UP);

			lineStart	= Point(frame.right - 13 + (Setup::rightToLeft ? 1 : 0), frame.top + 8);
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

		if (prevSelectedEntry != entry)
		{
			Paint(SP_PAINT);

			if (flags & CB_HOTSPOTONLY) entry->Deselect();

			onSelectEntry.Emit(entry);
		}
	}
}

S::Void S::GUI::ComboBox::OpenListBox()
{
	if (listBox == NIL)
	{
		listBox		= new ListBox(Point(0, 0), Size(GetWidth(), 15 * Math::Min(GetNOfEntries(), 5) + 4));
		listBox->onSelectEntry.Connect(&ComboBox::OnSelectEntry, this);

		toolWindow	= new ToolWindow(GetRealPosition() + Point(0, GetHeight()), Size(GetWidth(), 15 * Math::Min(GetNOfEntries(), 5) + 4));
		toolWindow->onLoseFocus.Connect(&ComboBox::CloseListBox, this);

		listBox->SetFlags(LF_ALLOWRESELECT | LF_HIDEHEADER);
		listBox->AddTab("", 32768);

		prevSelectedEntry = GetSelectedEntry();

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

			ListEntry	*entry = (ListEntry *) GetNthObject(i);

			entry->SetRegisteredFlag(False);

			listBox->RegisterObject(entry);

			entry->Activate();
		}

		toolWindow->RegisterObject(listBox);

		RegisterObject(toolWindow);
	}
}

S::Void S::GUI::ComboBox::CloseListBox()
{
	if (listBox != NIL)
	{
		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

			ListEntry	*entry = (ListEntry *) GetNthObject(i);

			listBox->UnregisterObject(entry);

			entry->SetRegisteredFlag(True);
			entry->SetContainer(this);

			entry->Deactivate();
		}

		toolWindow->UnregisterObject(listBox);

		DeleteObject(listBox);

		listBox		= NIL;

		toolWindow->Close();

		UnregisterObject(toolWindow);

		DeleteObject(toolWindow);

		toolWindow	= NIL;
	}
}

S::Void S::GUI::ComboBox::OnChangeSize(const Size &nSize)
{
	hotspot->SetSize(nSize - Size(19, 2));

	buttonHotspot->SetHeight(nSize.cy - 6);
}
