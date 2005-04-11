 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/misc/i18n.h>
#include <smooth/gui/widgets/basic/scrollbar.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/multi/list/listboxheader.h>
#include <smooth/system/timer.h>
#include <smooth/gui/widgets/special/tooltip.h>

const S::Int	 S::GUI::ListBox::classID = S::Object::RequestClassID();

S::GUI::ListBox::ListBox(Point iPos, Size iSize)
{
	type			= classID;

	scrollbar		= NIL;
	scrollbarPos		= 0;
	lastScrollbarPos	= 0;

	header			= NIL;

	possibleContainers.AddEntry(Layer::classID);

	SetFont(Font(font.GetName(), I18N_DEFAULTFONTSIZE, Setup::ClientTextColor));

	SetBackgroundColor(Setup::ClientColor);

	pos			= iPos;
	size			= iSize;

	if (size.cx == 0) size.cx = 120;
	if (size.cy == 0) size.cy = 80;
}

S::GUI::ListBox::~ListBox()
{
	if (scrollbar != NIL)
	{
		if (scrollbar->IsRegistered() && container != NIL) container->UnregisterObject(scrollbar);

		DeleteObject(scrollbar);

		scrollbar = NIL;
	}

	if (header != NIL)
	{
		if (registered && container != NIL && !(flags & LF_HIDEHEADER)) container->UnregisterObject(header);

		DeleteObject(header);

		header = NIL;
	}
}

S::Int S::GUI::ListBox::AddTab(String tabName, Int iTabWidth)
{
	if (header == NIL)
	{
		header = new ListBoxHeader(this);

		if (visible && !(flags & LF_HIDEHEADER)) container->RegisterObject(header);
	}

	return header->AddTab(tabName, iTabWidth);
}

S::Int S::GUI::ListBox::ClearTabs()
{
	if (header != NIL)	return header->ClearTabs();
	else			return Success;
}

S::Int S::GUI::ListBox::GetNOfTabs()
{
	if (header != NIL)	return header->GetNOfTabs();
	else			return 0;
}

S::Int S::GUI::ListBox::GetNthTabOffset(Int n)
{
	if (header != NIL)	return header->GetNthTabOffset(n);
	else			return 0;
}

S::Int S::GUI::ListBox::GetNthTabWidth(Int n)
{
	if (header != NIL)	return header->GetNthTabWidth(n);
	else			return 0;
}

S::Int S::GUI::ListBox::Show()
{
	if (visible)	return Success;

	if (scrollbar != NIL)
	{
		Point	 realPos	= GetRealPosition();
		Point	 sbp		= Point(realPos.x + size.cx - container->pos.x - 18, realPos.y + 1 - container->pos.y + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16));
		Size	 sbs		= Size(scrollbar->size.cx, size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16));

		scrollbar->SetMetrics(sbp, sbs);
		scrollbar->SetRange(0, GetNOfObjects() - (int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

		scrollbar->Show();
	}

	if (header != NIL)
	{
		header->UpdateMetrics();

		if (!(flags & LF_HIDEHEADER)) container->RegisterObject(header);
	}

	return Widget::Show();
}

S::Int S::GUI::ListBox::Hide()
{
	if (!visible)	return Success;

	if (header != NIL && !(flags & LF_HIDEHEADER)) container->UnregisterObject(header);

	if (scrollbar != NIL) scrollbar->Hide();

	return Widget::Hide();
}

S::Int S::GUI::ListBox::Activate()
{
	Int	 rVal = Widget::Activate();

	if (rVal == Success && scrollbar != NIL) scrollbar->Activate();
	if (rVal == Success && header != NIL) header->Activate();

	return rVal;
}

S::Int S::GUI::ListBox::Deactivate()
{
	Int	 rVal = Widget::Deactivate();

	if (rVal == Success && scrollbar != NIL) scrollbar->Deactivate();
	if (rVal == Success && header != NIL) header->Deactivate();

	return rVal;
}

