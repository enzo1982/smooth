 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int	 S::GUI::ListBox::classID = S::Object::RequestClassID();

S::GUI::ListBox::ListBox(const Point &iPos, const Size &iSize)
{
	type			= classID;

	scrollbar		= NIL;
	scrollbarPos		= 0;

	visibleEntriesChecksum	= 0;

	SetFont(Font(font.GetName(), Font::DefaultSize, Font::Normal, Font::Normal, Setup::ClientTextColor));

	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&ListBox::OnChangeSize, this);

	scrollbar = new Scrollbar(Point(), Size(), OR_VERT, &scrollbarPos, 0, 1);
	scrollbar->SetOrientation(OR_UPPERRIGHT);
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

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	String	 visibleEntries;

	if (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) header->Show();
	else						  header->Hide();

	Int	 entriesHeight	= (header->IsVisible() ? 16 : 0) + 4;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		entriesHeight += GetNthObject(i)->GetHeight();

		if (entriesHeight > GetHeight()) break;
	}

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(frame);

			if (entriesHeight > GetHeight() && !(flags & LF_HIDESCROLLBAR))
			{
				scrollbar->SetMetrics(Point(18, 1 + (header->IsVisible() ? 16 : 0)), Size(scrollbar->GetWidth(), GetHeight() - 2 - (header->IsVisible() ? 16 : 0)));
				scrollbar->SetRange(0, GetNOfEntries() - (int) ((GetHeight() - 4 - (header->IsVisible() ? 16 : 0)) / GetNthEntry(0)->GetHeight()));

				scrollbar->SetPageSize((Int) ((GetHeight() - 4 - (header->IsVisible() ? 16 : 0)) / GetNthEntry(0)->GetHeight()) - 1);

				scrollbar->Show();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos = 0;
			}

			if (IsActive())	surface->Box(frame + Point(0, header->IsVisible() ? 17 : 0) - Size(scrollbar->IsVisible() ? 18 : 0, header->IsVisible() ? 17 : 0), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame + Point(0, header->IsVisible() ? 17 : 0) - Size(scrollbar->IsVisible() ? 18 : 0, header->IsVisible() ? 17 : 0), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			{
				frame.top = 1 + (header->IsVisible() ? 16 : 0);

				for (Int i = 0, n = 0; i < GetNOfEntries(); i++)
				{
					ListEntry	*operat = GetNthEntry(i);

					if (n++ >= scrollbarPos && frame.top + operat->GetHeight() <= GetHeight() - 3)
					{
						operat->SetMetrics(Point(2, frame.top + 1), Size(GetWidth() - 4 - (scrollbar->IsVisible() ? 17 : 0), operat->GetHeight()));
						operat->Show();

						visibleEntries.Append(operat->GetName());

						frame.top = Math::Min((Int) (frame.top + operat->GetHeight()), GetHeight() - 3);
					}
					else
					{
						operat->Hide();
					}
				}
			}

			visibleEntriesChecksum = visibleEntries.ComputeCRC32();

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			if (entriesHeight > GetHeight() && !(flags & LF_HIDESCROLLBAR))
			{
				if (!scrollbar->IsVisible())	Paint(SP_PAINT);
				else 				scrollbar->SetRange(0, GetNOfEntries() - (int) ((GetHeight() - 4 - (header->IsVisible() ? 16 : 0)) / GetNthEntry(0)->GetHeight()));
			}

			{
				frame.top = 1 + (header->IsVisible() ? 16 : 0);

				for (Int i = 0, n = 0; i < GetNOfEntries(); i++)
				{
					ListEntry	*operat = GetNthEntry(i);

					if (n++ >= scrollbarPos && frame.top + operat->GetHeight() <= GetHeight() - 3)
					{
						visibleEntries.Append(operat->GetName());

						frame.top = Math::Min((Int) (frame.top + operat->GetHeight()), GetHeight() - 3);
					}
				}
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

	if (upDown && selectedEntryNumber != 0)				exchangedEntryNumber = selectedEntryNumber - 1;
	else if (!upDown && selectedEntryNumber != GetNOfEntries() - 1)	exchangedEntryNumber = selectedEntryNumber + 1;

	Int	 oldSbPos = scrollbarPos;

	if (exchangedEntryNumber < scrollbarPos)			scrollbarPos--;
	else if (!GetNthEntry(exchangedEntryNumber)->IsVisible())	scrollbarPos++;

	if (selectedEntryNumber != exchangedEntryNumber)
	{
		SwitchEntries(selectedEntryNumber, exchangedEntryNumber);

		Paint(SP_PAINT);

		if (scrollbarPos != oldSbPos) Sleep(100);
	}

	return Success();
}

S::Void S::GUI::ListBox::OnScrollbarValueChange()
{
	Paint(SP_PAINT);
}

S::Void S::GUI::ListBox::OnChangeSize(const Size &nSize)
{
	if (scrollbar->IsVisible())
	{
		scrollbar->SetHeight(nSize.cy - 2 - (header->IsVisible() ? 16 : 0));
		scrollbar->SetRange(0, GetNOfEntries() - (int) ((nSize.cy - 4 - (header->IsVisible() ? 16 : 0)) / GetNthEntry(0)->GetHeight()));

		scrollbar->SetPageSize((Int) ((GetHeight() - 4 - (header->IsVisible() ? 16 : 0)) / GetNthEntry(0)->GetHeight()) - 1);
	}

	header->SetWidth(nSize.cx - 2);
}
