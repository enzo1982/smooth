 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/treeview.h>
#include <smooth/tree.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TREEVIEW = S::Object::RequestObjectID();

S::GUI::TreeView::TreeView(String name, Point pos, Size size)
{
	type				= OBJ_TREEVIEW;
	objectProperties->text		= name;
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(120 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);
}

S::GUI::TreeView::~TreeView()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::TreeView::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	Frame(dc, frame, FRAME_DOWN);

	if (objectProperties->text != NIL)
	{
		frame.left += 3;
		frame.top += 2;

		::SetText(dc, objectProperties->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

		frame.left -= 3;
		frame.top -= 2;
	}

	frame.top += (METRIC_TREEVIEWENTRYHEIGHT - 1);
	frame.right--;

	PaintTree(this, 0, frame);

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::GUI::TreeView::PaintTree(Tree *tree, Int level, Rect frame)
{
	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Tree::Entry	*operat;
	HDC		 dc = GetContext(wnd);
	Rect		 oframe = frame;
	Rect		 rect;
	Point		 p1;
	Point		 p2;
	Int		 ste;
	Int		 painted = tree->GetNOfEntries();

	frame.left += (METRIC_TREEVIEWENTRYHEIGHT - 1) * (level + 1);
	frame.bottom = frame.top + METRIC_TREEVIEWENTRYHEIGHT;

	for (Int i = 0; i < tree->GetNOfEntries(); i++)
	{
		if (frame.top >= oframe.bottom) break;

		operat = tree->entries.GetNthEntry(i);

		ste = 0;

		operat->rect = frame;

		operat->rect.left++;
		operat->rect.top++;
		operat->rect.right--;

		frame.left += 3;
		frame.top += 3;
		frame.bottom = min(frame.bottom, oframe.bottom - 1);

		::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

		frame.left -= 3;
		frame.top -= 3;

		if (operat->type == ST_ENTRY)
		{
			for (Int y = frame.top - 2; y < frame.bottom; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if ((frame.top - y) & 1)
				{
					PaintPixel(dc, Point(frame.left - 6, y), SMOOTH::Setup::GrayTextColor);
				}

				if (y == frame.top + 9)
				{
					for (Int x = frame.left - 5; x <= frame.left + 2; x++)
					{
						if (((frame.left - 5) - x) & 1)
						{
							PaintPixel(dc, Point(x, y), SMOOTH::Setup::GrayTextColor);
						}
					}
				}

				if (operat->last && y == frame.top + 9) break;
			}
		}
		else if (operat->type == ST_SUB)
		{
			rect.left = frame.left - 10;
			rect.top = frame.top + 5;
			rect.right = rect.left + 9;
			rect.bottom = rect.top + 9;

			p1.x = frame.left - 10;
			p1.y = frame.top + 5;
			p2.x = p1.x + 8;
			p2.y = p1.y;

			if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

			p1.y = p1.y + 8;
			p2.y = p1.y;

			if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

			p1.y = frame.top + 5;
			p2.y = min(p2.y + 1, oframe.bottom - 1);
			p2.x = p1.x;

			if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

			p1.x = p1.x + 8;
			p2.x = p1.x;

			if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

			p1.x = rect.left + 2;
			p1.y = rect.top + 4;
			p2.x = rect.right - 2;
			p2.y = p1.y;

			if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);

			if (!operat->open)
			{
				p1.x = rect.left + 4;
				p1.y = rect.top + 2;
				p2.x = p1.x;
				p2.y = min(rect.bottom - 2, oframe.bottom - 1);

				if (p1.y < oframe.bottom - 1) Line(dc, p1, p2, SMOOTH::Setup::ClientTextColor, PS_SOLID, 1);
			}

			for (Int y = frame.top - 2; y <= frame.top + 9; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if (((frame.top - y) & 1) && (y < frame.top + 7))
				{
					PaintPixel(dc, Point(frame.left - 6, y), SMOOTH::Setup::GrayTextColor);
				}

				if (y == frame.top + 9)
				{
					for (Int x = frame.left - 3; x <= frame.left + 2; x++)
					{
						if (((frame.left - 5) - x) & 1)
						{
							PaintPixel(dc, Point(x, y), SMOOTH::Setup::GrayTextColor);
						}
					}
				}
			}
		}

		oframe.top += METRIC_TREEVIEWENTRYHEIGHT;

		if (operat->open)
		{
			ste = PaintTree(operat->sub, level + 1, oframe);

			for (Int y = frame.top + (METRIC_TREEVIEWENTRYHEIGHT - 2); y <= frame.bottom + METRIC_TREEVIEWENTRYHEIGHT * ste; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if ((frame.top - y) & 1)
				{
					PaintPixel(dc, Point(frame.left - 6, y), SMOOTH::Setup::GrayTextColor);
				}
			}

			painted += ste;
		}

		oframe.top += METRIC_TREEVIEWENTRYHEIGHT * ste;
		frame.top += METRIC_TREEVIEWENTRYHEIGHT + METRIC_TREEVIEWENTRYHEIGHT * ste;
		frame.bottom += METRIC_TREEVIEWENTRYHEIGHT + METRIC_TREEVIEWENTRYHEIGHT * ste;
	}

	FreeContext(wnd, dc);

	return painted;
}

S::Int S::GUI::TreeView::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	HDC	 dc;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}
