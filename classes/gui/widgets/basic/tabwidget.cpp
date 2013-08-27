 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short	 S::GUI::TabWidget::classID = S::Object::RequestClassID();

S::GUI::TabWidget::TabWidget(const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;

	if (GetWidth()	== 0) SetWidth(120);
	if (GetHeight()	== 0) SetHeight(100);

	selectedTab = NIL;

	onSelectTab.SetParentObject(this);

	onChangeSize.Connect(&TabWidget::OnChangeSize, this);
}

S::GUI::TabWidget::~TabWidget()
{
}

S::Int S::GUI::TabWidget::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());
				Int	 offset	 = 19 * surface->GetSurfaceDPI() / 96;

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(frame + Point(0, offset) - Size(0, offset), FRAME_UP);

				frame.right	= frame.left;
				frame.bottom	= frame.top + offset;

				for (Int j = 0; j < GetNOfObjects(); j++)
				{
					Widget	*object	= GetNthObject(j);
					Widget	*prev	= (j > 0) ? GetNthObject(j - 1) : NIL;

					frame.left  = frame.right + 1;
					frame.right = frame.left + object->GetScaledTextWidth() + 12;

					if (object->IsVisible())
					{
						frame.left--;
						frame.right++;
					}
					else
					{
						frame.top++;
					}

					if (prev != NIL && prev->IsVisible()) frame.left++;

					surface->Frame(frame + Size(1, 1), FRAME_UP);

					if (IsRightToLeft()) { frame.left++; frame.right++; }

					if (prev != NIL && prev->IsVisible())
					{
						surface->Line(Point(frame.left, frame.top + 1), Point(frame.left, frame.bottom), Setup::BackgroundColor); frame.left--;

						if (IsRightToLeft()) surface->SetPixel(Point(frame.left + 1, frame.top), Setup::ClientColor);
					}
					else
					{
						surface->SetPixel(Point(frame.left, frame.top), Setup::BackgroundColor);
					}

					surface->SetPixel(Point(frame.right, frame.top), Setup::BackgroundColor);

					if (IsRightToLeft()) { frame.left--; frame.right--; }

					surface->SetText(object->GetText(), Rect(Point(frame.left + 6 + (object->IsVisible() ? 1 : 0), frame.top + 2), Size(object->GetScaledTextWidth(), offset - 2)), object->GetFont());

					if (object->IsVisible())
					{
						surface->Line(Point(frame.left + 1, frame.bottom), Point(frame.right + (IsRightToLeft() ? 1 : 0), frame.bottom), Setup::BackgroundColor);
						surface->SetPixel(Point(frame.left + (IsRightToLeft() ? 1 : 0), frame.bottom), (IsRightToLeft() ? Setup::DividerDarkColor : Setup::ClientColor));

						if (IsRightToLeft()) surface->SetPixel(Point(frame.right + 1, frame.bottom), Setup::ClientColor);

						frame.right--;
					}
					else
					{
						if (prev != NIL && prev->IsVisible()) frame.left++;

						surface->Line(Point(frame.left, frame.bottom), Point(frame.right + (IsRightToLeft() ? 2 : 1), frame.bottom), Setup::ClientColor);

						if (prev != NIL && prev->IsVisible()) frame.left--;

						frame.top--;
					}
				}
			}

			break;
	}

	return Widget::Paint(message);
}

S::Int S::GUI::TabWidget::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		 return Error();
	if (!IsActive() || !IsVisible()) return Success();

	switch (message)
	{
		case SM_LBUTTONDOWN:
			{
				Window	*window	 = container->GetContainerWindow();
				Surface	*surface = GetDrawSurface();
				Point	 realPos = GetRealPosition();
				Int	 offset	 = 19 * surface->GetSurfaceDPI() / 96;
				Rect	 frame	 = Rect(Point(0, realPos.y + 1), Size(realPos.x, offset - 1));

				for (Int i = 0; i < GetNOfObjects(); i++)
				{
					Widget	*widget = GetNthObject(i);

					frame.left  = frame.right + 1;
					frame.right = frame.left + widget->GetScaledTextWidth() + 12;

					if (!widget->IsVisible() && window->IsMouseOn(frame)) { SelectTab(widget); break; }
				}
			}

			break;
	}

	return Widget::Process(message, wParam, lParam);
}

S::Int S::GUI::TabWidget::SelectTab(const Widget *tabWidget)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*widget = GetNthObject(i);

		if (widget != tabWidget) continue;

		Surface	*surface = GetDrawSurface();

		surface->StartPaint(GetVisibleArea());

		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			if (j == i) continue;

			GetNthObject(j)->Hide();
		}

		widget->Show();

		Paint(SP_PAINT);

		surface->EndPaint();

		selectedTab = widget;

		onSelectTab.Emit(widget);

		return Success();
	}

	return Error();
}

const S::GUI::Widget *S::GUI::TabWidget::GetSelectedTab()
{
	return selectedTab;
}

S::Void S::GUI::TabWidget::OnChangeSize(const Size &nSize)
{
	for (Int i = 0; i < GetNOfObjects(); i++) GetNthObject(i)->SetSize(nSize - Size(3, 22));
}

S::Int S::GUI::TabWidget::Add(Widget *widget)
{
	if (Widget::Add(widget) == Success())
	{
		widget->SetMetrics(Point(2, 21), GetSize() - Size(3, 22));

		if (GetNOfObjects() == 1) SelectTab(widget);
		else			  widget->Hide();

		return Success();
	}

	return Error();
}

S::Int S::GUI::TabWidget::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	Bool	 wasVisible = widget->IsVisible();

	if (Widget::Remove(widget) == Success())
	{
		if (wasVisible)
		{
			if (GetNOfObjects() > 0) SelectTab(GetNthObject(0));
			else			 selectedTab = NIL;
		}

		return Success();
	}

	return Error();
}
