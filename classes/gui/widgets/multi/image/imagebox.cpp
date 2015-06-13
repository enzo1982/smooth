 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/image/imagebox.h>
#include <smooth/gui/widgets/basic/scrollbar.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/system/system.h>

const S::Short	 S::GUI::ImageBox::classID = S::Object::RequestClassID();

S::GUI::ImageBox::ImageBox(const Point &iPos, const Size &iSize)
{
	type	     = classID;

	scrollbar    = NIL;
	scrollbarPos = 0;

	SetBackgroundColor(Setup::ClientColor);
	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&ImageBox::OnChangeSize, this);

	scrollbar = new Scrollbar(Point(), Size(), OR_HORZ, &scrollbarPos, 0, 1);
	scrollbar->SetOrientation(OR_LOWERLEFT);
	scrollbar->SetStepSize(15);
	scrollbar->onValueChange.Connect(&ImageBox::OnScrollbarValueChange, this);
	scrollbar->Hide();

	Add(scrollbar);
}

S::GUI::ImageBox::~ImageBox()
{
	DeleteObject(scrollbar);
}

S::Int S::GUI::ImageBox::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface		*surface	   = GetDrawSurface();
	Rect		 frame		   = Rect(GetRealPosition(), GetRealSize());
	Rect		 entryRect	   = frame;

	String		 visibleEntries;
	Int		 visibleEntryCount = 0;

	Int		 entriesWidth	   = 0;
	Int		 scrollbarHeight   = (scrollbar->IsVisible() ? scrollbar->GetHeight() : 0);

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(frame);

			/* Update scrollbar if necessary.
			 */
			entriesWidth = GetEntriesWidth();

			if (entriesWidth > GetWidth() - 4)
			{
				scrollbar->SetMetrics(Point(1, 18), Size(GetWidth() - 2, scrollbar->GetHeight()));
				scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));

				scrollbar->SetPageSize(GetWidth() - 4);

				scrollbar->Show();

				scrollbarHeight = scrollbar->GetHeight();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos	= 0;
				scrollbarHeight = 0;
			}

			/* Set visibility of image entries first.
			 */
			entryRect.left	= -scrollbarPos;
			entryRect.right = -scrollbarPos;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				entry->SetVisibleDirect(False);

				if (entryRect.right + entry->GetWidth() >= 0 && entryRect.left <= GetWidth() - 4)
				{
					entry->SetMetrics(Point(entryRect.left + 2, 2), Size(entry->GetWidth(), GetHeight() - 4 - scrollbarHeight));
					entry->SetVisibleDirect(True);

					visibleEntries[visibleEntryCount++] = entry->GetHandle() % 32767 + 1;
				}

				entryRect.left	+= entry->GetWidth();
				entryRect.right += entry->GetWidth();
			}

			visibleEntriesString = visibleEntries;

			/* Now paint the imagebox and all entries.
			 */
			if (IsActive())	surface->Box(frame - Size(0, scrollbarHeight), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame - Size(0, scrollbarHeight), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			/* Update scrollbar if necessary.
			 */
			entriesWidth = GetEntriesWidth();

			if ((entriesWidth >  GetWidth() - 4 && !scrollbar->IsVisible()) ||
			    (entriesWidth <= GetWidth() - 4 &&  scrollbar->IsVisible())) return Paint(SP_PAINT);

			scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));

			/* Find visible entries.
			 */
			entryRect.left	= -scrollbarPos;
			entryRect.right = -scrollbarPos;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*entry = GetNthEntry(i);

				if (entryRect.right + entry->GetWidth() >= 0 && entryRect.left <= GetWidth() - 4)
				{
					visibleEntries[visibleEntryCount++] = entry->GetHandle() % 32767 + 1;
				}

				entryRect.left	+= entry->GetWidth();
				entryRect.right += entry->GetWidth();
			}

			/* Check for changes.
			 */
			if (visibleEntriesString != visibleEntries) Paint(SP_PAINT);

			break;
	}

	return Success();
}

S::GUI::Rect S::GUI::ImageBox::GetVisibleArea() const
{
	if (!IsVisible()) return Widget::GetVisibleArea();
	else		  return Widget::GetVisibleArea() + Point(2, 0) - Size(4, 0);
}

S::Int S::GUI::ImageBox::GetEntriesWidth() const
{
	Int	 entriesWidth	= 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		entriesWidth += GetNthObject(i)->GetWidth();
	}

	return entriesWidth;
}

S::Void S::GUI::ImageBox::OnScrollbarValueChange()
{
	Paint(SP_PAINT);
}

S::Void S::GUI::ImageBox::OnChangeSize(const Size &nSize)
{
	if (scrollbar->IsVisible())
	{
		scrollbar->SetWidth(nSize.cx - 2);
		scrollbar->SetRange(0, GetEntriesWidth() - (GetWidth() - 4));

		scrollbar->SetPageSize(GetWidth() - 4);
	}
}
