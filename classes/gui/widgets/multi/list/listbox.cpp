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
	entryCount		= -1;

	scrollbar		= NIL;
	scrollbarPos		= 0;
	lastScrollbarPos	= 0;

	header			= NIL;

	possibleContainers.AddEntry(Layer::classID);

	SetFont(Font(font.GetName(), I18N_DEFAULTFONTSIZE, Setup::ClientTextColor));

	pos			= iPos;
	size			= iSize;

	if (size.cx == 0) size.cx = 120;
	if (size.cy == 0)size.cy = 80;
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

	if (registered && container != NIL) container->UnregisterObject(this);
}

S::GUI::ListEntry *S::GUI::ListBox::AddEntry(String name, Int id)
{
	if (id >= 0 && GetEntry(id) != NIL) return NIL;

	if (id == -1) id = ++entryCount;

	ListEntry *newEntry = List::AddEntry(name, id);

	Paint(SP_UPDATE);

	return newEntry;
}

S::Int S::GUI::ListBox::ModifyEntry(Int code, String name)
{
	if (List::ModifyEntry(code, name) == Success)
	{
		Paint(SP_PAINT);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::GUI::ListBox::RemoveEntry(Int number)
{
	if (List::RemoveEntry(number) == Error) return Error;

	if (scrollbar != NIL)
	{
		if (15 * GetNOfEntries() + 4 <= size.cy)
		{
			scrollbarPos = 0;
			lastScrollbarPos = 0;

			container->UnregisterObject(scrollbar);

			DeleteObject(scrollbar);

			scrollbar = NIL;
		}
		else
		{
			scrollbarPos++;
		}
	}

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ListBox::RemoveAll()
{
	if (List::RemoveAll() == Error) return Error;

	if (scrollbar != NIL)
	{
		scrollbarPos = 0;
		lastScrollbarPos = 0;

		container->UnregisterObject(scrollbar);

		DeleteObject(scrollbar);

		scrollbar = NIL;
	}

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ListBox::SelectEntry(Int id)
{
	List::SelectEntry(id);

	Paint(SP_PAINT);

	return Success;
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
	if (header != NIL)
	{
		return header->ClearTabs();
	}

	return Success;
}

S::Int S::GUI::ListBox::GetNthTabOffset(Int n)
{
	if (header != NIL)
	{
		return header->GetNthTabOffset(n);
	}

	return 0;
}

S::Int S::GUI::ListBox::GetNthTabWidth(Int n)
{
	if (header != NIL)
	{
		return header->GetNthTabWidth(n);
	}

	return 0;
}

S::Int S::GUI::ListBox::Show()
{
	if (visible)	return Success;

	if (scrollbar != NIL)
	{
		Layer	*layer = (Layer *) container;
		Point	 realPos = GetRealPosition();
		Point	 sbp = Point(realPos.x + size.cx - layer->pos.x - 18, realPos.y + 1 - layer->pos.y + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16));
		Size	 sbs = Size(scrollbar->size.cx, size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16));

		scrollbar->SetMetrics(sbp, sbs);
		scrollbar->SetRange(0, GetNOfEntries() - (int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

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
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	Layer		*layer = (Layer *) container;
	Point		 realPos = GetRealPosition();
	ListEntry	*operat;
	Rect		 frame;
	Point		 sbp;
	Size		 sbs;
	Int		 maxFrameY;
	Int		 i;

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			if (!IsListSane()) SynchronizeList();

			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + size.cx - 1;
			frame.bottom	= realPos.y + size.cy - 1;

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

			maxFrameY = frame.bottom - 1;

			frame.left++;
			frame.top = frame.top + 1 + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);
			frame.right--;
			frame.bottom = frame.top + 15;

			frame.bottom = min(frame.bottom, maxFrameY);

			if (15 * GetNOfEntries() + (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16) + 4 > size.cy && !(flags & LF_HIDESCROLLBAR))
			{
				if (scrollbar == NIL)
				{
					sbp.x = frame.right - layer->pos.x - 16;
					sbp.y = frame.top - layer->pos.y;
					sbs.cx = 0;
					sbs.cy = size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);

					scrollbar = new Scrollbar(sbp, sbs, OR_VERT, &scrollbarPos, 0, GetNOfEntries() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

					scrollbar->onClick.Connect(&ListBox::ScrollbarProc, this);

					layer->RegisterObject(scrollbar);

					scrollbar->Paint(SP_PAINT);
				}
				else
				{
					sbp.x = frame.right - layer->pos.x - 16;
					sbp.y = frame.top - layer->pos.y;
					sbs.cy = size.cy - 2 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16);

					scrollbar->pos = sbp;
					scrollbar->size.cy = sbs.cy;

					scrollbar->SetRange(0, GetNOfEntries() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));
				}

				frame.right -= 17;
			}
			else
			{
				if (scrollbar != NIL)
				{
					scrollbarPos = 0;
					lastScrollbarPos = 0;

					layer->UnregisterObject(scrollbar);

					DeleteObject(scrollbar);

					scrollbar = NIL;
				}
			}

			lastScrollbarPos = scrollbarPos;

			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (i >= scrollbarPos && frame.top < maxFrameY)
				{
					operat->rect = frame;

					operat->rect.left++;
					operat->rect.top++;
					operat->rect.right--;

					if (i == (GetNOfEntries() - 1) || message != SP_UPDATE)
					{
						if (flags & LF_MULTICHECKBOX)
						{
							Rect	 cbRect = operat->rect;

							cbRect.left += 2;
							cbRect.top += 3;
							cbRect.right = cbRect.left + 9;
							cbRect.bottom = (Int) Math::Min(cbRect.top + 9, operat->rect.bottom);

							if (cbRect.top <= cbRect.bottom - 1)
							{
								surface->Box(cbRect, Setup::ClientColor, FILLED);
								surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

								if (operat->selected && cbRect.top <= cbRect.bottom - 3)
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

						frame.left += (2 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
						frame.top += 2;
						frame.right -= 2;

						DrawEntryText(operat->GetText(), frame, operat->font.GetColor());

						frame.left -= (2 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
						frame.top -= 2;
						frame.right += 2;
					}

					if (operat->clicked && frame.top < frame.bottom)
					{
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;
					}

					frame.top += 15;
					frame.bottom += 15;

					frame.top = (Int) Math::Min(frame.top, maxFrameY);
					frame.bottom = (Int) Math::Min(frame.bottom, maxFrameY);
				}
				else
				{
					operat->rect = Rect(Point(-1, -1), Size(0, 0));
				}
			}

			if (header != NIL && !(flags & LF_HIDEHEADER) && message != SP_UPDATE)
			{
		 		header->UpdateMetrics();
				header->Paint(SP_PAINT);
			}

			break;
		case SP_MOUSEIN:
			for (i = scrollbarPos; i < GetNOfEntries(); i++)
			{
				operat = GetNthEntry(i);

				if (operat == NIL) continue;

				if (operat->checked)
				{
					operat->rect.right++;
					operat->rect.bottom++;
					surface->Gradient(operat->rect, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
					operat->rect.right--;
					operat->rect.bottom--;

					if (flags & LF_MULTICHECKBOX)
					{
						Rect	 cbRect = operat->rect;

						cbRect.left += 2;
						cbRect.top += 3;
						cbRect.right = cbRect.left + 9;
						cbRect.bottom = (Int) Math::Min(cbRect.top + 9, operat->rect.bottom);

						if (cbRect.top <= cbRect.bottom - 1)
						{
							surface->Box(cbRect, Setup::ClientColor, FILLED);
							surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

							if (operat->selected && cbRect.top <= cbRect.bottom - 3)
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

					operat->rect.left += (1 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
					operat->rect.top++;
					operat->rect.right -= 1;
					DrawEntryText(operat->GetText(), operat->rect, Setup::GradientTextColor);
					operat->rect.left -= (1 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
					operat->rect.top--;
					operat->rect.right += 1;

					if (operat->clicked)
					{
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
			}

			break;
		case SP_MOUSEOUT:
			for (i = scrollbarPos; i < GetNOfEntries(); i++)
			{
				operat = GetNthEntry(i);

				if (operat == NIL) continue;

				if (operat->checked)
				{
					operat->rect.right++;
					operat->rect.bottom++;
					surface->Box(operat->rect, Setup::ClientColor, FILLED);
					operat->rect.right--;
					operat->rect.bottom--;

					if (flags & LF_MULTICHECKBOX)
					{
						Rect	 cbRect = operat->rect;

						cbRect.left += 2;
						cbRect.top += 3;
						cbRect.right = cbRect.left + 9;
						cbRect.bottom = (Int) Math::Min(cbRect.top + 9, operat->rect.bottom);

						if (cbRect.top <= cbRect.bottom - 1)
						{
							surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

							if (operat->selected && cbRect.top <= cbRect.bottom - 3)
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

					operat->rect.left += (1 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
					operat->rect.top++;
					operat->rect.right -= 1;
					DrawEntryText(operat->GetText(), operat->rect, operat->font.GetColor());
					operat->rect.left -= (1 + ((flags & LF_MULTICHECKBOX) ? 12 : 0));
					operat->rect.top--;
					operat->rect.right += 1;

					if (operat->clicked)
					{
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
			}

			break;
		case SP_MOUSEDOWN:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = GetNthEntry(i);

				if (operat == NIL) break;

				Window	*wnd = container->GetContainerWindow();
				if (wnd == NIL) break;

				if (wnd->IsMouseOn(operat->rect) && (!operat->clicked || (flags & LF_ALLOWRESELECT) || (flags & LF_MULTICHECKBOX)))
				{
					operat->rect.right++;
					operat->rect.bottom++;
					surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
					operat->rect.right--;
					operat->rect.bottom--;

					if (flags & LF_MULTICHECKBOX)
					{
						Rect	 cbRect = operat->rect;

						cbRect.left += 2;
						cbRect.top += 3;
						cbRect.right = cbRect.left + 9;
						cbRect.bottom = (Int) Math::Min(cbRect.top + 9, operat->rect.bottom);

						if (cbRect.top <= cbRect.bottom - 1)
						{
							surface->Box(cbRect, Setup::ClientColor, FILLED);
							surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

							if (operat->selected && cbRect.top <= cbRect.bottom - 3)
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
			}

			break;
		case SP_MOUSEUP:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = GetNthEntry(i);

				if (operat == NIL) break;

				Window	*wnd = container->GetContainerWindow();

				if (wnd == NIL) break;

				if (!wnd->IsMouseOn(operat->rect) && operat->clicked)
				{
					operat->rect.right++;
					operat->rect.bottom++;
					surface->Box(operat->rect, Setup::ClientColor, OUTLINED);
					operat->rect.right--;
					operat->rect.bottom--;
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ListBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Int	 retVal = Success;
	Bool	 change = False;
	Int	 i;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect))
				{
					wnd->Process(SM_LOOSEFOCUS, GetHandle(), 0);

					change = True;
				}
			}

			for (i = 0; i < GetNOfEntries(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (!wnd->IsMouseOn(operat->rect) && operat->clicked && change)
				{
					Paint(SP_MOUSEUP);

					operat->checked = False;
					operat->clicked = False;
				}
			}

			for (i = 0; i < GetNOfEntries(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect) && (!operat->clicked || (flags & LF_ALLOWRESELECT) || (flags & LF_MULTICHECKBOX)))
				{
					Rect	 checkRect = operat->rect;

					checkRect.right = checkRect.left + 13;

					if (wnd->IsMouseOn(checkRect))
					{
						if (operat->selected)	operat->selected = False;
						else			operat->selected = True;
					}

					Paint(SP_MOUSEDOWN);

					operat->checked = True;
					operat->clicked = True;

					onClick.Emit(wnd->MouseX(), wnd->MouseY());
					operat->onClick.Emit(wnd->MouseX(), wnd->MouseY());

					if (operat->tipTimer != NIL)
					{
						operat->tipTimer->Stop();

						DeleteObject(operat->tipTimer);

						operat->tipTimer = NIL;
					}

					if (operat->tooltip != NIL)
					{
						operat->tooltip->Hide();

						operat->tooltip->GetContainer()->GetContainerWindow()->UnregisterObject(operat->tooltip);

						DeleteObject(operat->tooltip);

						operat->tooltip = NIL;
					}

					retVal = Break;
				}
			}

			break;
		case SM_MOUSEMOVE:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (!wnd->IsMouseOn(operat->rect) && operat->checked)
				{
					Paint(SP_MOUSEOUT);

					operat->checked = False;

					operat->onMouseOut.Emit();

					if (operat->tipTimer != NIL)
					{
						operat->tipTimer->Stop();

						DeleteObject(operat->tipTimer);

						operat->tipTimer = NIL;
					}

					if (operat->tooltip != NIL)
					{
						operat->tooltip->Hide();

						operat->tooltip->GetContainer()->GetContainerWindow()->UnregisterObject(operat->tooltip);

						DeleteObject(operat->tooltip);

						operat->tooltip = NIL;
					}
				}
			}

			for (i = 0; i < GetNOfEntries(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect) && !operat->checked)
				{
					operat->checked = True;

					Paint(SP_MOUSEIN);

					operat->onMouseOver.Emit();

					if (operat->tooltipText != NIL)
					{
						operat->tipTimer = new System::Timer();

						operat->tipTimer->onInterval.Connect(&ListEntry::ActivateTooltip, operat);
						operat->tipTimer->Start(500);
					}
				}
				else if (operat->checked && wnd->IsMouseOn(operat->rect))
				{
					if (operat->tipTimer != NIL && wParam == 0)
					{
						operat->tipTimer->Stop();
						operat->tipTimer->Start(500);
					}
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::ListBox::DrawEntryText(String newText, Rect rect, Int color)
{
	Surface	*surface = container->GetDrawSurface();
	Bool	 gotTabs = False;

	for (Int r = 0; r < newText.Length(); r++)
	{
		if (newText[r] == '\t')
		{
			gotTabs = True;

			break;
		}
	}

	Font	 nFont = font;

	nFont.SetColor(color);

	if (header != NIL && gotTabs)
	{
		for (Int i = 0; i < header->GetNOfTabs(); i++)
		{
			String	 nText;
			Rect	 rRect = rect;

			rRect.left += header->GetNthTabOffset(i);
			rRect.left -= (i >= 1 ? ((flags & LF_MULTICHECKBOX) ? 12 : 0) : 0);

			if (header->GetNOfTabs() >= i + 2) rRect.right = rRect.left + (header->GetNthTabOffset(i + 1) - header->GetNthTabOffset(i)) - 3;

			Int	 tabCount = 0;

			for (Int p = 0; p < newText.Length(); p++)
			{
				if (tabCount == i)
				{
					for (Int q = p; q < newText.Length(); q++)
					{
						if (newText[q] == '\t') break;

						nText[q - p] = newText[q];
					}

					break;
				}

				if (newText[p] == '\t') tabCount++;
			}

			surface->SetText(nText, rRect, nFont);
		}
	}
	else
	{
		surface->SetText(newText, rect, nFont);
	}
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
	scrollbarPos = Math::Min(scrollbarPos, GetNOfEntries() - (Int) ((size.cy - 4 - (header == NIL || (flags & LF_HIDEHEADER) ? 0 : 16)) / 15));

	if (scrollbar != NIL) scrollbar->Paint(SP_PAINT);

	ScrollbarProc();

	return Success;
}

S::Void S::GUI::ListBox::ScrollbarProc()
{
	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + size.cx - 1;
	frame.bottom	= realPos.y + size.cy - 1;

	if (scrollbarPos != lastScrollbarPos)
	{
		lastScrollbarPos = scrollbarPos;

		surface->StartPaint(frame);
		Paint(SP_PAINT);
		surface->EndPaint();
	}
}

S::Void S::GUI::ListBox::CheckFlags()
{
	if (flags & LF_ADDNILENTRY) addNil = True;
}
