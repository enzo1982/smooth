 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/binary.h>
#include <smooth/system/timer.h>

const S::Int	 S::GUI::PopupMenuEntry::classID = S::Object::RequestClassID();

S::GUI::PopupMenuEntry::PopupMenuEntry(const String &iText, const Bitmap &iBitmap, PopupMenu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode) : MenuEntry(iText, iBitmap, iPopup, ibVar, iiVar, iiCode)
{
	type		= classID;
	orientation	= OR_UPPERLEFT;
	hotspot		= NIL;
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

		RegisterObject(hotspot);

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
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame	 = Rect(realPos, GetSize());
	Rect	 bmpRect = Rect(realPos + Point(2, 2), bitmap.GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_MOUSEOUT:
			if (text == NIL && bitmap == NIL)
			{
				Point	 p1 = Point(frame.left - 1, frame.top + 1);
				Point	 p2 = Point(frame.right + 1, frame.top + 1);

				surface->Bar(p1, p2, OR_HORZ);
			}
			else if (text != NIL && bitmap == NIL)
			{
				Rect	 textRect = Rect(realPos + Point(18, 0), GetSize() - Size(22, 2));
				Font	 nFont = font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->Box(frame, Setup::BackgroundColor, FILLED);
				surface->SetText(text, textRect, nFont);

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - shortcutOffset;

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (popup != NIL)
				{
					Point	 p1 = Point(frame.right - 8, frame.top + 4);
					Point	 p2 = Point(frame.right - 8, frame.top + 11);

					for (Int i = 0; i < 4; i++) surface->Line(p1 + Point(i, i), p2 + Point(i, -i), Setup::TextColor);
				}

				if (bVar != NIL)
				{
					if (*bVar == True)
					{
						Point p1 = Point(frame.left + 4, frame.top + 4);
						Point p2 = Point(frame.left + 11, frame.bottom - 4);

						for (Int i = 0; i < 2; i++)
						{
							Int	 color = Setup::DividerDarkColor;

							if (i == 1)
							{
								if (IsActive())	color = Setup::TextColor;
								else		color = Setup::GrayTextColor;

								p1 -= Point(i, i);
								p2 -= Point(i, i);
							}

							surface->Line(p1 + Point(0, 0), p2 + Point(0, 0), color);
							surface->Line(p1 + Point(1, 0), p2 + Point(0, -1), color);
							surface->Line(p1 + Point(0, 1), p2 + Point(-1, 0), color);
							surface->Line(p1 + Point(6, 0), p2 + Point(-8, 0), color);
							surface->Line(p1 + Point(6, 1), p2 + Point(-7, 0), color);
							surface->Line(p1 + Point(5, 0), p2 + Point(-8, -1), color);
						}
					}
				}

				if (iVar != NIL)
				{
					if (*iVar == iCode)
					{
						Point	 point = Point(frame.left + 5, frame.top + 5);

						for (Int i = 0; i < 2; i++)
						{
							Int	 color = Setup::DividerDarkColor;

							if (i == 1)
							{
								if (IsActive())	color = Setup::TextColor;
								else		color = Setup::GrayTextColor;

								point -= Point(i, i);
							}

							surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, FILLED);
							surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, FILLED);
						}
					}
				}
			}

			break;
		case SP_MOUSEIN:
			if (text != NIL && bitmap == NIL)
			{
				Rect	 textRect	= Rect(realPos + Point(18, 0), GetSize() - Size(22, 2));
				Font	 nFont		= font;

				nFont.SetColor(Setup::GradientTextColor);

				surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				surface->SetText(text, textRect, nFont);

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - shortcutOffset;

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (popup != NIL)
				{
					Point	 p1 = Point(frame.right - 8, frame.top + 4);
					Point	 p2 = Point(frame.right - 8, frame.top + 11);

					for (Int i = 0; i < 4; i++) surface->Line(p1 + Point(i, i), p2 + Point(i, -i), Setup::GradientTextColor);
				}

				if (bVar != NIL)
				{
					if (*bVar == True)
					{
						Point p1 = Point(frame.left + 4, frame.top + 4);
						Point p2 = Point(frame.left + 11, frame.bottom - 4);

						for (Int i = 0; i < 2; i++)
						{
							Int	 color = Setup::InactiveGradientTextColor;

							if (i == 1)
							{
								color = Setup::GradientTextColor;

								p1 -= Point(i, i);
								p2 -= Point(i, i);
							}

							surface->Line(p1 + Point(0, 0), p2 + Point(0, 0), color);
							surface->Line(p1 + Point(1, 0), p2 + Point(0, -1), color);
							surface->Line(p1 + Point(0, 1), p2 + Point(-1, 0), color);
							surface->Line(p1 + Point(6, 0), p2 + Point(-8, 0), color);
							surface->Line(p1 + Point(6, 1), p2 + Point(-7, 0), color);
							surface->Line(p1 + Point(5, 0), p2 + Point(-8, -1), color);
						}
					}
				}

				if (iVar != NIL)
				{
					if (*iVar == iCode)
					{
						Point	 point = Point(frame.left + 5, frame.top + 5);

						for (Int i = 0; i < 2; i++)
						{
							Int	 color = Setup::InactiveGradientTextColor;

							if (i == 1)
							{
								color = Setup::GradientTextColor;

								point -= Point(i, i);
							}

							surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, FILLED);
							surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, FILLED);
						}
					}
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::PopupMenuEntry::Show()
{
	visible = True;

	return Success();
}

