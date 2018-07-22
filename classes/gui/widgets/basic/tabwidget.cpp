 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());
				Int	 offset	 = 19 * surface->GetSurfaceDPI() / 96;

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(frame + Point(0, offset) - Size(0, offset), FRAME_UP);

				frame.right	= frame.left;
				frame.bottom	= frame.top + offset;

				for (Int i = 0; i < GetNOfObjects(); i++)
				{
					Widget		*widget = GetNthObject(i);
					Widget		*prev	= (i > 0) ? GetNthObject(i - 1) : NIL;

					const Bitmap	&bitmap = bitmaps.Get(widget->GetHandle());
					Size		 sSize	= bitmap == NIL ? Size() : Size(bitmap.GetSize().cx / (bitmap.GetSize().cy / 15.0) , 15) * surface->GetSurfaceDPI() / 96;

					frame.left  = frame.right + 1;
					frame.right = frame.left + widget->GetScaledTextWidth() + sSize.cx + 6 + Int(widget->GetText() != NIL ? 6 : 0);

					if (widget->IsVisible())
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

					if (widget->IsVisible()) frame.bottom--;

					if (bitmap != NIL)
					{
						surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), Rect(Point(frame.left + 3 + (widget->IsVisible() ? 1 : 0), frame.top + 2), sSize));
					}

					surface->SetText(widget->GetText(), Rect(Point(frame.left + sSize.cx + 6 + (widget->IsVisible() ? 1 : 0), frame.top + Math::Ceil(Float(frame.GetHeight() - widget->GetFont().GetScaledTextSizeY()) / 2) - 1), Size(widget->GetScaledTextWidth() + sSize.cx, offset - 2)), widget->GetFont());

					if (widget->IsVisible())
					{
						frame.bottom++;

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
					Widget		*widget = GetNthObject(i);

					const Bitmap	&bitmap = bitmaps.Get(widget->GetHandle());
					Size		 sSize	= bitmap == NIL ? Size() : Size(bitmap.GetSize().cx / (bitmap.GetSize().cy / 15.0) , 15) * surface->GetSurfaceDPI() / 96;

					frame.left  = frame.right + 1;
					frame.right = frame.left + widget->GetScaledTextWidth() + sSize.cx + 6 + Int(widget->GetText() != NIL ? 6 : 0);

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

S::Void S::GUI::TabWidget::OnChangeSize(const Size &nSize)
{
	for (Int i = 0; i < GetNOfObjects(); i++) GetNthObject(i)->SetSize(nSize - Size(3, 22));
}

S::Int S::GUI::TabWidget::Add(Widget *widget)
{
	return Add(widget, NIL);
}

S::Int S::GUI::TabWidget::Add(Widget *widget, const Bitmap &nBitmap)
{
	if (widget == NIL) return Error();

	if (!widget->IsRegistered())
	{
		widget->Hide();

		if (Widget::Add(widget) == Success())
		{
			Bitmap	 bitmap = nBitmap;

			bitmap.SetBackgroundColor(GetBackgroundColor());

			bitmaps.Add(bitmap, widget->GetHandle());

			widget->SetMetrics(Point(2, 21), GetSize() - Size(3, 22));

			if (GetNOfObjects() == 1) SelectTab(widget);

			return Success();
		}
	}

	return Error();
}

S::Int S::GUI::TabWidget::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	Bool	 wasVisible = widget->IsVisible();

	if (Widget::Remove(widget) == Success())
	{
		bitmaps.Remove(widget->GetHandle());

		if (wasVisible)
		{
			if (GetNOfObjects() > 0) SelectTab(GetNthObject(0));
			else			 selectedTab = NIL;
		}

		return Success();
	}

	return Error();
}