S::Int S::GUI::ListBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame		= Rect(GetRealPosition(), size);
	Point	 sbp;
	Size	 sbs;
	Int	 maxFrameY;

	surface->StartPaint(frame);

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			if (!(15 * GetNOfObjects() + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > size.cy && !(flags & LF_HIDESCROLLBAR)))
			{
				if (scrollbar != NIL)
				{
					scrollbarPos = 0;
					lastScrollbarPos = 0;

					container->UnregisterObject(scrollbar);

					DeleteObject(scrollbar);

					scrollbar = NIL;
				}
			}

			if (message != SP_UPDATE)
			{
				if (header != NIL && !(flags & LF_HIDEHEADER))	frame.top += 17;
				if (scrollbar != NIL)				frame.right -= 17;

				if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
				else		surface->Box(frame, Setup::BackgroundColor, FILLED);

				if (header != NIL && !(flags & LF_HIDEHEADER))	frame.top -= 17;
				if (scrollbar != NIL)				frame.right += 17;

				surface->Frame(frame, FRAME_DOWN);
			}

			maxFrameY = frame.bottom - 2;

			frame.left++;
			frame.top = frame.top + 1 + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);
			frame.right--;
			frame.bottom = frame.top + 15;

			frame.bottom = Math::Min(frame.bottom, maxFrameY);

			if (15 * GetNOfObjects() + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > size.cy && !(flags & LF_HIDESCROLLBAR))
			{
				if (scrollbar == NIL)
				{
					sbp.x = frame.right - container->pos.x - 17;
					sbp.y = frame.top - container->pos.y;
					sbs.cx = 0;
					sbs.cy = size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);

					scrollbar = new Scrollbar(sbp, sbs, OR_VERT, &scrollbarPos, 0, GetNOfObjects() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

					scrollbar->onClick.Connect(&ListBox::ScrollbarProc, this);

					container->RegisterObject(scrollbar);

					scrollbar->Paint(SP_PAINT);
				}
				else
				{
					sbp.x = frame.right - container->pos.x - 17;
					sbp.y = frame.top - container->pos.y;
					sbs.cy = size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);

					scrollbar->pos = sbp;
					scrollbar->size.cy = sbs.cy;

					scrollbar->SetRange(0, GetNOfObjects() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
				}

				frame.right -= 18;
			}

			lastScrollbarPos = scrollbarPos;

			for (Int i = 0; i < GetNOfObjects(); i++)
			{
				ListEntry	*operat = (ListEntry *) GetNthObject(i);

				if (operat == NIL) break;

				if (i >= scrollbarPos && frame.top < maxFrameY)
				{
					operat->pos	= Point(frame.left + 1, frame.top + 1);
					operat->size	= Size(frame.right - frame.left - 2, frame.bottom - frame.top - 1);

					operat->Paint(message);

					frame.top += 15;
					frame.bottom += 15;

					frame.top = (Int) Math::Min(frame.top, maxFrameY);
					frame.bottom = (Int) Math::Min(frame.bottom, maxFrameY);
				}
				else
				{
					operat->pos	= Point(-1, -1);
					operat->size	= Size(0, 0);
				}
			}

			if (header != NIL && !(flags & LF_HIDEHEADER) && message != SP_UPDATE)
			{
		 		header->UpdateMetrics();
				header->Paint(SP_PAINT);
			}

			break;
	}

	surface->EndPaint();

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ListBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Int	 retVal = Success;

	EnterProtectedRegion();

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		ListEntry	*object = (ListEntry *) assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == Break) retVal = Break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Int S::GUI::ListBox::ScrollUp(Int nLines)
{
	scrollbarPos -= nLines;
	scrollbarPos = Math::Max(scrollbarPos, 0);

	if (scrollbar != NIL) scrollbar->Paint(SP_PAINT);

	ScrollbarProc();

	return Success;
}

S::Int S::GUI::ListBox::ScrollDown(Int nLines)
{
	scrollbarPos += nLines;
	scrollbarPos = Math::Min(scrollbarPos, GetNOfObjects() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

	if (scrollbar != NIL) scrollbar->Paint(SP_PAINT);

	ScrollbarProc();

	return Success;
}

S::Void S::GUI::ListBox::ScrollbarProc()
{
	if (scrollbarPos != lastScrollbarPos)
	{
		lastScrollbarPos = scrollbarPos;

		Paint(SP_PAINT);
	}
}
