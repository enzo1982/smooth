 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/tree/tree.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Short	 S::GUI::Tree::classID = S::Object::RequestClassID();

S::GUI::Tree::Tree(const String &iText) : ListEntry(iText)
{
	type	= classID;

	list.SetFlags(LF_ALLOWRESELECT);

	list.onSelectEntry.Connect(&onSelectEntry);

	Add(&list);

	headHotspot	= new Hotspot(Point(), Size(GetWidth(), 16));

	headHotspot->onMouseOver.Connect(&Tree::OnMouseOver, this);
	headHotspot->onMouseOut.Connect(&Tree::OnMouseOut, this);

	headHotspot->onLeftButtonDown.Connect(&onSelect);

	Add(headHotspot);

	hotspot->onLeftButtonDown.Disconnect(&ListEntry::OnSelectEntry, this);

	internalOnSelectEntry.Connect(&Tree::OnSelectEntry, this);

	onChangeSize.Connect(&Tree::OnChangeSize, this);
	onToggleMark.Connect(&Tree::OnToggleMark, this);
}

S::GUI::Tree::~Tree()
{
	internalOnSelectEntry.Disconnect(&Tree::OnSelectEntry, this);

	DeleteObject(headHotspot);

	if (IsRegistered() && container != NIL) container->Remove(this);
}

S::Int S::GUI::Tree::Add(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->GetObjectType() == ListEntry::classID) return list.Add(widget);

	return Widget::Add(widget);
}

S::Int S::GUI::Tree::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->GetObjectType() == ListEntry::classID) return list.Remove(widget);

	return Widget::Remove(widget);
}

S::Void S::GUI::Tree::CalculateHeight()
{
	/* Calculate tree height.
	 */
	Int	 height	= 16;

	if (IsMarked())
	{
		for (Int i = 0; i < Length(); i++) height += GetNthEntry(i)->GetHeight();
	}

	if (height != GetHeight())
	{
		SetHeight(height);

		if (IsRegistered())
		{
			/* Recalculate all parent trees.
		 	*/
			Widget	*widget = container;

			while (widget->GetObjectType() != ListBox::classID)
			{
				if (widget->GetObjectType() == Tree::classID)
				{
					((Tree *) widget)->CalculateHeight();

					break;
				}

				widget = widget->GetContainer();
			}
		}
	}
}

S::Int S::GUI::Tree::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	CalculateHeight();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());
				Window	*window	 = container->GetContainerWindow();

				if (window == NIL) break;

				surface->StartPaint(GetVisibleArea());

				if (!IsMarked())
				{
					list.Hide();
				}

				if (window->IsMouseOn(Rect(Point(frame.left, frame.top), Size(frame.GetWidth(),  Math::Round(16 * surface->GetSurfaceDPI() / 96.0))))) PaintText(Setup::GradientTextColor, True);
				else																       PaintText(active ? font.GetColor() : Setup::InactiveTextColor, False);

				if (IsMarked())
				{
					list.SetVisibleDirect(False);
					list.SetMetrics(Point(12, 16), GetSize() - Size(12, 15));

					Rect	 visibleArea	   = list.GetVisibleArea() - list.GetRealPosition();

					Point	 entryPosition	   = Point(0, 0);
					Point	 entryRealPosition = Point(0, 0);

					for (Int i = 0; i < Length(); i++)
					{
						ListEntry	*entry	       = GetNthEntry(i);
						Size		 entryRealSize = entry->GetRealSize();

						entry->SetVisibleDirect(False);

						if (entryRealPosition.y + entryRealSize.cy >= visibleArea.top && entryRealPosition.y <= visibleArea.bottom)
						{
							entry->SetMetrics(entryPosition, Size(list.GetWidth(), entry->GetHeight()));
							entry->SetVisibleDirect(True);
						}

						entryPosition.y	    += entry->GetHeight();
						entryRealPosition.y  = Math::Round(entryPosition.y * surface->GetSurfaceDPI() / 96.0);
					}

					list.Show();

					Point	 realPos = GetRealPosition();

					for (Int i = Math::Round(16 * surface->GetSurfaceDPI() / 96.0); i < frame.GetHeight() - Math::Round(7 * surface->GetSurfaceDPI() / 96.0); i += 2)
					{
						surface->SetPixel(realPos + Point(Math::Round(6 * surface->GetSurfaceDPI() / 96.0) + (IsRightToLeft() ? 1 : 0), i), Setup::InactiveTextColor);
					}
				}

				surface->EndPaint();
			}

			break;
		case SP_MOUSEIN:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());
				Window	*window	 = container->GetContainerWindow();

				if (window->IsMouseOn(Rect(Point(frame.left, frame.top), Size(frame.GetWidth(), Math::Round(16 * surface->GetSurfaceDPI() / 96.0))))) PaintText(Setup::GradientTextColor, True);
			}

			break;
		case SP_MOUSEOUT:
			PaintText(font.GetColor(), False);

			break;
	}

	return Success();
}