S::Int S::GUI::PopupMenuEntry::Hide()
{
	if (popup != NIL)
	{
		if (popup->GetContainer() == this) ClosePopupMenu();
	}

	visible = False;

	return Success();
}

S::GUI::Size S::GUI::PopupMenuEntry::GetMinimumSize() const
{
	if (text != NIL && shortcut == NIL) return Size(textSize.cx + 44, 15);
	if (text != NIL && shortcut != NIL) return Size(textSize.cx + font.GetTextSizeX(shortcut->ToString()) + 59, 15);

	return Size(15, 4);
}

S::Int S::GUI::PopupMenuEntry::GetShortcutTextSize() const
{
	if (shortcut == NIL) return 0;

	return font.GetTextSizeX(shortcut->ToString());
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
		SetActiveWindow((HWND) container->GetContainerWindow()->GetSystemWindow());
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

	if (bVar != NIL) { *bVar = !*bVar; CheckBox::internalCheckValues.Emit(); }
	if (iVar != NIL) { *iVar = iCode; OptionBox::internalCheckValues.Emit(); }

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

	if (window == NIL) return;

	if (timer != NIL)
	{
		DeleteObject(timer);

		timer = NIL;
	}

	hotspot->Deactivate();

	Point	 realPos	= GetRealPosition();
	Point	 popupPos	= realPos + Point(GetWidth(), -3);

	popup->CalculateSize();

	if (window->GetX() + popupPos.x + popup->GetWidth() >= LiSAGetDisplaySizeX()) popupPos.x = realPos.x - popup->GetWidth();
	if (window->GetY() + popupPos.y + popup->GetHeight() >= LiSAGetDisplaySizeY()) popupPos.y = realPos.y - popup->GetHeight() + GetHeight() + 3;

	popup->SetPosition(popupPos);
	popup->internalRequestClose.Connect(&PopupMenuEntry::ClosePopupMenu, this);

	owner->SetFlags(owner->GetFlags() | MB_POPUPOPEN);

	RegisterObject(popup);
}

S::Void S::GUI::PopupMenuEntry::ClosePopupMenu()
{
	if (popup == NIL) return;

	if (popup->GetContainer() == this)
	{
		owner->SetFlags(owner->GetFlags() & ~MB_POPUPOPEN);

		popup->internalRequestClose.Disconnect(&PopupMenuEntry::ClosePopupMenu, this);

		UnregisterObject(popup);

		hotspot->Activate();

		if (popup->IsClosedByClick())
		{
			owner->SetClosedByClick(True);
			owner->internalRequestClose.Emit();
		}
		else
		{
			if (!container->GetContainerWindow()->IsMouseOn(Rect(Point(), container->GetSize()))) owner->internalRequestClose.Emit();
			else										      SetActiveWindow((HWND) container->GetContainerWindow()->GetSystemWindow());
		}
	}
}

S::Bool S::GUI::PopupMenuEntry::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == MenuEntry::classID)	return True;
	else													return False;
}
