 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listentry.h>
#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/window/window.h>
#include <smooth/system/timer.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>

S::GUI::ListEntry::ListEntry(String iText)
{
	marked		= False;

	text		= iText;

	tipTimer	= NIL;
	tooltip		= NIL;

	font.SetColor(Setup::ClientTextColor);

	onMouseOver.SetParentObject(this);
	onMouseOut.SetParentObject(this);

	possibleContainers.AddEntry(List::classID);

	onRegister.Connect(&ListEntry::OnRegister, this);
	onUnregister.Connect(&ListEntry::OnUnregister, this);
}

S::GUI::ListEntry::~ListEntry()
{
	if (tipTimer != NIL)
	{
		tipTimer->Stop();

		DeleteObject(tipTimer);

		tipTimer = NIL;
	}

	if (tooltip != NIL)
	{
		Window	*wnd = tooltip->GetContainer()->GetContainerWindow();

		tooltip->Hide();

		wnd->UnregisterObject(tooltip);

		DeleteObject(tooltip);

		tooltip = NIL;
	}
}

S::Int S::GUI::ListEntry::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();
	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, size);

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			if (message != SP_UPDATE)
			{
				if (container->GetFlags() & LF_MULTICHECKBOX)
				{
					Rect	 cbRect = frame;

					cbRect.left += 2;
					cbRect.top += 3;
					cbRect.right = cbRect.left + 9;
					cbRect.bottom = (Int) Math::Min(cbRect.top + 9, frame.bottom);

					if (cbRect.top <= cbRect.bottom - 1)
					{
						surface->Box(cbRect, Setup::ClientColor, FILLED);
						surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

						if (marked && cbRect.top <= cbRect.bottom - 3)
						{
							Point	 p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							Point	 p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.left + 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 4 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);
						}
					}
				}
			}

			if (frame.top < frame.bottom)
			{
				frame.right++;
				frame.bottom++;

				if (clicked)	surface->Box(frame, Setup::ClientTextColor, OUTLINEDOTS);
				else		surface->Box(frame, Setup::ClientColor, OUTLINED);

				if (checked)	surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);

				if (checked)	DrawEntryText(Setup::GradientTextColor);
				else		DrawEntryText(font.GetColor());

				frame.right--;
				frame.bottom--;
			}

			break;
		case SP_MOUSEIN:
			if (checked)
			{
				frame.right++;
				frame.bottom++;
				surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				frame.right--;
				frame.bottom--;

				if (container->GetFlags() & LF_MULTICHECKBOX)
				{
					Rect	 cbRect = frame;

					cbRect.left += 2;
					cbRect.top += 3;
					cbRect.right = cbRect.left + 9;
					cbRect.bottom = (Int) Math::Min(cbRect.top + 9, frame.bottom);

					if (cbRect.top <= cbRect.bottom - 1)
					{
						surface->Box(cbRect, Setup::ClientColor, FILLED);
						surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

						if (marked && cbRect.top <= cbRect.bottom - 3)
						{
							Point	 p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							Point	 p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.left + 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 4 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);
						}
					}
				}

				DrawEntryText(Setup::GradientTextColor);

				if (clicked)
				{
					frame.right++;
					frame.bottom++;
					surface->Box(frame, Setup::ClientTextColor, OUTLINEDOTS);
					frame.right--;
					frame.bottom--;
				}
			}

			break;
		case SP_MOUSEOUT:
			if (checked)
			{
				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::ClientColor, FILLED);
				frame.right--;
				frame.bottom--;

				if (container->GetFlags() & LF_MULTICHECKBOX)
				{
					Rect	 cbRect = frame;

					cbRect.left += 2;
					cbRect.top += 3;
					cbRect.right = cbRect.left + 9;
					cbRect.bottom = (Int) Math::Min(cbRect.top + 9, frame.bottom);

					if (cbRect.top <= cbRect.bottom - 1)
					{
						surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

						if (marked && cbRect.top <= cbRect.bottom - 3)
						{
							Point	 p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							Point	 p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.left + 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 4 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);
						}
					}
				}

				DrawEntryText(font.GetColor());

				if (clicked)
				{
					frame.right++;
					frame.bottom++;
					surface->Box(frame, Setup::ClientTextColor, OUTLINEDOTS);
					frame.right--;
					frame.bottom--;
				}
			}

			break;
		case SP_MOUSEDOWN:
			if (wnd->IsMouseOn(frame) && (!clicked || (container->GetFlags() & LF_ALLOWRESELECT) || (container->GetFlags() & LF_MULTICHECKBOX)))
			{
				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::ClientTextColor, OUTLINEDOTS);
				frame.right--;
				frame.bottom--;

				if (container->GetFlags() & LF_MULTICHECKBOX)
				{
					Rect	 cbRect = frame;

					cbRect.left += 2;
					cbRect.top += 3;
					cbRect.right = cbRect.left + 9;
					cbRect.bottom = (Int) Math::Min(cbRect.top + 9, frame.bottom);

					if (cbRect.top <= cbRect.bottom - 1)
					{
						surface->Box(cbRect, Setup::ClientColor, FILLED);
						surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

						if (marked && cbRect.top <= cbRect.bottom - 3)
						{
							Point	 p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							Point	 p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.left + 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);

							p1 = Point(cbRect.right - 3 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 3);
							p2 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::GrayTextColor);

							p1 = Point(cbRect.right - 4 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 1 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 3);

							surface->Line(p1, p2, Setup::GrayTextColor);
						}
					}
				}
			}

			break;
		case SP_MOUSEUP:
			if (!wnd->IsMouseOn(frame) && clicked)
			{
				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::ClientColor, OUTLINED);
				frame.right--;
				frame.bottom--;
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ListEntry::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, size);
	Int	 retVal		= Success;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (wnd->IsMouseOn(frame) && (!clicked || (container->GetFlags() & LF_ALLOWRESELECT) || (container->GetFlags() & LF_MULTICHECKBOX)))
			{
				Rect	 checkRect = frame;

				checkRect.right = checkRect.left + 13;

				if (wnd->IsMouseOn(checkRect)) marked = !marked;

				Paint(SP_MOUSEDOWN);

				checked = True;
				clicked = True;

				((List *) container)->internalOnSelectEntry.Emit();

				container->onClick.Emit(wnd->MouseX(), wnd->MouseY());
				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				if (tipTimer != NIL)
				{
					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}

				if (tooltip != NIL)
				{
					tooltip->Hide();

					wnd->UnregisterObject(tooltip);

					DeleteObject(tooltip);

					tooltip = NIL;
				}
			}

			break;
		case SM_MOUSEMOVE:
			if (!wnd->IsMouseOn(frame) && checked)
			{
				Paint(SP_MOUSEOUT);

				checked = False;

				onMouseOut.Emit();

				if (tipTimer != NIL)
				{
					tipTimer->Stop();

					DeleteObject(tipTimer);

					tipTimer = NIL;
				}

				if (tooltip != NIL)
				{
					tooltip->Hide();

					wnd->UnregisterObject(tooltip);

					DeleteObject(tooltip);

					tooltip = NIL;
				}
			}
			else if (wnd->IsMouseOn(frame) && !checked)
			{
				checked = True;

				Paint(SP_MOUSEIN);

				onMouseOver.Emit();

				if (tooltipText != NIL)
				{
					tipTimer = new System::Timer();

					tipTimer->onInterval.Connect(&ListEntry::ActivateTooltip, this);
					tipTimer->Start(500);
				}
			}
			else if (wnd->IsMouseOn(frame) && checked)
			{
				if (tipTimer != NIL && wParam == 0)
				{
					tipTimer->Stop();
					tipTimer->Start(500);
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::ListEntry::DrawEntryText(Int color)
{
	if (container->GetObjectType() != ListBox::classID) return;

	Surface	*surface	= container->GetDrawSurface();
	Bool	 gotTabs	= False;

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, size);

	frame.left += (1 + ((container->GetFlags() & LF_MULTICHECKBOX) ? 12 : 0));
	frame.top++;
	frame.right -= 1;

	Font	 nFont = font;

	nFont.SetColor(color);

	for (Int r = 0; r < text.Length(); r++)
	{
		if (text[r] == '\t')
		{
			gotTabs = True;

			break;
		}
	}

	if (((ListBox *) container)->GetNOfTabs() > 0 && gotTabs)
	{
		for (Int i = 0; i < ((ListBox *) container)->GetNOfTabs(); i++)
		{
			String	 tabText;
			Rect	 rect = frame;

			rect.left += ((ListBox *) container)->GetNthTabOffset(i);
			rect.left -= (i >= 1 ? ((container->GetFlags() & LF_MULTICHECKBOX) ? 12 : 0) : 0);

			if (((ListBox *) container)->GetNOfTabs() >= i + 2) rect.right = rect.left + (((ListBox *) container)->GetNthTabOffset(i + 1) - ((ListBox *) container)->GetNthTabOffset(i)) - 3;

			Int	 tabCount = 0;

			for (Int p = 0; p < text.Length(); p++)
			{
				if (tabCount == i)
				{
					for (Int q = p; q < text.Length(); q++)
					{
						if (text[q] == '\t') break;

						tabText[q - p] = text[q];
					}

					break;
				}

				if (text[p] == '\t') tabCount++;
			}

			surface->SetText(tabText, rect, nFont);
		}
	}
	else
	{
		surface->SetText(text, frame, nFont);
	}
}

S::Int S::GUI::ListEntry::SetMark(Bool nMarked)
{
	marked = nMarked;

	Paint(SP_PAINT);

	return Success;
}

S::Bool S::GUI::ListEntry::IsMarked()
{
	return marked;
}

S::Void S::GUI::ListEntry::OnRegister()
{
	((List *) container)->internalOnSelectEntry.Connect(&ListEntry::SelectEntry, this);
}

S::Void S::GUI::ListEntry::OnUnregister()
{
	((List *) container)->internalOnSelectEntry.Disconnect(&ListEntry::SelectEntry, this);
}

S::Int S::GUI::ListEntry::SetText(const String &newText)
{
	Bool	 prevVisible = IsVisible();

	if (registered && prevVisible) Hide();

	text = newText;

	GetTextSize();

	checked = False;

	if (registered && prevVisible) Show();

	Process(SM_MOUSEMOVE, 0, 0);

	return Success;
}

S::Void S::GUI::ListEntry::SelectEntry()
{
	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return;

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, size);
	
	if (!wnd->IsMouseOn(frame) && clicked)
	{
		Paint(SP_MOUSEUP);

		clicked = False;
	}
}

S::Void S::GUI::ListEntry::ActivateTooltip()
{
	if (tooltip != NIL)	return;
	if (tipTimer == NIL)	return;

	tipTimer->Stop();

	DeleteObject(tipTimer);

	tipTimer = NIL;

	Window	*wnd	= container->GetContainerWindow();
	Point	 nPos	= Point(wnd->MouseX(), wnd->MouseY());
	Size	 nSize	= Size(0, 0);

	tooltip = new Tooltip();

	tooltip->SetText(tooltipText);
	tooltip->SetMetrics(nPos, nSize);
	tooltip->SetTimeout(3000);

	wnd->RegisterObject(tooltip);
}
