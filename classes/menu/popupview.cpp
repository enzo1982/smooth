 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

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

S::Int	 S::OBJ_POPUPVIEW = S::Object::RequestObjectID();

S::GUI::PopupView::PopupView(PopupMenu *popupMenu)
{
	type				= OBJ_POPUPVIEW;
	objectProperties->orientation	= OR_FREE;
	myPopup				= popupMenu;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::GUI::PopupView::~PopupView()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::PopupView::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Menu::Entry	*entry;
	Rect		 popupRect;
	HDC		 dc = GetContext(wnd);
	Point		 p1;
	Point		 p2;
	Rect		 frame;
	Rect		 textRect;
	Int		 currentXPos = 20;
	Int		 currentYPos = 3;

	myPopup->GetSize();

	popupRect.left		= 0;
	popupRect.top		= 0;
	popupRect.right		= myPopup->popupsize.cx;
	popupRect.bottom	= myPopup->popupsize.cy;

	Box(dc, popupRect, Setup::BackgroundColor, FILLED);
	Frame(dc, popupRect, FRAME_UP);

	for (Int i = 0; i < myPopup->nOfEntries; i++)
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
					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
				}
			}

			if (entry->bVar != NIL)
			{
				if (*(entry->bVar) == True)
				{
					frame.left = textRect.left - 15;
					frame.top = textRect.top + 2;
					frame.right = frame.left + 10;
					frame.bottom = frame.top + 10;

					p1.x = frame.left + 3;
					p1.y = frame.top + 3;
					p2.x = frame.right;
					p2.y = frame.bottom;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 4;
					p1.y = frame.top + 3;
					p2.x = frame.right;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 3;
					p1.y = frame.top + 4;
					p2.x = frame.right - 1;
					p2.y = frame.bottom;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 1;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 1;
					p1.y = frame.top + 4;
					p2.x = frame.left + 3;
					p2.y = frame.bottom;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

					p1.x = frame.left + 2;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.left + 3;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 2;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.left + 2;
					p1.y = frame.top + 3;
					p2.x = frame.right - 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x = frame.right - 3;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 2;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
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

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

					p1.x--;
					p2.x++;

					for (int i = 0; i < 3; i++)
					{
						p1.y++;
						p2.y++;

						Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
					}

					PaintPixel(dc, Point(p2.x, p2.y - 1), Setup::DividerDarkColor);
					PaintPixel(dc, p2, Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
					PaintPixel(dc, Point(p2.x - 1, p2.y), Setup::DividerDarkColor);
					PaintPixel(dc, p2, Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);
				}
			}

			currentYPos = currentYPos + METRIC_POPUPENTRYSIZE;
		}
	}

	FreeContext(wnd, dc);

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupView::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Rect	 popupRect;
	Int	 retVal = Success;
	Int	 i;
	HDC	 dc;
	Rect	*entryRect = new Rect [myPopup->nOfEntries];
	Rect	 frame;
	Point	 p1;
	Point	 p2;
	Int	 currentX = 3;
	Int	 currentY = 3;
	Int	 maxX = myPopup->popupsize.cx - 3;

	String	 newStatus;
	Bool	 updateStatus = False;
	Bool	 setOldStatus = False;

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

				DeleteObject(myPopup->nextPopup);

				myPopup->nextPopup = NIL;
			}

			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				Menu::Entry	*entry = myPopup->entries.GetNthEntry(i);

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
					myPopup->nextPopup = new PopupMenu();

					myPopup->nextPopup->prevPopup = myPopup;
					myPopup->nextPopup->MenuToPopup(entry->popup);
					myPopup->nextPopup->SetContainer(myContainer);
					myPopup->nextPopup->GetSize();
					myPopup->nextPopup->GetObjectProperties()->pos.x = entryRect[i].right - 1;
					myPopup->nextPopup->GetObjectProperties()->pos.y = entryRect[i].top - 3;

					if (myPopup->nextPopup->GetObjectProperties()->pos.x + myPopup->nextPopup->popupsize.cx >= LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x) myPopup->nextPopup->GetObjectProperties()->pos.x = entryRect[i].left + 1 - myPopup->nextPopup->popupsize.cx;
					if (myPopup->nextPopup->GetObjectProperties()->pos.y + myPopup->nextPopup->popupsize.cy >= LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y) myPopup->nextPopup->GetObjectProperties()->pos.y = LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y - myPopup->popupsize.cy - 1;

					wnd->RegisterObject(myPopup->nextPopup);

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONUP:
			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				Menu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->checked && (entry->onClick.GetNOfConnectedSlots() != 0 && entry->bVar == NIL && entry->iVar == NIL))
				{
					entry->checked = False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					PopupMenu::status = POPUP_PENDING;

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					PopupMenu::status = POPUP_FINISHED;

					entry->onClick.Emit();

					retVal = Break;

					break;
				}

				if (entry->checked && (entry->bVar != NIL))
				{
					Bool	 valueChanged = False;

					entry->checked = False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					PopupMenu::status = POPUP_PENDING;

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					PopupMenu::status = POPUP_FINISHED;

					if (*(entry->bVar) == False)	{ *(entry->bVar) = True; valueChanged = True; }
					else				{ *(entry->bVar) = False; valueChanged = True; }

					if (valueChanged)
					{
						for (int j = 0; j < Object::objectCount; j++)
						{
							Object	*object = mainObjectManager->RequestObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == OBJ_CHECKBOX) object->Process(SM_CHECKCHECKBOXES, 0, 0);
							}
						}

						entry->onClick.Emit();
					}

					retVal = Break;

					break;
				}

				if (entry->checked && (entry->iVar != NIL))
				{
					Bool	 valueChanged = False;

					entry->checked = False;

					if (entry->description != NIL) wnd->SetStatusText(backupStatusText);

					PopupMenu::status = POPUP_PENDING;

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					PopupMenu::status = POPUP_FINISHED;

					if (*(entry->iVar) != entry->iCode) { *(entry->iVar) = entry->iCode; valueChanged = True; }

					if (valueChanged)
					{
						for (int j = 0; j < Object::objectCount; j++)
						{
							Object	*object = mainObjectManager->RequestObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == OBJ_OPTIONBOX) object->Process(SM_CHECKOPTIONBOXES, 0, 0);
							}
						}

						entry->onClick.Emit();
					}

					retVal = Break;

					break;
				}
			}

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			for (i = 0; i < myPopup->nOfEntries; i++)
			{
				Menu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + METRIC_POPUPENTRYSIZE - 2;

					currentY = currentY + METRIC_POPUPENTRYSIZE;

					if (!IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && entry->checked)
					{
						entry->checked = False;

						if (entry->description != NIL) setOldStatus = True;

						entryRect[i].right++;
						entryRect[i].bottom++;

						Box(dc, entryRect[i], Setup::BackgroundColor, FILLED);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (Int x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
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

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (Int j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, Point(p2.x, p2.y - 1), Setup::DividerDarkColor);
								PaintPixel(dc, p2, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
								PaintPixel(dc, Point(p2.x - 1, p2.y), Setup::DividerDarkColor);
								PaintPixel(dc, p2, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);
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
				Menu::Entry	*entry = myPopup->entries.GetNthEntry(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + METRIC_POPUPENTRYSIZE - 2;

					currentY = currentY + METRIC_POPUPENTRYSIZE;

					if (IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && !entry->checked)
					{
						entry->checked = True;

						if (entry->description != NIL)
						{
							newStatus = entry->description;

							updateStatus = True;
						}

						entryRect[i].right++;
						entryRect[i].bottom++;

						Gradient(dc, entryRect[i], Setup::GradientStartColor, Setup::GradientEndColor, GRADIENT_LR);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, Setup::GradientTextColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (Int x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);
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

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (Int j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, Point(p2.x, p2.y - 1), Setup::InactiveGradientTextColor);
								PaintPixel(dc, p2, Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::GradientTextColor, PS_SOLID, 1);
								PaintPixel(dc, Point(p2.x - 1, p2.y), Setup::InactiveGradientTextColor);
								PaintPixel(dc, p2, Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::InactiveGradientTextColor, PS_SOLID, 1);
							}
						}
					}
					else if (!IsMouseOn(wnd->hwnd, entryRect[i], WINDOW) && entry->checked)
					{
						entry->checked = False;

						if (entry->description != NIL) setOldStatus = True;

						entryRect[i].right++;
						entryRect[i].bottom++;

						Box(dc, entryRect[i], Setup::BackgroundColor, FILLED);

						entryRect[i].left = entryRect[i].left + 17;

						::SetText(dc, entry->text, entryRect[i], objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + METRIC_POPUPARROWOFFSETY;
							p2.y = p1.y + 9;

							for (Int x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
							}
						}

						if (entry->bVar != NIL)
						{
							if (*(entry->bVar) == True)
							{
								frame.left = entryRect[i].left - 15;
								frame.top = entryRect[i].top + 2;
								frame.right = frame.left + 10;
								frame.bottom = frame.top + 10;

								p1.x = frame.left + 3;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
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

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);

								p1.x--;
								p2.x++;

								for (Int j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
								}

								PaintPixel(dc, Point(p2.x, p2.y - 1), Setup::DividerDarkColor);
								PaintPixel(dc, p2, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::TextColor, PS_SOLID, 1);
								PaintPixel(dc, Point(p2.x - 1, p2.y), Setup::DividerDarkColor);
								PaintPixel(dc, p2, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								Line(dc, p1, p2, Setup::DividerDarkColor, PS_SOLID, 1);
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
		Window	*rWnd = (Window *) myPopup->GetContainer()->GetContainerObject();

		for (;;)
		{
			if (rWnd->GetContainer()->GetContainerObject()->GetObjectType() != OBJ_APPLICATION)	rWnd = (Window *) rWnd->GetContainer()->GetContainerObject();
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
