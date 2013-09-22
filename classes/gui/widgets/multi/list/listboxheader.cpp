 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listboxheader.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>

const S::Short	 S::GUI::ListBoxHeader::classID = S::Object::RequestClassID();

S::GUI::ListBoxHeader::ListBoxHeader(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	moveTab		= -1;
	draggingTab	= False;

	type		= classID;

	font.SetWeight(Font::Bold);

	dragHotspot	= new Hotspot(Point(0, 0), GetSize());
	dragHotspot->SetIndependent(True);
	dragHotspot->onMouseDragStart.Connect(&ListBoxHeader::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&ListBoxHeader::OnMouseDrag, this);
	dragHotspot->onMouseDragEnd.Connect(&ListBoxHeader::OnMouseDragEnd, this);

	Add(dragHotspot);

	onChangeSize.Connect(&ListBoxHeader::OnChangeSize, this);
}

S::GUI::ListBoxHeader::~ListBoxHeader()
{
	DeleteObject(dragHotspot);
}

S::Int S::GUI::ListBoxHeader::EnableLocking(Bool enable)
{
	if (enable)
	{
		tabNames.EnableLocking();
		tabWidths.EnableLocking();
		tabOrientations.EnableLocking();
		tabChecked.EnableLocking();
	}
	else
	{
		tabNames.DisableLocking();
		tabWidths.DisableLocking();
		tabOrientations.DisableLocking();
		tabChecked.DisableLocking();
	}

	return Widget::EnableLocking(enable);
}

S::Int S::GUI::ListBoxHeader::AddTab(const String &iTabName, Int iTabWidth, Int iTabOrientation)
{
	tabNames.Add(iTabName);
	tabWidths.Add(iTabWidth);
	tabOrientations.Add(iTabOrientation);
	tabChecked.Add(False);

	OnChangeSize(GetSize());

	return Success();
}

S::Int S::GUI::ListBoxHeader::RemoveAllTabs()
{
	tabNames.RemoveAll();
	tabWidths.RemoveAll();
	tabOrientations.RemoveAll();
	tabChecked.RemoveAll();

	OnChangeSize(GetSize());

	return Success();
}

S::Int S::GUI::ListBoxHeader::GetNOfTabs() const
{
	return tabWidths.Length();
}

S::Int S::GUI::ListBoxHeader::GetNthTabOffset(Int n) const
{
	if (n >= tabWidths.Length()) return -1;

	Int	 offset = 0;

	for (Int i = 0; i < n; i++)
	{
		offset += (Int) Math::Abs(tabWidths.GetNth(i));
	}

	return offset;
}

S::Int S::GUI::ListBoxHeader::GetNthTabWidth(Int n) const
{
	return (Int) Math::Abs(tabWidths.GetNth(n));
}

S::Int S::GUI::ListBoxHeader::GetNthTabOrientation(Int n) const
{
	return (Int) Math::Abs(tabOrientations.GetNth(n));
}

