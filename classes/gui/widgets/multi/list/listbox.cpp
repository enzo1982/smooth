 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

	SetFont(Font(font.GetName(), I18N_DEFAULTFONTSIZE, Setup::ClientTextColor));

	SetBackgroundColor(Setup::ClientColor);

	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&ListBox::OnChangeSize, this);

	header = new ListBoxHeader(Point(1, 1), Size(GetWidth() - 2, 16));

	RegisterObject(header);
}

S::GUI::ListBox::~ListBox()
{
	if (scrollbar != NIL) DeleteObject(scrollbar);

	DeleteObject(header);
}

S::Int S::GUI::ListBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	String	 visibleEntries;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(frame);

			if (flags & LF_HIDEHEADER) header->Hide();

			if (!(15 * GetNOfEntries() + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > GetHeight() && !(flags & LF_HIDESCROLLBAR)))
			{
				if (scrollbar != NIL)
				{
					scrollbarPos = 0;

					DeleteObject(scrollbar);

					scrollbar = NIL;
				}
			}

			if (active)	surface->Box(frame + Point(0, (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) ? 17 : 0) - Size(scrollbar != NIL ? 18 : 0, (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) ? 17 : 0), Setup::ClientColor, FILLED);
			else		surface->Box(frame + Point(0, (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) ? 17 : 0) - Size(scrollbar != NIL ? 18 : 0, (GetNOfTabs() > 0 && !(flags & LF_HIDEHEADER)) ? 17 : 0), Setup::BackgroundColor, FILLED);

			surface->Frame(frame, FRAME_DOWN);

			if (15 * GetNOfEntries() + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > GetHeight() && !(flags & LF_HIDESCROLLBAR))
			{
				if (scrollbar == NIL)
				{
					scrollbar = new Scrollbar(Point(18, 1 + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)), Size(0, GetHeight() - 2 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)), OR_VERT, &scrollbarPos, 0, GetNOfEntries() - (Int) ((GetHeight() - 4 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
					scrollbar->SetOrientation(OR_UPPERRIGHT);
					scrollbar->onValueChange.Connect(&ListBox::OnScrollbarValueChange, this);

					RegisterObject(scrollbar);
				}
				else
				{
					scrollbar->SetRange(0, GetNOfEntries() - (int) ((GetHeight() - 4 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
				}
			}

			{
				frame.top = 1 + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16);

				for (Int i = 0, n = 0; i < GetNOfObjects(); i++)
				{
					if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

					ListEntry	*operat = (ListEntry *) GetNthObject(i);

					if (n++ >= scrollbarPos && frame.top + 15 <= GetHeight() - 3)
					{
						operat->SetMetrics(Point(2, frame.top + 1), Size(GetWidth() - 4 - (scrollbar != NIL ? 17 : 0), operat->GetHeight()));
						operat->Show();

						visibleEntries.Append(operat->GetName());

						frame.top = Math::Min((Int) (frame.top + 15), GetHeight() - 3);
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
			if (15 * GetNOfEntries() + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > GetHeight() && !(flags & LF_HIDESCROLLBAR))
			{
				if (scrollbar == NIL)
				{
					Paint(SP_PAINT);
				}
				else
				{
					scrollbar->SetRange(0, GetNOfEntries() - (int) ((GetHeight() - 4 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
				}
			}

			{
				frame.top = 1 + (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16);

				for (Int i = 0, n = 0; i < GetNOfObjects(); i++)
				{
					if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

					ListEntry	*operat = (ListEntry *) GetNthObject(i);

					if (n++ >= scrollbarPos && frame.top + 15 <= GetHeight() - 3)
					{
						visibleEntries.Append(operat->GetName());

						frame.top = Math::Min((Int) (frame.top + 15), GetHeight() - 3);
					}
				}

				if (visibleEntriesChecksum != visibleEntries.ComputeCRC32()) Paint(SP_PAINT);
			}

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::ListBox::OnScrollbarValueChange()
{
	Paint(SP_PAINT);
}

S::Void S::GUI::ListBox::OnChangeSize(const Size &nSize)
{
	if (scrollbar != NIL)
	{
		scrollbar->SetHeight(nSize.cy - 2 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16));
		scrollbar->SetRange(0, GetNOfEntries() - (int) ((nSize.cy - 4 - (GetNOfTabs() == 0 || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
	}

	header->SetWidth(nSize.cx - 2);
	header->UpdateMetrics();
}
