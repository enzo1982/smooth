 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/listbox.h>
#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/scrollbar.h>
#include <smooth/layer.h>
#include <smooth/surface.h>
#include <smooth/listboxheader.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_LISTBOX = S::Object::RequestObjectID();

S::GUI::ListBox::ListBox(Point pos, Size size)
{
	type		= OBJ_LISTBOX;
	entryCount	= -1;

	needScrollbar		= False;
	scrollbar		= NIL;
	scrollbarPos		= 0;
	lastScrollbarPos	= 0;
	allowReselect		= False;

	header			= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, Setup::ClientTextColor, objectProperties->fontWeight);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(120 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::ListBox::~ListBox()
{
	if (needScrollbar)
	{
		if (myContainer != NIL) myContainer->UnregisterObject(scrollbar);

		delete scrollbar;
	}

	if (header != NIL)
	{
		if (myContainer != NIL) myContainer->UnregisterObject(header);

		delete header;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::ListBox::AllowReselect(Bool value)
{
	allowReselect = value;

	return Success;
}

S::List::Entry *S::GUI::ListBox::AddEntry(String name)
{
	entryCount++;

	Entry *newEntry = AddListEntry(entryCount, name);

	Paint(SP_UPDATE);

	return newEntry;
}

S::Int S::GUI::ListBox::ModifyEntry(Int code, String name)
{
	if (ModifyListEntry(code, name) == Success)
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
	RemoveListEntry(number);

	if (needScrollbar)
	{
		needScrollbar = False;

		if (METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4 <= objectProperties->size.cy)
		{
			scrollbarPos = 0;
			lastScrollbarPos = 0;
		}

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);

	return Success;
}

S::Void S::GUI::ListBox::Cleanup()
{
	CleanupList();

	if (needScrollbar)
	{
		needScrollbar = False;
		scrollbarPos = 0;
		lastScrollbarPos = 0;

		myContainer->UnregisterObject(scrollbar);

		delete scrollbar;

		scrollbar = NIL;
	}

	Paint(SP_PAINT);
}

S::Int S::GUI::ListBox::SelectEntry(Int code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ListBox::AddTab(String tabName, Int iTabWidth)
{
	if (header == NIL)
	{
		header = new ListBoxHeader(this);

		if (visible) myContainer->RegisterObject(header);
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

S::Int S::GUI::ListBox::Show()
{
	if (visible)	return Success;

	if (needScrollbar)
	{
		Layer	*layer = (Layer *) myContainer->GetContainerObject();
		Point	 realPos = GetRealPosition();
		Point	 sbp = Point(realPos.x + objectProperties->size.cx - 2 - layer->GetObjectProperties()->pos.x - METRIC_LISTBOXSBOFFSET, realPos.y + 1 - layer->GetObjectProperties()->pos.y + (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1));
		Size	 sbs = Size(METRIC_LISTBOXSBSIZE, objectProperties->size.cy - 1 - (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1));
		Float	 oldMeasurement = Setup::FontSize;

		SetMeasurement(SMT_PIXELS);

		scrollbar->SetMetrics(sbp, sbs);
		scrollbar->SetRange(0, nOfEntries - (int) ((objectProperties->size.cy - 4 - (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1)) / METRIC_LISTBOXENTRYHEIGHT));

		Setup::FontSize = oldMeasurement;

		scrollbar->Show();
	}

	if (header != NIL)
	{
		header->UpdateMetrics();

		myContainer->RegisterObject(header);
	}

	return Widget::Show();
}

S::Int S::GUI::ListBox::Hide()
{
	if (!visible)	return Success;

	if (header != NIL) myContainer->UnregisterObject(header);

	if (needScrollbar) scrollbar->Hide();

	return Widget::Hide();
}

S::Int S::GUI::ListBox::Activate()
{
	Int	 rVal = Widget::Activate();

	if (rVal == Success && header != NIL) header->Activate();

	return rVal;
}

S::Int S::GUI::ListBox::Deactivate()
{
	Int	 rVal = Widget::Deactivate();

	if (rVal == Success && header != NIL) header->Deactivate();

	return rVal;
}

S::Int S::GUI::ListBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Layer		*layer = (Layer *) myContainer->GetContainerObject();
	Point		 realPos = GetRealPosition();
	List::Entry	*operat;
	Rect		 frame;
	Point		 sbp;
	Size		 sbs;
	Int		 maxFrameY;
	Float		 oldMeasurement;

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			if (message != SP_UPDATE)
			{
				if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
				else		surface->Box(frame, Setup::BackgroundColor, FILLED);

				surface->Frame(frame, FRAME_DOWN);
			}

			maxFrameY = frame.bottom - 1;

			frame.left++;
			frame.top = frame.top + 1 + (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1);
			frame.right--;
			frame.bottom = frame.top + METRIC_LISTBOXENTRYHEIGHT;

			frame.bottom = min(frame.bottom, maxFrameY);

			if (METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4 > objectProperties->size.cy)
			{
				if (!needScrollbar)
				{
					needScrollbar = True;

					sbp.x = frame.right - layer->GetObjectProperties()->pos.x - METRIC_LISTBOXSBOFFSET;
					sbp.y = frame.top - layer->GetObjectProperties()->pos.y;
					sbs.cx = METRIC_LISTBOXSBSIZE;
					sbs.cy = objectProperties->size.cy - 1 - (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1);

					oldMeasurement = Setup::FontSize;

					SetMeasurement(SMT_PIXELS);

					scrollbar = new Scrollbar(sbp, sbs, OR_VERT, &scrollbarPos, 0, nOfEntries - (int) ((objectProperties->size.cy - 4 - (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1)) / METRIC_LISTBOXENTRYHEIGHT));

					scrollbar->onClick.Connect(&ListBox::ScrollbarProc, this);

					Setup::FontSize = oldMeasurement;

					layer->RegisterObject(scrollbar);

					scrollbar->Paint(SP_PAINT);
				}
				else
				{
					scrollbar->SetRange(0, nOfEntries - (int) ((objectProperties->size.cy - 4 - (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1)) / METRIC_LISTBOXENTRYHEIGHT));
				}

				frame.right -= (METRIC_LISTBOXSBOFFSET + 1);
			}
			else
			{
				if (needScrollbar)
				{
					needScrollbar = False;
					scrollbarPos = 0;
					lastScrollbarPos = 0;

					layer->UnregisterObject(scrollbar);

					delete scrollbar;
					scrollbar = NIL;
				}
			}

			lastScrollbarPos = scrollbarPos;

			for (int i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (i >= scrollbarPos && frame.top < maxFrameY)
				{
					operat->rect = frame;

					operat->rect.left++;
					operat->rect.top++;
					operat->rect.right--;

					if (i == (nOfEntries - 1) || message != SP_UPDATE)
					{
						frame.left += METRIC_LISTBOXTEXTOFFSETXY;
						frame.top += METRIC_LISTBOXTEXTOFFSETXY;
						DrawEntryText(operat->text, frame, objectProperties->fontColor);
						frame.left -= METRIC_LISTBOXTEXTOFFSETXY;
						frame.top -= METRIC_LISTBOXTEXTOFFSETXY;
					}

					if (operat->clk && frame.top < frame.bottom)
					{
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;
					}

					frame.top += METRIC_LISTBOXENTRYHEIGHT;
					frame.bottom += METRIC_LISTBOXENTRYHEIGHT;

					frame.top = min(frame.top, maxFrameY);
					frame.bottom = min(frame.bottom, maxFrameY);
				}
				else
				{
					operat->rect = Rect(Point(-1, -1), Size(0, 0));
				}
			}

			break;
	}

	LeaveProtectedRegion();

	if (needScrollbar) scrollbar->Paint(SP_PAINT);
	if (header != NIL) header->Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ListBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window		*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface		*surface = myContainer->GetDrawSurface();
	Point		 realPos = GetRealPosition();
	Int		 retVal = Success;
	List::Entry	*operat;
	Rect		 frame;
	Bool		 change = False;
	Int		 maxFrameY;
	Int		 i;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_CHECKLISTBOXES:
			if (scrollbarPos != lastScrollbarPos)
			{
				lastScrollbarPos = scrollbarPos;

				frame.left++;
				frame.top = frame.top + 1 + (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1);
				frame.right--;
				frame.bottom--;

				frame.right -= (METRIC_LISTBOXSBOFFSET + 1);

				surface->Box(frame, Setup::ClientColor, FILLED);

				frame.left	= realPos.x;
				frame.top	= realPos.y;
				frame.right	= realPos.x + objectProperties->size.cx - 1;
				frame.bottom	= realPos.y + objectProperties->size.cy - 1;

				maxFrameY = frame.bottom - 1;

				frame.left++;
				frame.top	= frame.top + 1 + (header == NIL ? 0 : METRIC_LISTBOXENTRYHEIGHT + 1);
				frame.right	-= (METRIC_LISTBOXSBOFFSET + 2);
				frame.bottom	= frame.top + METRIC_LISTBOXENTRYHEIGHT;

				frame.bottom = min(frame.bottom, maxFrameY);

				for (i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat == NIL) break;

					if (i >= scrollbarPos && frame.top < maxFrameY)
					{
						operat->rect = frame;

						operat->rect.left++;
						operat->rect.top++;
						operat->rect.right--;

						frame.left += METRIC_LISTBOXTEXTOFFSETXY;
						frame.top += METRIC_LISTBOXTEXTOFFSETXY;
						DrawEntryText(operat->text, frame, objectProperties->fontColor);
						frame.left -= METRIC_LISTBOXTEXTOFFSETXY;
						frame.top -= METRIC_LISTBOXTEXTOFFSETXY;

						if (operat->clk && frame.top < frame.bottom)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}

						frame.top += METRIC_LISTBOXENTRYHEIGHT;
						frame.bottom += METRIC_LISTBOXENTRYHEIGHT;

						frame.top = min(frame.top, maxFrameY);
						frame.bottom = min(frame.bottom, maxFrameY);
					}
					else
					{
						operat->rect = Rect(Point(-1, -1), Size(0, 0));
					}
				}

				retVal = Break;
			}

			break;
		case SM_LBUTTONDOWN:
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect))
				{
					wnd->Process(SM_LOOSEFOCUS, handle, 0);

					change = True;
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!wnd->IsMouseOn(operat->rect))
				{
					if (operat->clk && change)
					{
						operat->chk = False;
						operat->clk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect))
				{
					if (!operat->clk || allowReselect)
					{
						operat->chk = True;
						operat->clk = True;
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
						operat->rect.right--;
						operat->rect.bottom--;

						onClick.Emit();
						operat->onClick.Emit();
					}

					retVal = Break;
				}
			}

			break;
		case SM_MOUSELEAVE:
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!wnd->IsMouseOn(operat->rect))
				{
					if (operat->chk)
					{
						operat->chk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientColor, FILLED);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						DrawEntryText(operat->text, operat->rect, objectProperties->fontColor);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			break;
		case SM_MOUSEMOVE:
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (!wnd->IsMouseOn(operat->rect))
				{
					if (operat->chk)
					{
						operat->chk = False;
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::ClientColor, FILLED);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						DrawEntryText(operat->text, operat->rect, objectProperties->fontColor);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat == NIL) break;

				if (wnd->IsMouseOn(operat->rect))
				{
					if (!operat->chk)
					{
						operat->chk = True;
						operat->rect.right++;
						operat->rect.bottom++;
						surface->Gradient(operat->rect, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
						operat->rect.right--;
						operat->rect.bottom--;

						operat->rect.left++;
						operat->rect.top++;
						DrawEntryText(operat->text, operat->rect, Setup::GradientTextColor);
						operat->rect.left--;
						operat->rect.top--;

						if (operat->clk)
						{
							operat->rect.right++;
							operat->rect.bottom++;
							surface->Box(operat->rect, Setup::ClientTextColor, OUTLINEDOTS);
							operat->rect.right--;
							operat->rect.bottom--;
						}
					}
				}
			}

			break;
	}

	return retVal;
}

S::Void S::GUI::ListBox::DrawEntryText(String text, Rect rect, Int color)
{
	Surface	*surface = myContainer->GetDrawSurface();

	if (header != NIL)
	{
		for (Int i = 0; i < header->GetNOfTabs(); i++)
		{
			String	 nText;
			Rect	 rRect = rect;

			rRect.left += header->GetNthTabOffset(i);

			Int	 tabCount = 0;

			for (Int p = 0; p < text.Length(); p++)
			{
				if (tabCount == i)
				{
					for (Int q = p; q < text.Length(); q++)
					{
						if (text[q] == '\t') break;

						nText[q - p] = text[q];
					}

					break;
				}

				if (text[p] == '\t') tabCount++;
			}

			surface->SetText(nText, rRect, objectProperties->font, objectProperties->fontSize, color, objectProperties->fontWeight);
		}
	}
	else
	{
		surface->SetText(text, rect, objectProperties->font, objectProperties->fontSize, color, objectProperties->fontWeight);
	}
}

S::Void S::GUI::ListBox::ScrollbarProc()
{
	Process(SM_CHECKLISTBOXES, 0, 0);
}
