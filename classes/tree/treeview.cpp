 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/treeview.h>
#include <smooth/tree.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/misc/math.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/window/window.h>

const S::Int	 S::GUI::TreeView::classID = S::Object::RequestClassID();

S::GUI::TreeView::TreeView(String name, Point pos, Size size)
{
	type				= classID;
	objectProperties->text		= name;

	objectProperties->font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(100 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(120 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::TreeView::~TreeView()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::TreeView::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	surface->Box(frame, Setup::ClientColor, FILLED);
	surface->Frame(frame, FRAME_DOWN);

	if (objectProperties->text != NIL)
	{
		frame.left += 3;
		frame.top += 2;

		surface->SetText(objectProperties->text, frame, objectProperties->font);

		frame.left -= 3;
		frame.top -= 2;
	}

	frame.top += 15;
	frame.right--;

	PaintTree(this, 0, frame);

	return Success;
}

S::Int S::GUI::TreeView::PaintTree(Tree *tree, Int level, Rect frame)
{
	Surface		*surface = myContainer->GetDrawSurface();
	Tree::Entry	*operat;
	Rect		 oframe = frame;
	Rect		 rect;
	Point		 p1;
	Point		 p2;
	Int		 ste;
	Int		 painted = tree->GetNOfEntries();

	frame.left += 15 * (level + 1);
	frame.bottom = frame.top + 16;

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

		surface->SetText(operat->text, frame, objectProperties->font);

		frame.left -= 3;
		frame.top -= 3;

		if (operat->type == ST_ENTRY)
		{
			for (Int y = frame.top - 2; y < frame.bottom; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if ((frame.top - y) & 1)
				{
					surface->SetPixel(frame.left - 6, y, Setup::GrayTextColor);
				}

				if (y == frame.top + 9)
				{
					for (Int x = frame.left - 5; x <= frame.left + 2; x++)
					{
						if (((frame.left - 5) - x) & 1)
						{
							surface->SetPixel(x, y, Setup::GrayTextColor);
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

			if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::GrayTextColor);

			p1.y = p1.y + 8;
			p2.y = p1.y;

			if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::GrayTextColor);

			p1.y = frame.top + 5;
			p2.y = min(p2.y + 1, oframe.bottom - 1);
			p2.x = p1.x;

			if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::GrayTextColor);

			p1.x = p1.x + 8;
			p2.x = p1.x;

			if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::GrayTextColor);

			p1.x = rect.left + 2;
			p1.y = rect.top + 4;
			p2.x = rect.right - 2;
			p2.y = p1.y;

			if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::ClientTextColor);

			if (!operat->open)
			{
				p1.x = rect.left + 4;
				p1.y = rect.top + 2;
				p2.x = p1.x;
				p2.y = min(rect.bottom - 2, oframe.bottom - 1);

				if (p1.y < oframe.bottom - 1) surface->Line(p1, p2, Setup::ClientTextColor);
			}

			for (Int y = frame.top - 2; y <= frame.top + 9; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if (((frame.top - y) & 1) && (y < frame.top + 7))
				{
					surface->SetPixel(frame.left - 6, y, Setup::GrayTextColor);
				}

				if (y == frame.top + 9)
				{
					for (Int x = frame.left - 3; x <= frame.left + 2; x++)
					{
						if (((frame.left - 5) - x) & 1)
						{
							surface->SetPixel(x, y, Setup::GrayTextColor);
						}
					}
				}
			}
		}

		oframe.top += 16;

		if (operat->open)
		{
			ste = PaintTree(operat->sub, level + 1, oframe);

			for (Int y = frame.top + 14; y <= frame.bottom + 16 * ste; y++)
			{
				if (y >= oframe.bottom - 1) break;

				if ((frame.top - y) & 1)
				{
					surface->SetPixel(frame.left - 6, y, Setup::GrayTextColor);
				}
			}

			painted += ste;
		}

		oframe.top += 16 * ste;
		frame.top += 16 + 16 * ste;
		frame.bottom += 16 + 16 * ste;
	}

	return painted;
}

S::Int S::GUI::TreeView::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			break;
		case SM_MOUSEMOVE:
			break;
	}

	return retVal;
}
