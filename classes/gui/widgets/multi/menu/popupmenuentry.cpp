 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenuentry.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/misc/binary.h>
#include <smooth/system/timer.h>
#include <smooth/system/screen.h>

const S::Short	 S::GUI::PopupMenuEntry::classID = S::Object::RequestClassID();

S::GUI::PopupMenuEntry::PopupMenuEntry(const String &iText, const Bitmap &iBitmap, PopupMenu *iPopup) : MenuEntry(iText, iBitmap, iPopup)
{
	type		= classID;
	orientation	= OR_UPPERLEFT;
	hotspot		= NIL;
	owner		= NIL;
	timer		= NIL;

	shortcutOffset	= 0;

	SetSize(GetMinimumSize());

	if (text != NIL)
	{
		hotspot	= new Hotspot(Point(), GetSize());

		hotspot->onMouseOver.Connect(&PopupMenuEntry::OnMouseOver, this);
		hotspot->onMouseOut.Connect(&PopupMenuEntry::OnMouseOut, this);
		hotspot->onLeftButtonDown.Connect(&PopupMenuEntry::OpenPopupMenu, this);
		hotspot->onLeftButtonClick.Connect(&PopupMenuEntry::OnClickEntry, this);

		Add(hotspot);

		onChangeSize.Connect(&PopupMenuEntry::OnChangeSize, this);
	}
}

S::GUI::PopupMenuEntry::~PopupMenuEntry()
{
	if (hotspot != NIL) DeleteObject(hotspot);

	if (timer != NIL)
	{
		DeleteObject(timer);

		timer = NIL;
	}
}

S::Int S::GUI::PopupMenuEntry::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());
	Rect	 bmpRect = Rect(GetRealPosition() + Point(2, 2) * surface->GetSurfaceDPI() / 96.0 - Point(0, 1), bitmap.GetSize() * surface->GetSurfaceDPI() / 96.0);

	surface->StartPaint(frame);

	switch (message)
	{
		case SP_PAINT:
		case SP_MOUSEOUT:
			if (text == NIL && bitmap == NIL)
			{
				Point	 p1 = Point(frame.left - 1, frame.top + 1);
				Point	 p2 = Point(frame.right + 1, frame.top + 1);

				surface->Bar(p1, p2, OR_HORZ);
			}
			else if (text != NIL || bitmap != NIL)
			{
				Rect	 textRect = frame + Point(18 * surface->GetSurfaceDPI() / 96.0, 0) - Size(4 + 18 * surface->GetSurfaceDPI() / 96.0, 2);
				Font	 nFont	  = font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->SetText(text, textRect, nFont);

				if (bitmap != NIL && bVar == NIL && iVar == NIL)
				{
					surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				}

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - Math::Round(shortcutOffset * surface->GetSurfaceDPI() / 96.0);

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (popup != NIL)
				{
					Int	 size = Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

					Point	 lineStart = Point(frame.right - 4 - size, frame.top + 1 + Math::Round(4 * surface->GetSurfaceDPI() / 96.0));
					Point	 lineEnd   = Point(frame.right - 4 - size, frame.top + 1 + Math::Round(4 * surface->GetSurfaceDPI() / 96.0) + size * 2 - 1);

					for (Int i = 0; i < size; i++) surface->Line(lineStart + Point(i, i), lineEnd + Point(i, -i), Setup::TextColor);
				}
			}

			break;
		case SP_MOUSEIN:
			if (text != NIL || bitmap != NIL)
			{
				Rect	 textRect = frame + Point(18 * surface->GetSurfaceDPI() / 96.0, 0) - Size(4 + 18 * surface->GetSurfaceDPI() / 96.0, 2);
				Font	 nFont	  = font;

				nFont.SetColor(Setup::GradientTextColor);

				surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				surface->SetText(text, textRect, nFont);

				if (bitmap != NIL && bVar == NIL && iVar == NIL)
				{
					surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);

					surface->Frame(bmpRect, FRAME_DOWN);
				}

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - Math::Round(shortcutOffset * surface->GetSurfaceDPI() / 96.0);

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (popup != NIL)
				{
					Int	 size = Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

					Point	 lineStart = Point(frame.right - 4 - size, frame.top + 1 + Math::Round(4 * surface->GetSurfaceDPI() / 96.0));
					Point	 lineEnd   = Point(frame.right - 4 - size, frame.top + 1 + Math::Round(4 * surface->GetSurfaceDPI() / 96.0) + size * 2 - 1);

					for (Int i = 0; i < size; i++) surface->Line(lineStart + Point(i, i), lineEnd + Point(i, -i), Setup::GradientTextColor);
				}
			}

			break;
	}

	surface->EndPaint();

	return Success();
}

S::Int S::GUI::PopupMenuEntry::Show()
{
	visible = True;

	if (hotspot != NIL) hotspot->Show();

	return Success();
}