S::Void S::GUI::Tree::PaintText(const Color &color, Bool drawGradient)
{
	Surface	*surface	= GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetRealSize());
	Font	 nFont		= font;
	Bool	 gotTabs	= False;

	if (!active)	nFont.SetColor(Setup::InactiveTextColor);
	else		nFont.SetColor(color);

	for (Int r = 0; r < text.Length(); r++) if (text[r] == '\t') { gotTabs = True; break; }

	surface->StartPaint(GetVisibleArea());

	if (drawGradient)	surface->Gradient(Rect(Point(frame.left, frame.top), Size(frame.GetWidth(), Math::Round(16 * surface->GetSurfaceDPI() / 96.0))), Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
	else			surface->Box(Rect(Point(frame.left, frame.top), Size(frame.GetWidth(), Math::Round(16 * surface->GetSurfaceDPI() / 96.0))), Setup::ClientColor, Rect::Filled);

	Rect	 cbRect = Rect(GetRealPosition() + Point(2, 3) * surface->GetSurfaceDPI() / 96.0, Size(9, 9) * surface->GetSurfaceDPI() / 96.0);

	if (cbRect.GetWidth() % 2 == 0) cbRect = cbRect - Size(1, 1);

	if (cbRect.top <= cbRect.bottom - 1)
	{
		surface->Box(cbRect, Setup::ClientColor, Rect::Filled);
		surface->Box(cbRect, Setup::InactiveTextColor, Rect::Outlined);

		if (cbRect.top <= cbRect.bottom - 3)
		{
			Point	 p1 = Point(cbRect.left + 2 + (IsRightToLeft() ? 1 : 0), cbRect.top + Math::Round(4 * surface->GetSurfaceDPI() / 96.0));
			Point	 p2 = Point(cbRect.right - 2 + (IsRightToLeft() ? 1 : 0), cbRect.top + Math::Round(4 * surface->GetSurfaceDPI() / 96.0));

			Color	 darkColor = Setup::ClientTextColor;

			if (!active) darkColor = Setup::InactiveTextColor;

			surface->Line(p1, p2, darkColor);

			if (!IsMarked())
			{
				p1 = Point(cbRect.left + Math::Round(4 * surface->GetSurfaceDPI() / 96.0) + (IsRightToLeft() ? 1 : 0), cbRect.top + 2);
				p2 = Point(cbRect.left + Math::Round(4 * surface->GetSurfaceDPI() / 96.0) + (IsRightToLeft() ? 1 : 0), cbRect.bottom - 2);

				surface->Line(p1, p2, darkColor);
			}
		}
	}

	if (container->GetObjectType() == ListBox::classID && ((ListBox *) container)->GetNOfTabs() > 0 && gotTabs)
	{
		for (Int i = 0; i < ((ListBox *) container)->GetNOfTabs(); i++)
		{
			Rect	 rect = Rect(GetRealPosition() + Point(1, 1) * surface->GetSurfaceDPI() / 96.0, GetSize() - Size(1, 1) * surface->GetSurfaceDPI() / 96.0 * 2 - Size(1, 0));

			rect.left += ((ListBox *) container)->GetNthTabOffset(i);
			rect.left += (i == 0 ? Math::Round(12 * surface->GetSurfaceDPI() / 96.0) : 0);

			if (((ListBox *) container)->GetNOfTabs() >= i + 2) rect.right = rect.left + (((ListBox *) container)->GetNthTabOffset(i + 1) - ((ListBox *) container)->GetNthTabOffset(i)) - (i == 0 ? Math::Round(12 * surface->GetSurfaceDPI() / 96.0) : 0) - Math::Round(1 * surface->GetSurfaceDPI() / 96.0) * 2 - 1;

			String	 tabText = GetNthTabText(i);

			if (((ListBox *) container)->GetNthTabOrientation(i) == OR_RIGHT)
			{
				rect.left = Math::Max(rect.left, rect.right - nFont.GetScaledTextSizeX(tabText));
			}

			surface->SetText(tabText, rect, nFont);
		}
	}
	else
	{
		surface->SetText(text, frame + Point(1, 1) * surface->GetSurfaceDPI() / 96.0 + Point(Math::Round(12 * surface->GetSurfaceDPI() / 96.0), 0) - Size(1, 1) * surface->GetSurfaceDPI() / 96.0 * 2 - Size(Math::Round(12 * surface->GetSurfaceDPI() / 96.0), 0), nFont);
	}

	surface->EndPaint();
}

S::Void S::GUI::Tree::OnSelectEntry(Int containerHandle, Int handle)
{
	if (!IsRegistered()) return;

	if	(containerHandle == list.GetHandle()	   && handle != GetHandle()) internalOnSelectEntry.Emit(container->GetHandle(), GetHandle());
	else if (containerHandle == container->GetHandle() && handle != GetHandle()) internalOnSelectEntry.Emit(list.GetHandle(),	GetHandle());
}

S::Void S::GUI::Tree::OnChangeSize(const Size &newSize)
{
	headHotspot->SetWidth(newSize.cx);
}

S::Void S::GUI::Tree::OnToggleMark(Bool marked)
{
	if (marked) onOpen.Emit();
	else	    onClose.Emit();

	CalculateHeight();

	if (IsRegistered())
	{
		/* Find ListBox container and repaint it.
		 */
		Widget	*widget = container;

		while (widget != NIL && widget->GetObjectType() != ListBox::classID) widget = widget->GetContainer();

		if (widget != NIL) widget->Paint(SP_PAINT);
	}
}

S::Void S::GUI::Tree::OnMouseOver()
{
	Paint(SP_MOUSEIN);
}

S::Void S::GUI::Tree::OnMouseOut()
{
	Paint(SP_MOUSEOUT);
}

S::Bool S::GUI::Tree::IsTypeCompatible(Short compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == ListEntry::classID) return True;
	else												  return False;
}