S::Int S::GUI::ListBoxHeader::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Point	 realPos = GetRealPosition();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(frame, FRAME_UP);

				for (Int i = 0; i < tabWidths.Length(); i++)
				{
					frame.right = (Int) Math::Min(frame.left + Math::Abs((Int) Math::Round(tabWidths.GetNth(i) * surface->GetSurfaceDPI() / 96.0)), realPos.x + GetRealSize().cx);

					surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
					surface->Frame(frame, FRAME_UP);

					frame.left += 3;
					frame.top += 1;

					Font	 nFont = font;

					if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

					surface->SetText(tabNames.GetNth(i), frame, nFont);

					frame.top -= 1;
					frame.left += (Int) (Math::Abs(Math::Round(tabWidths.GetNth(i) * surface->GetSurfaceDPI() / 96.0)) - 3);
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::ListBoxHeader::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		 return Error();
	if (!IsActive() || !IsVisible()) return Success();

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (draggingTab) break;

			{
				Window	*window	  = GetContainerWindow();
				Surface	*surface  = GetDrawSurface();

				Point	 realPos  = GetRealPosition();
				Size	 realSize = GetRealSize();
				Rect	 frame	  = Rect(realPos, realSize);

				frame.left = realPos.x - 2;

				for (Int i = 0; i < tabWidths.Length() - 1; i++)
				{
					frame.left += (Int) Math::Abs(Math::Round(tabWidths.GetNth(i) * surface->GetSurfaceDPI() / 96.0));
					frame.right = frame.left + 4;

					if (window->IsMouseOn(frame))
					{
						if (moveTab != i)
						{
							moveTab = i;

							Input::Pointer::SetCursor(window, Input::Pointer::CursorHSize);
						}
					}
					else if (moveTab == i)
					{
						moveTab = -1;

						Input::Pointer::SetCursor(window, Input::Pointer::CursorArrow);
					}
				}

				frame.left = realPos.x + 1;
				frame.top++;
				frame.bottom--;

				for (Int j = 0; j < tabWidths.Length(); j++)
				{
					frame.right = (Int) Math::Min(frame.left + Math::Abs((Int) Math::Round(tabWidths.GetNth(j) * surface->GetSurfaceDPI() / 96.0)) - 2, realPos.x + GetRealSize().cx - 1);

					if (window->IsMouseOn(frame) && !tabChecked.GetNth(j) && moveTab == -1)
					{
						surface->Box(frame, Setup::LightGrayColor, Rect::Filled);
						surface->SetText(tabNames.GetNth(j), frame + Point(2, 0) - Size(2, 0), font);

						tabChecked.Set(tabChecked.GetNthIndex(j), True);
					}
					else if ((!window->IsMouseOn(frame) || moveTab != -1) && tabChecked.GetNth(j))
					{
						surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
						surface->SetText(tabNames.GetNth(j), frame + Point(2, 0) - Size(2, 0), font);

						tabChecked.Set(tabChecked.GetNthIndex(j), False);
					}

					frame.left += (Int) Math::Abs(Math::Round(tabWidths.GetNth(j) * surface->GetSurfaceDPI() / 96.0));
				}
			}

			break;
	}

	return Widget::Process(message, wParam, lParam);
}

S::Void S::GUI::ListBoxHeader::OnMouseDragStart(const Point &mousePos)
{
	if (moveTab == -1) return;

	startMousePos = mousePos;

	moveTabStartWidth = tabWidths.GetNth(moveTab);
	lastTabStartWidth = tabWidths.GetNth(tabWidths.Length() - 1);

	draggingTab = True;
}

S::Void S::GUI::ListBoxHeader::OnMouseDrag(const Point &mousePos)
{
	if (!draggingTab) return;

	Int	 bias = Math::Round((startMousePos.x - mousePos.x) * 96.0 / Surface().GetSurfaceDPI());

	if (bias != 0)
	{
		tabWidths.SetNth(moveTab, (Int) Math::Max(Math::Abs(moveTabStartWidth) - bias, 1) * Math::Sign(moveTabStartWidth));
		tabWidths.SetNth(tabWidths.Length() - 1, (Int) Math::Max(Math::Abs(lastTabStartWidth) + bias, 1) * Math::Sign(lastTabStartWidth));

		OnChangeSize(GetSize());

		container->Paint(SP_PAINT);
	}
}

S::Void S::GUI::ListBoxHeader::OnMouseDragEnd(const Point &mousePos)
{
	draggingTab = False;
}

S::Void S::GUI::ListBoxHeader::OnChangeSize(const Size &nSize)
{
	Int	 varSizeTabs	  = 0;
	Int	 sumFixedTabSizes = 0;

	for (Int i = 0; i < tabWidths.Length(); i++)
	{
		if (tabWidths.GetNth(i) <= 0)	varSizeTabs++;
		else				sumFixedTabSizes += tabWidths.GetNth(i);
	}

	if (varSizeTabs > 0)
	{
		for (Int i = 0; i < tabWidths.Length(); i++)
		{
			if (tabWidths.GetNth(i) <= 0) tabWidths.SetNth(i, -Math::Max(0, (GetWidth() - sumFixedTabSizes) / varSizeTabs));
		}
	}

	dragHotspot->SetSize(nSize);
}