S::Int S::GUI::PopupMenuEntry::Hide()
{
	if (popup != NIL)
	{
		if (popup->GetContainer() == this) ClosePopupMenu();
	}

	visible = False;

	if (hotspot != NIL) hotspot->Hide();

	return Success();
}

S::GUI::Size S::GUI::PopupMenuEntry::GetMinimumSize() const
{
	if (text != NIL && shortcut == NIL) return Size(unscaledTextSize.cx + 44, 15);
	if (text != NIL && shortcut != NIL) return Size(unscaledTextSize.cx + font.GetUnscaledTextSizeX(shortcut->ToString()) + 59, 15);

	return Size(15, 4);
}

S::Int S::GUI::PopupMenuEntry::GetShortcutTextSize() const
{
	if (shortcut == NIL) return 0;

	return font.GetUnscaledTextSizeX(shortcut->ToString());
}

S::Void S::GUI::PopupMenuEntry::SetShortcutOffset(Int nShortcutOffset)
{
	shortcutOffset = nShortcutOffset;
}

S::Void S::GUI::PopupMenuEntry::OnMouseOver()
{
	if (popup == NIL || IsMouseOver()) return;

	if (Binary::IsFlagSet(owner->GetFlags(), MB_POPUPOPEN))
	{
		container->GetContainerWindow()->Raise();

		OpenPopupMenu();
	}
	else
	{
		if (timer != NIL) DeleteObject(timer);

		timer = new System::Timer();

		timer->onInterval.Connect(&PopupMenuEntry::OpenPopupMenu, this);
		timer->Start(500);
	}
}

S::Void S::GUI::PopupMenuEntry::OnMouseOut()
{
	if (timer != NIL)
	{
		DeleteObject(timer);

		timer = NIL;
	}
}

S::Void S::GUI::PopupMenuEntry::OnClickEntry()
{
	EnterProtectedRegion();

	owner->SetClosedByClick(True);
	owner->internalRequestClose.Emit();

	onAction.Emit();

	LeaveProtectedRegion();
}

S::Void S::GUI::PopupMenuEntry::OnChangeSize(const Size &nSize)
{
	hotspot->SetSize(nSize);
}

S::Void S::GUI::PopupMenuEntry::OpenPopupMenu()
{
	if (popup == NIL) return;

	Widget	*window		= container->GetContainerWindow();
	Surface	*surface	= GetDrawSurface();

	if (window == NIL) return;

	if (timer != NIL)
	{
		DeleteObject(timer);

		timer = NIL;
	}

	hotspot->Deactivate();

	Rect	 monitor	= System::Screen::GetActiveScreenMetrics();
	Point	 realPos	= GetRealPosition();
	Size	 realSize	= GetRealSize();
	Point	 popupPos	= realPos + Point(realSize.cx, Math::Round(-3 * surface->GetSurfaceDPI() / 96.0));

	popup->CalculateSize();

	if (!IsRightToLeft()) { if (window->GetX() + popupPos.x			       + popup->GetWidth() >= monitor.GetWidth()) popupPos.x = realPos.x - Math::Round(popup->GetWidth() * surface->GetSurfaceDPI() / 96.0); }
	else		      { if (window->GetX() + (window->GetWidth() - popupPos.x) - popup->GetWidth() <= 0)		  popupPos.x = realPos.x - Math::Round(popup->GetWidth() * surface->GetSurfaceDPI() / 96.0); }

	if (window->GetY() + popupPos.y + popup->GetHeight() >= monitor.GetHeight()) popupPos.y = realPos.y - popup->GetHeight() + GetHeight() + 3;

	popup->SetPosition(popupPos);
	popup->internalRequestClose.Connect(&PopupMenuEntry::ClosePopupMenu, this);

	owner->SetFlags(owner->GetFlags() | MB_POPUPOPEN);

	Add(popup);
}

S::Void S::GUI::PopupMenuEntry::ClosePopupMenu()
{
	if (popup == NIL) return;

	if (popup->GetContainer() == this)
	{
		owner->SetFlags(owner->GetFlags() & ~MB_POPUPOPEN);

		popup->internalRequestClose.Disconnect(&PopupMenuEntry::ClosePopupMenu, this);

		Remove(popup);

		hotspot->Activate();

		if (popup->IsClosedByClick())
		{
			owner->SetClosedByClick(True);
			owner->internalRequestClose.Emit();
		}
		else
		{
			if (!container->GetContainerWindow()->IsMouseOn(Rect(Point(), container->GetRealSize()))) owner->internalRequestClose.Emit();
			else											  container->GetContainerWindow()->Raise();
		}
	}
}

S::Bool S::GUI::PopupMenuEntry::IsTypeCompatible(Short compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == MenuEntry::classID) return True;
	else												  return False;
}
