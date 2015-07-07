 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
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

const S::Short	 S::GUI::ListBox::classID = S::Object::RequestClassID();

S::GUI::ListBox::ListBox(const Point &iPos, const Size &iSize)
{
	type	     = classID;

	scrollbar    = NIL;
	scrollbarPos = 0;

	SetFont(Font(font.GetName(), Font::DefaultSize, Font::Normal, Font::Normal, Setup::ClientTextColor));

	SetBackgroundColor(Setup::ClientColor);
	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&ListBox::OnChangeSize, this);

	scrollbar = new Scrollbar(Point(), Size(), OR_VERT, &scrollbarPos, 0, 1);
	scrollbar->SetOrientation(OR_UPPERRIGHT);
	scrollbar->SetStepSize(16);
	scrollbar->onValueChange.Connect(&ListBox::OnScrollbarValueChange, this);
	scrollbar->Hide();

	header = new ListBoxHeader(Point(1, 1), Size(GetWidth() - 2, 16));
	header->onClickTab.Connect(&onClickTab);
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
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface		*surface	   = GetDrawSurface();
	Rect		 frame		   = Rect(GetRealPosition(), GetRealSize());
	Rect		 entryRect	   = frame;

	String		 visibleEntries;
	Int		 visibleEntryCount = 0;

	ListEntry	*lastVisibleEntry  = NIL;

	if (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) header->Show();
	else						  header->Hide();

	Int		 entriesHeight	   = 0;
	Int		 headerHeight	   = (header->IsVisible()    ? header->GetHeight()   : 0);
	Int		 scrollbarWidth	   = (scrollbar->IsVisible() ? scrollbar->GetWidth() : 0);

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(frame);

			/* Update scrollbar if necessary.
			 */
			entriesHeight = GetEntriesHeight();

			if (entriesHeight > GetHeight() - headerHeight - 4 && !(flags & LF_HIDESCROLLBAR))
			{
				scrollbar->SetMetrics(Point(18, 1 + headerHeight), Size(scrollbar->GetWidth(), GetHeight() - 2 - headerHeight));
				scrollbar->SetRange(0, entriesHeight - (GetHeight() - 4 - headerHeight));

				scrollbar->SetPageSize(GetHeight() - 4 - headerHeight);

				scrollbar->Show();

				scrollbarWidth = scrollbar->GetWidth();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos   = 0;
				scrollbarWidth = 0;
			}

			/* Set visibility of list entries first.
			 */
			entryRect.top	 = -scrollbarPos;
			entryRect.bottom = -scrollbarPos;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				entry->SetVisibleDirect(False);

				if (entryRect.bottom + entry->GetHeight() >= 0 && entryRect.top <= GetHeight() - headerHeight - 4)
				{
					entry->SetMetrics(Point(2, entryRect.top + 2 + headerHeight), Size(GetWidth() - 4 - scrollbarWidth, entry->GetHeight()));
					entry->SetVisibleDirect(True);

					visibleEntries[visibleEntryCount++] = entry->GetHandle() % 32767 + 1;
				}

				entryRect.top	 += entry->GetHeight();
				entryRect.bottom += entry->GetHeight();
			}

			visibleEntriesString = visibleEntries;

			/* Now paint the listbox and all entries.
			 */
			if (IsActive())	surface->Box(frame + Point(0, headerHeight) - Size(scrollbarWidth, headerHeight), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame + Point(0, headerHeight) - Size(scrollbarWidth, headerHeight), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			/* Update scrollbar if necessary.
			 */
			entriesHeight = GetEntriesHeight();

			if ((entriesHeight >  GetHeight() - headerHeight - 4 && !scrollbar->IsVisible() && !(flags & LF_HIDESCROLLBAR)) ||
			    (entriesHeight <= GetHeight() - headerHeight - 4 &&  scrollbar->IsVisible()				      )) return Paint(SP_PAINT);

			scrollbar->SetRange(0, entriesHeight - (GetHeight() - 4 - headerHeight));

			/* Find visible entries.
			 */
			entryRect.top	 = -scrollbarPos;
			entryRect.bottom = -scrollbarPos;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				entry->SetVisibleDirect(False);

				if (entryRect.bottom + entry->GetHeight() >= 0 && entryRect.top <= GetHeight() - headerHeight - 4)
				{
					entry->SetMetrics(Point(2, entryRect.top + 2 + headerHeight), Size(GetWidth() - 4 - scrollbarWidth, entry->GetHeight()));
					entry->SetVisibleDirect(True);

					lastVisibleEntry = entry;

					visibleEntries[visibleEntryCount++] = entry->GetHandle() % 32767 + 1;
				}

				entryRect.top	 += entry->GetHeight();
				entryRect.bottom += entry->GetHeight();

				if (entryRect.top > GetHeight() - headerHeight - 4) break;
			}

			/* Check for changes.
			 */
			if (visibleEntriesString != visibleEntries)
			{
				/* Draw added entry only if it was added to the end.
				 */
				if	(visibleEntries.StartsWith(visibleEntriesString) && visibleEntries.Length() == visibleEntriesString.Length() + 1) lastVisibleEntry->Paint(SP_PAINT);

				/* Nothing needs to be done if last entry was removed.
				 */
				else if (visibleEntriesString.StartsWith(visibleEntries) && visibleEntriesString.Length() == visibleEntries.Length() + 1) ;

				/* Redraw the whole list if something else changed.
				 */
				else															  Paint(SP_PAINT);

				visibleEntriesString = visibleEntries;
			}

			break;
	}

	return Success();
}

