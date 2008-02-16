 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listboxheader.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::ListBoxHeader::classID = S::Object::RequestClassID();

S::GUI::ListBoxHeader::ListBoxHeader(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	moveTab		= -1;
	innerLoop	= False;

	type		= classID;

	font.SetWeight(Font::Bold);

	onChangeSize.Connect(&ListBoxHeader::OnChangeSize, this);
}

S::GUI::ListBoxHeader::~ListBoxHeader()
{
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
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = container->GetDrawSurface();
				Point	 realPos = GetRealPosition();
				Rect	 frame	 = Rect(GetRealPosition(), GetSize());

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(frame, FRAME_UP);

				for (Int i = 0; i < tabWidths.Length(); i++)
				{
					frame.right = (Int) Math::Min(frame.left + Math::Abs(tabWidths.GetNth(i)) + 1, realPos.x + GetWidth());

					surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
					surface->Frame(frame, FRAME_UP);

					frame.left += 3;
					frame.top += 1;

					Font	 nFont = font;

					if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

					surface->SetText(tabNames.GetNth(i), frame, nFont);

					frame.top -= 1;
					frame.left += (Int) (Math::Abs(tabWidths.GetNth(i)) - 2);
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::ListBoxHeader::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsActive() || !IsVisible())	return Success();

	Window	*window	 = container->GetContainerWindow();
	Surface	*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	Point	 realPos = GetRealPosition();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());
	Int	 retVal	 = Success();

	switch (message)
	{
		case SM_MOUSEMOVE:
			if (innerLoop) break;

			{
				frame.left = realPos.x - 2;

				for (Int i = 0; i < tabWidths.Length() - 1; i++)
				{
					frame.left += (Int) (Math::Abs(tabWidths.GetNth(i)) + 1);
					frame.right = frame.left + 4;

					if (window->IsMouseOn(frame))
					{
						if (moveTab != i)
						{
							moveTab = i;

							LiSASetMouseCursor(window->GetSystemWindow(), LiSA_MOUSE_HSIZE);
						}
					}
					else if (moveTab == i)
					{
						moveTab = -1;

						LiSASetMouseCursor(window->GetSystemWindow(), LiSA_MOUSE_ARROW);
					}
				}

				frame.left = realPos.x;
				frame.bottom--;

				for (Int j = 0; j < tabWidths.Length(); j++)
				{
					frame.right = (Int) Math::Min(frame.left + Math::Abs(tabWidths.GetNth(j)), realPos.x + GetWidth() - 1);

					frame.left++;
					frame.top++;

					if (window->IsMouseOn(frame) && !tabChecked.GetNth(j) && moveTab == -1)
					{
						surface->Box(frame, Setup::LightGrayColor, Rect::Filled);

						frame.left += 2;
						surface->SetText(tabNames.GetNth(j), frame, font);
						frame.left -= 2;

						tabChecked.Set(tabChecked.GetNthIndex(j), True);
					}
					else if ((!window->IsMouseOn(frame) || moveTab != -1) && tabChecked.GetNth(j))
					{
						surface->Box(frame, Setup::BackgroundColor, Rect::Filled);

						frame.left += 2;
						surface->SetText(tabNames.GetNth(j), frame, font);
						frame.left -= 2;

						tabChecked.Set(tabChecked.GetNthIndex(j), False);
					}

					frame.left--;
					frame.top--;

					frame.left += (Int) (Math::Abs(tabWidths.GetNth(j)) + 1);
				}
			}

			break;
		case SM_LBUTTONDOWN:
			if (moveTab != -1)
			{
#ifdef __WIN32__
				Int	 leftButton;

				if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
				else					leftButton = VK_LBUTTON;

				Int	 omx = window->GetMousePosition().x;

				innerLoop = True;

				System::EventProcessor	*event = new System::EventProcessor();

				do
				{
					if (peekLoop > 0)	event->ProcessNextEvent(False);
					else			event->ProcessNextEvent(True);

					Int	 mx = window->GetMousePosition().x;
					Int	 bias = omx - mx;

					if (bias != 0)
					{
						tabWidths.Set(moveTab, (Int) Math::Max(Math::Abs(tabWidths.Get(moveTab)) - bias, 1) * Math::Sign(tabWidths.Get(moveTab)));
						tabWidths.Set(tabWidths.Length() - 1, (Int) Math::Max(Math::Abs(tabWidths.Get(tabWidths.Length() - 1)) + bias, 1) * Math::Sign(tabWidths.Get(tabWidths.Length() - 1)));

						omx = mx;

						OnChangeSize(GetSize());

						container->Paint(SP_PAINT);
					}
				}
				while (GetAsyncKeyState(leftButton) != 0);

				delete event;

				innerLoop = False;
#endif
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::ListBoxHeader::OnChangeSize(const Size &nSize)
{
	Int	 varSizeTabs = 0;
	Int	 sumFixedTabSizes = 0;

	for (Int i = 0; i < tabWidths.Length(); i++)
	{
		if (tabWidths.GetNth(i) <= 0)	varSizeTabs++;
		else				sumFixedTabSizes += tabWidths.GetNth(i);
	}

	for (Int j = 0; j < tabWidths.Length(); j++)
	{
		if (tabWidths.GetNth(j) <= 0) tabWidths.Set(tabWidths.GetNthIndex(j), -Math::Max(0, (GetWidth() - sumFixedTabSizes) / varSizeTabs));
	}
}
