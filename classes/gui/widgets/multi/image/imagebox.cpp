 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
	type			= classID;

	scrollbar		= NIL;
	scrollbarPos		= 0;

	visibleEntriesChecksum	= 0;

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
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	String	 visibleEntries;

	Int	 entriesWidth	= 0;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->StartPaint(frame);

			entriesWidth = GetEntriesWidth();

			if (entriesWidth > GetWidth() - 4)
			{
				scrollbar->SetMetrics(Point(1, 18), Size(GetWidth() - 2, scrollbar->GetHeight()));
				scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));

				scrollbar->SetPageSize(GetWidth() - 4);

				scrollbar->Show();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos = 0;
			}

			if (IsActive())	surface->Box(frame - Size(0, scrollbar->IsVisible() ? 18 : 0), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame - Size(0, scrollbar->IsVisible() ? 18 : 0), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			frame.left = -scrollbarPos;
			frame.right = frame.left;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				frame.right += operat->GetWidth();

				if (frame.right >= 0 && frame.left <= GetWidth() - 4)
				{
					operat->SetMetrics(Point(frame.left + 2, 2), Size(operat->GetWidth(), GetHeight() - 4 - (scrollbar->IsVisible() ? 17 : 0)));
					operat->Show();

					visibleEntries.Append(operat->GetName());
				}
				else
				{
					operat->Hide();
				}

				frame.left += operat->GetWidth();
			}

			visibleEntriesChecksum = visibleEntries.ComputeCRC32();

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			entriesWidth = GetEntriesWidth();

			if (entriesWidth > GetWidth() - 4)
			{
				if (!scrollbar->IsVisible())	Paint(SP_PAINT);
				else 				scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));
			}

			frame.left = -scrollbarPos;
			frame.right = frame.left;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				frame.right += operat->GetWidth();

				if (frame.right >= 0 && frame.left <= GetWidth() - 4)
				{
					visibleEntries.Append(operat->GetName());
				}

				frame.left += operat->GetWidth();
			}

			if (visibleEntriesChecksum != visibleEntries.ComputeCRC32()) Paint(SP_PAINT);

			break;
	}

	return Success();
}

S::GUI::Rect S::GUI::ImageBox::GetVisibleArea() const
{
	if (!IsVisible()) return Widget::GetVisibleArea();
	else		  return Widget::GetVisibleArea() + Point(0, 2) - Size(0, 4);
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
