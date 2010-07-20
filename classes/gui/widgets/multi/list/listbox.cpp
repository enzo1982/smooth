 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/basic/scrollbar.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>

const S::Int	 S::GUI::ListBox::classID = S::Object::RequestClassID();

S::GUI::ListBox::ListBox(const Point &iPos, const Size &iSize)
{
	type			= classID;

	scrollbar		= NIL;
	scrollbarPos		= 0;

	visibleEntriesChecksum	= 0;

	SetFont(Font(font.GetName(), Font::DefaultSize, Font::Normal, Font::Normal, Setup::ClientTextColor));

	SetBackgroundColor(Setup::ClientColor);
	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&ListBox::OnChangeSize, this);

	scrollbar = new Scrollbar(Point(), Size(), OR_VERT, &scrollbarPos, 0, 1);
	scrollbar->SetOrientation(OR_UPPERRIGHT);
	scrollbar->SetStepSize(15);
	scrollbar->onValueChange.Connect(&ListBox::OnScrollbarValueChange, this);
	scrollbar->Hide();

	header = new ListBoxHeader(Point(1, 1), Size(GetWidth() - 2, 16));
	header->Hide();

	Add(scrollbar);
	Add(header);
}

S::GUI::ListBox::~ListBox()
{
	DeleteObject(scrollbar);
	DeleteObject(header);
}

S::Int S::GUI::ListBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());
	Rect	 entryRect	= frame;

	String	 visibleEntries;

	if (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) header->Show();
	else						  header->Hide();

	Int	 entriesHeight	= 0;
	Int	 headerHeight	= (header->IsVisible() ? 16 : 0);

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(frame);

			entriesHeight = GetEntriesHeight();

			if (entriesHeight > GetHeight() - headerHeight - 4 && !(flags & LF_HIDESCROLLBAR))
			{
				scrollbar->SetMetrics(Point(18, 1 + headerHeight), Size(scrollbar->GetWidth(), GetHeight() - 2 - headerHeight));
				scrollbar->SetRange(0, entriesHeight - (GetHeight() - 4 - headerHeight));

				scrollbar->SetPageSize(GetHeight() - 4 - headerHeight);

				scrollbar->Show();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos = 0;
			}

			/* Set visibility of list entries first.
			 */
			entryRect.top	 = -scrollbarPos;
			entryRect.bottom = -scrollbarPos;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				entry->SetVisibleDirect(False);

				entryRect.bottom += entry->GetHeight();

				if (entryRect.bottom >= 0 && entryRect.top <= GetHeight() - headerHeight - 4)
				{
					entry->SetMetrics(Point(2, entryRect.top + 2 + headerHeight), Size(GetWidth() - 4 - (scrollbar->IsVisible() ? 17 : 0), entry->GetHeight()));
					entry->SetVisibleDirect(True);

					visibleEntries.Append(entry->GetName());
				}

				entryRect.top += entry->GetHeight();
			}

			visibleEntriesChecksum = visibleEntries.ComputeCRC32();

			/* Now paint the listbox and all entries.
			 */
			if (IsActive())	surface->Box(frame + Point(0, headerHeight) - Size(scrollbar->IsVisible() ? 18 : 0, headerHeight), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame + Point(0, headerHeight) - Size(scrollbar->IsVisible() ? 18 : 0, headerHeight), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			entriesHeight = GetEntriesHeight();

			if (entriesHeight > GetHeight() - headerHeight - 4 && !(flags & LF_HIDESCROLLBAR))
			{
				if (!scrollbar->IsVisible())	Paint(SP_PAINT);
				else 				scrollbar->SetRange(0, entriesHeight - (GetHeight() - 4 - headerHeight));
			}

			frame.top = -scrollbarPos;
			frame.bottom = frame.top;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				frame.bottom += entry->GetHeight();

				if (frame.bottom >= 0 && frame.top <= GetHeight() - headerHeight - 4)
				{
					visibleEntries.Append(entry->GetName());
				}

				frame.top += entry->GetHeight();

				if (frame.top > GetHeight() - headerHeight - 4) break;
			}

			if (visibleEntriesChecksum != visibleEntries.ComputeCRC32()) Paint(SP_PAINT);

			break;
	}

	return Success();
}

S::Int S::GUI::ListBox::DragSelectedEntry(Bool upDown)
{
	if (!(flags & LF_ALLOWREORDER)) return Error();

	Int	 selectedEntryNumber = GetSelectedEntryNumber();
	Int	 exchangedEntryNumber = selectedEntryNumber;

	if	( upDown && selectedEntryNumber != 0)		 exchangedEntryNumber = selectedEntryNumber - 1;
	else if (!upDown && selectedEntryNumber != Length() - 1) exchangedEntryNumber = selectedEntryNumber + 1;

	Int	 oldSbPos = scrollbarPos;

	if	(exchangedEntryNumber < scrollbarPos)		  scrollbarPos--;
	else if (!GetNthEntry(exchangedEntryNumber)->IsVisible()) scrollbarPos++;

	if (selectedEntryNumber != exchangedEntryNumber)
	{
		SwitchEntries(selectedEntryNumber, exchangedEntryNumber);

		Paint(SP_PAINT);

		if (scrollbarPos != oldSbPos) System::System::Sleep(100);
	}

	return Success();
}

S::GUI::Rect S::GUI::ListBox::GetVisibleArea() const
{
	if (!IsVisible()) return Widget::GetVisibleArea();
	else		  return Widget::GetVisibleArea() + Point(0, 2 + (header->IsVisible() ? 16 : 0)) - Size(0, 4 + (header->IsVisible() ? 16 : 0));
}

S::Int S::GUI::ListBox::GetEntriesHeight() const
{
	Int	 entriesHeight	= 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		entriesHeight += GetNthObject(i)->GetHeight();
	}

	return entriesHeight;
}

S::Void S::GUI::ListBox::OnScrollbarValueChange()
{
	Paint(SP_PAINT);
}

S::Void S::GUI::ListBox::OnChangeSize(const Size &nSize)
{
	if (scrollbar->IsVisible())
	{
		Int	 headerHeight = (header->IsVisible() ? 16 : 0);

		scrollbar->SetHeight(nSize.cy - 2 - headerHeight);
		scrollbar->SetRange(0, GetEntriesHeight() - (GetHeight() - 4 - headerHeight));

		scrollbar->SetPageSize(GetHeight() - 4 - headerHeight);
	}

	header->SetWidth(nSize.cx - 2);
}