S::Int S::GUI::ListBox::ScrollUp(Int n)
{
	if (n == ScrollToTop) scrollbar->SetValue(0);
	else		      scrollbar->SetValue(scrollbarPos - scrollbar->GetStepSize());

	return Success();
}

S::Int S::GUI::ListBox::ScrollDown(Int n)
{
	if (n == ScrollToBottom) scrollbar->SetValue(2147483647);
	else			 scrollbar->SetValue(scrollbarPos + scrollbar->GetStepSize());

	return Success();
}

S::Int S::GUI::ListBox::DragSelectedEntry(Bool upDown)
{
	if (!(flags & LF_ALLOWREORDER)) return Error();

	Int	 selectedEntryNumber = GetSelectedEntryNumber();
	Int	 newEntryNumber	     = selectedEntryNumber;

	if	( upDown && selectedEntryNumber != 0)		 newEntryNumber = selectedEntryNumber - 1;
	else if (!upDown && selectedEntryNumber != Length() - 1) newEntryNumber = selectedEntryNumber + 1;

	Int	 oldSbPos = scrollbarPos;

	if	(newEntryNumber < scrollbarPos)		    scrollbarPos--;
	else if (!GetNthEntry(newEntryNumber)->IsVisible()) scrollbarPos++;

	if (selectedEntryNumber != newEntryNumber)
	{
		MoveEntry(selectedEntryNumber, newEntryNumber);

		Paint(SP_PAINT);

		if (scrollbarPos != oldSbPos) System::System::Sleep(100);
	}

	return Success();
}

S::GUI::Rect S::GUI::ListBox::GetVisibleArea() const
{
	Int	 headerHeight = (header->IsVisible() ? header->GetRealSize().cy : 0);

	if (!IsVisible()) return Widget::GetVisibleArea();
	else		  return Widget::GetVisibleArea() + Point(0, 2 + headerHeight) - Size(0, 4 + headerHeight);
}

S::Int S::GUI::ListBox::GetEntriesHeight() const
{
	Int	 entriesHeight	= 0;

	for (Int i = 0; i < Length(); i++)
	{
		entriesHeight += GetNthEntry(i)->GetHeight();
	}

	return entriesHeight;
}

S::Void S::GUI::ListBox::OnScrollbarValueChange()
{
	/* Redraw only the list entries.
	 */
	Surface	*surface = GetDrawSurface();

	Rect	 frame		= Rect(GetRealPosition(), GetRealSize());

	Int	 headerHeight	= (header->IsVisible()	  ? header->GetRealSize().cy	: 0);
	Int	 scrollbarWidth	= (scrollbar->IsVisible() ? scrollbar->GetRealSize().cx : 0);

	surface->StartPaint(frame + Point(2, 2 + headerHeight) - Size(4 + scrollbarWidth, 4 + headerHeight));

	Paint(SP_PAINT);

	surface->EndPaint();
}

S::Void S::GUI::ListBox::OnChangeSize(const Size &nSize)
{
	if (scrollbar->IsVisible())
	{
		Int	 headerHeight = (header->IsVisible() ? header->GetHeight() : 0);

		scrollbar->SetHeight(nSize.cy - 2 - headerHeight);
		scrollbar->SetRange(0, GetEntriesHeight() - (GetHeight() - 4 - headerHeight));

		scrollbar->SetPageSize(GetHeight() - 4 - headerHeight);
	}

	header->SetWidth(nSize.cx - 2);
}
