 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_POPUPVIEW_
#define __OBJSMOOTH_POPUPVIEW_

#include <smooth/popupview.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>
#include <smooth/metrics.h>
#include <smooth/optionbox.h>
#include <smooth/checkbox.h>
#include <smooth/objectmanager.h>
#include <smooth/objectproperties.h>
#include <smooth/popupmenu.h>
#include <smooth/toolwindow.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_POPUPVIEW = SMOOTH::RequestObjectID();

SMOOTHPopupView::SMOOTHPopupView(SMOOTHPopupMenu *popupMenu)
{
	type				= OBJ_POPUPVIEW;
	objectProperties->orientation	= OR_FREE;
	myPopup				= popupMenu;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHPopupView::~SMOOTHPopupView()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHPopupView::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHMenu::Entry	*entry;
	SMOOTHRect		 popupRect;
	HDC			 dc = GetContext(wnd);
	SMOOTHPoint		 p1;
	SMOOTHPoint		 p2;
	SMOOTHRect		 frame;
	SMOOTHRect		 textRect;
	SMOOTHInt		 currentXPos = 20;
	SMOOTHInt		 currentYPos = 3;

	myPopup->GetSize();

	popupRect.left		= 0;
	popupRect.top		= 0;
	popupRect.right		= myPopup->popupsize.cx;
	popupRect.bottom	= myPopup->popupsize.cy;

	Box(dc, popupRect, SMOOTH::Setup::BackgroundColor, FILLED);
	Frame(dc, popupRect, FRAME_UP);

	for (SMOOTHInt i = 0; i < myPopup->nOfEntries; i++)
	{
		entry = myPopup->entries.GetNthEntry(i);

		if (entry == NIL) continue;

		if (entry->type == SM_SEPARATOR)
		{
			p1.x = popupRect.left + 2;
			p2.x = popupRect.right - 2;
			p1.y = popupRect.top + currentYPos + 1;
			p2.y = popupRect.top + currentYPos + 1;

			HBar(dc, p1, p2);

			currentYPos = currentYPos + 5;
		}
		else
		{
			textRect.left	= popupRect.left + currentXPos;
			textRect.right	= popupRect.right;
			textRect.top	= popupRect.top + currentYPos;
			textRect.bottom	= textRect.top + METRIC_POPUPENTRYSIZE;

			::SetText(dc, entry->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

			if (entry->popup != NIL)
			{
				p1.x = popupRect.right - 11;
				p2.x = p1.x;
				p1.y = popupRect.top + currentYPos + METRIC_POPUPARROWOFFSETY;
				p2.y = p1.y + 9;

				for (int x = 0; x < 4; x++)
				{
					p1.x++;
					p2.x++;
					p1.y++;
					p2.y--;
					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
				}
			}

			if (entry->bVar != NIL)
			{
				if (*(entry->bVar) == SMOOTH::True)
				{
					frame.left = textRect.left - 15;
					frame.top = textRect.top + 2;
					frame.right = frame.left + 10;
					frame.bottom = frame.top + 10;

					p1.x = frame.left + 3;
					p1.y = frame.top + 3;
					p2.x = frame.right;
					p2.y = frame.bottom;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 4;
					p1.y = frame.top + 3;
					p2.x = frame.right;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 3;
					p1.y = frame.top + 4;
					p2.x = frame.right - 1;
					p2.y = frame.bottom;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 1;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 1;
					p1.y = frame.top + 4;
					p2.x = frame.left + 3;
					p2.y = frame.bottom;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 2;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.left + 3;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 2;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.left + 2;
					p1.y = frame.top + 3;
					p2.x = frame.right - 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 3;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 2;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
				}
			}

			if (entry->iVar != NIL)
			{
				p1.x = textRect.left - 12;
				p1.y = textRect.top + 12;
				p2.x = p1.x + 5;
				p2.y = p1.y;

				if (*(entry->iVar) == entry->iCode)
				{
					p1.x++;
					p1.y -= 7;
					p2.x--;
					p2.y -= 7;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

					p1.x--;
					p2.x++;

					for (int i = 0; i < 3; i++)
					{
						p1.y++;
						p2.y++;

						Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
					}

					PaintPixel(dc, SMOOTHPoint(p2.x, p2.y - 1), SMOOTH::Setup::DividerDarkColor);
					PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
					PaintPixel(dc, SMOOTHPoint(p2.x - 1, p2.y), SMOOTH::Setup::DividerDarkColor);
					PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);
				}
			}

			currentYPos = currentYPos + METRIC_POPUPENTRYSIZE;
		}
	}

	FreeContext(wnd, dc);

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHPopupView::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHRect	 popupRect;
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHInt	 i;
	HDC		 dc;
	SMOOTHRect	*entryRect = new SMOOTHRect [myPopup->nOfEntries];
	SMOOTHRect	 frame;
	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	SMOOTHInt	 currentX = 3;
	SMOOTHInt	 currentY = 3;
	SMOOTHInt	 maxX = myPopup->popupsize.cx - 3;

	SMOOTHString	 newStatus;
	SMOOTHBool	 updateStatus = SMOOTH::False;
	SMOOTHBool	 setOldStatus = SMOOTH::False;

	popupRect.left		= 0;
	popupRect.top		= 0;
	popupRect.right		= myPopup->popupsize.cx;
	popupRect.bottom	= myPopup->popupsize.cy;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (myPopup->nextPopup != NIL)
			{
				myPopup->nextPopup->Hide();

				SMOOTH::DeleteObject(myPopup->nextPopup);

				myPopup->nextPopup = NIL;
			}

			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				SMOOTHMenu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= objectProperties->pos.x + currentX;
					entryRect[i].right	= objectProperties->pos.x + maxX;
					entryRect[i].top	= objectProperties->pos.y + currentY;
					entryRect[i].bottom	= objectProperties->pos.y + currentY + METRIC_POPUPENTRYSIZE - 2;

					currentY = currentY + METRIC_POPUPENTRYSIZE;
				}
				else
				{
					currentY = currentY + 5;
				}

				if (entry->checked && (entry->popup != NIL))
				{
					myPopup->nextPopup = new SMOOTHPopupMenu();

					myPopup->nextPopup->prevPopup = myPopup;
					myPopup->nextPopup->MenuToPopup(entry->popup);
					myPopup->nextPopup->SetContainer(myContainer);
					myPopup->nextPopup->GetSize();
					myPopup->nextPopup->GetObjectProperties()->pos.x = entryRect[i].right - 1;
					myPopup->nextPopup->GetObjectProperties()->pos.y = entryRect[i].top - 3;

					if (myPopup->nextPopup->GetObjectProperties()->pos.x + myPopup->nextPopup->popupsize.cx >= LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x) myPopup->nextPopup->GetObjectProperties()->pos.x = entryRect[i].left + 1 - myPopup->nextPopup->popupsize.cx;
					if (myPopup->nextPopup->GetObjectProperties()->pos.y + myPopup->nextPopup->popupsize.cy >= LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y) myPopup->nextPopup->GetObjectProperties()->pos.y = LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y - myPopup->popupsize.cy - 1;

					wnd->RegisterObject(myPopup->nextPopup);

					retVal = SMOOTH::Break;
				}
			}

			break;
		case SM_LBUTTONUP:
			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				SMOOTHMenu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->checked && (entry->proc != NIL && entry->bVar == NIL && entry->iVar == NIL))
				{
					entry->checked = SMOOTH::False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					SMOOTHPopupMenu::status = POPUP_PENDING;

					SMOOTHPopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					SMOOTHPopupMenu::status = POPUP_FINISHED;

					SMOOTHProcCall(entry->proc, entry->procParam);

					retVal = SMOOTH::Break;

					break;
				}

				if (entry->checked && (entry->bVar != NIL))
				{
					SMOOTHBool	 valueChanged = SMOOTH::False;

					entry->checked = SMOOTH::False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					SMOOTHPopupMenu::status = POPUP_PENDING;

					SMOOTHPopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					SMOOTHPopupMenu::status = POPUP_FINISHED;

					if (*(entry->bVar) == SMOOTH::False)	{ *(entry->bVar) = SMOOTH::True; valueChanged = SMOOTH::True; }
					else					{ *(entry->bVar) = SMOOTH::False; valueChanged = SMOOTH::True; }

					if (valueChanged)
					{
						for (int j = 0; j < SMOOTHObject::objectCount; j++)
						{
							SMOOTHObject	*object = mainObjectManager->RequestObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == OBJ_CHECKBOX) object->Process(SM_CHECKCHECKBOXES, 0, 0);
							}
						}

						SMOOTHProcCall(entry->proc, entry->procParam);
					}

					retVal = SMOOTH::Break;

					break;
				}

				if (entry->checked && (entry->iVar != NIL))
				{
					SMOOTHBool	 valueChanged = SMOOTH::False;

					entry->checked = SMOOTH::False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					SMOOTHPopupMenu::status = POPUP_PENDING;

					SMOOTHPopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					SMOOTHPopupMenu::status = POPUP_FINISHED;

					if (*(entry->iVar) != entry->iCode) { *(entry->iVar) = entry->iCode; valueChanged = SMOOTH::True; }

					if (valueChanged)
					{
						for (int j = 0; j < SMOOTHObject::objectCount; j++)
						{
							SMOOTHObject	*object = mainObjectManager->RequestObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == OBJ_OPTIONBOX) object->Process(SM_CHECKOPTIONBOXES, 0, 0);
							}
						}

						SMOOTHProcCall(entry->proc, entry->procParam);
					}

					retVal = SMOOTH::Break;

					break;
				}
			}

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				SMOOTHMenu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + METRIC_POPUPENTRYSIZE - 2;

					currentY = currentY + METRIC_POPUPENTRYSIZE;

					if (!IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && entry->checked)
					{
						entry->checked = SMOOTH::False;

						if (entry->description != NIL) setOldStatus = SMOOTH::True;

						entryRect[i].right++;
						entryRect[i].bottom++;

						Box(dc, entryRect[i], SMOOTH::Setup::BackgroundColor, FILLED);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (SMOOTHInt x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == SMOOTH::True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->iVar != NIL)
						{
							p1.x = entryRect[i].left - 12;
							p1.y = entryRect[i].top + 12;
							p2.x = p1.x + 5;
							p2.y = p1.y;

							if (*(entry->iVar) == entry->iCode)
							{
								p1.x++;
								p1.y -= 7;
								p2.x--;
								p2.y -= 7;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (SMOOTHInt j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, SMOOTHPoint(p2.x, p2.y - 1), SMOOTH::Setup::DividerDarkColor);
								PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
								PaintPixel(dc, SMOOTHPoint(p2.x - 1, p2.y), SMOOTH::Setup::DividerDarkColor);
								PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);
							}
						}
					}
				}
				else
				{
					currentY = currentY + 5;
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				SMOOTHMenu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + METRIC_POPUPENTRYSIZE - 2;

					currentY = currentY + METRIC_POPUPENTRYSIZE;

					if (IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && !entry->checked)
					{
						entry->checked = SMOOTH::True;

						if (entry->description != NIL)
						{
							newStatus = entry->description;

							updateStatus = SMOOTH::True;
						}

						entryRect[i].right++;
						entryRect[i].bottom++;

						Gradient(dc, entryRect[i], SMOOTH::Setup::GradientStartColor, SMOOTH::Setup::GradientEndColor, GRADIENT_LR);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GradientTextColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (SMOOTHInt x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == SMOOTH::True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);
							}
						}

						if (entry->iVar != NIL)
						{
							p1.x = entryRect[i].left - 12;
							p1.y = entryRect[i].top + 12;
							p2.x = p1.x + 5;
							p2.y = p1.y;

							if (*(entry->iVar) == entry->iCode)
							{
								p1.x++;
								p1.y -= 7;
								p2.x--;
								p2.y -= 7;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (SMOOTHInt j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, SMOOTHPoint(p2.x, p2.y - 1), SMOOTH::Setup::InactiveGradientTextColor);
								PaintPixel(dc, p2, SMOOTH::Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::GradientTextColor, PS_SOLID, 1);
								PaintPixel(dc, SMOOTHPoint(p2.x - 1, p2.y), SMOOTH::Setup::InactiveGradientTextColor);
								PaintPixel(dc, p2, SMOOTH::Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::InactiveGradientTextColor, PS_SOLID, 1);
							}
						}
					}
					else if (!IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && entry->checked)
					{
						entry->checked = SMOOTH::False;

						if (entry->description != NIL) setOldStatus = SMOOTH::True;

						entryRect[i].right++;
						entryRect[i].bottom++;

						Box(dc, entryRect[i], SMOOTH::Setup::BackgroundColor, FILLED);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (SMOOTHInt x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == SMOOTH::True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->iVar != NIL)
						{
							p1.x = entryRect[i].left - 12;
							p1.y = entryRect[i].top + 12;
							p2.x = p1.x + 5;
							p2.y = p1.y;

							if (*(entry->iVar) == entry->iCode)
							{
								p1.x++;
								p1.y -= 7;
								p2.x--;
								p2.y -= 7;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (SMOOTHInt j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, SMOOTHPoint(p2.x, p2.y - 1), SMOOTH::Setup::DividerDarkColor);
								PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
								PaintPixel(dc, SMOOTHPoint(p2.x - 1, p2.y), SMOOTH::Setup::DividerDarkColor);
								PaintPixel(dc, p2, SMOOTH::Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);
							}
						}
					}
				}
				else
				{
					currentY = currentY + 5;
				}
			}

			FreeContext(wnd, dc);

			break;
	}

	if (updateStatus || setOldStatus)
	{
		SMOOTHWindow	*rWnd;

		rWnd = (SMOOTHWindow *) myPopup->GetContainer()->GetContainerObject();

		for (;;)
		{
			if (rWnd->GetContainer()->GetContainerObject()->GetObjectType() != OBJ_APPLICATION)	rWnd = (SMOOTHWindow *) rWnd->GetContainer()->GetContainerObject();
			else											break;
		}

		if (setOldStatus && !updateStatus) rWnd->SetStatusText(backupStatusText);

		if (updateStatus)
		{
			if (!setOldStatus) backupStatusText = rWnd->GetStatusText();

			rWnd->SetStatusText(newStatus);
		}
	}

	if (myPopup->nOfEntries > 0) delete [] entryRect;

	LeaveProtectedRegion();

	return retVal;
}

#endif
