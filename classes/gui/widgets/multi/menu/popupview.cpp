 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupview.h>
#include <smooth/definitions.h>
#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/system/timer.h>
#include <smooth/gui/application/application.h>

const S::Int	 S::GUI::PopupView::classID = S::Object::RequestClassID();

S::GUI::PopupView::PopupView(PopupMenu *popupMenu, Menu *iRealMenu)
{
	type		= classID;
	orientation	= OR_FREE;
	myPopup		= popupMenu;
	realMenu	= iRealMenu;
	openTimer	= NIL;
	closeTimer	= NIL;

	possibleContainers.AddEntry(Window::classID);
}

S::GUI::PopupView::~PopupView()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::PopupView::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface		*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	MenuEntry	*entry;
	Rect		 popupRect;
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

	surface->Box(popupRect, Setup::BackgroundColor, FILLED);
	surface->Frame(popupRect, FRAME_UP);

	for (Int i = 0; i < realMenu->GetNOfObjects(); i++)
	{
		entry = (MenuEntry *) realMenu->GetNthObject(i);

		if (entry == NIL) continue;

		if (entry->type == SM_SEPARATOR)
		{
			p1.x = popupRect.left + 2;
			p2.x = popupRect.right - 2;
			p1.y = popupRect.top + currentYPos + 1;
			p2.y = popupRect.top + currentYPos + 1;

			surface->Bar(p1, p2, OR_HORZ);

			currentYPos = currentYPos + 5;
		}
		else
		{
			textRect.left	= popupRect.left + currentXPos;
			textRect.right	= popupRect.right;
			textRect.top	= popupRect.top + currentYPos;
			textRect.bottom	= textRect.top + 16;

			surface->SetText(entry->GetText(), textRect, font);

			if (entry->popup != NIL)
			{
				p1.x = popupRect.right - 11;
				p2.x = p1.x;
				p1.y = popupRect.top + currentYPos + 3;
				p2.y = p1.y + 9;

				for (int x = 0; x < 4; x++)
				{
					p1.x++;
					p2.x++;
					p1.y++;
					p2.y--;
					surface->Line(p1, p2, Setup::TextColor);
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

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.left + 4;
					p1.y = frame.top + 3;
					p2.x = frame.right;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.left + 3;
					p1.y = frame.top + 4;
					p2.x = frame.right - 1;
					p2.y = frame.bottom;

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.right - 1;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom;

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.right - 1;
					p1.y = frame.top + 4;
					p2.x = frame.left + 3;
					p2.y = frame.bottom;

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::DividerDarkColor);

					p1.x = frame.left + 2;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::TextColor);

					p1.x = frame.left + 3;
					p1.y = frame.top + 2;
					p2.x = frame.right - 1;
					p2.y = frame.bottom - 2;

					surface->Line(p1, p2, Setup::TextColor);

					p1.x = frame.left + 2;
					p1.y = frame.top + 3;
					p2.x = frame.right - 2;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::TextColor);

					p1.x = frame.right - 2;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::TextColor);

					p1.x = frame.right - 2;
					p1.y = frame.top + 3;
					p2.x = frame.left + 2;
					p2.y = frame.bottom - 1;

					surface->Line(p1, p2, Setup::TextColor);

					p1.x = frame.right - 3;
					p1.y = frame.top + 2;
					p2.x = frame.left + 1;
					p2.y = frame.bottom - 2;

					surface->Line(p1, p2, Setup::TextColor);
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

					surface->Line(p1, p2, Setup::TextColor);

					p1.x--;
					p2.x++;

					for (int i = 0; i < 3; i++)
					{
						p1.y++;
						p2.y++;

						surface->Line(p1, p2, Setup::TextColor);
					}

					surface->SetPixel(p2.x, p2.y - 1, Setup::DividerDarkColor);
					surface->SetPixel(p2.x, p2.y, Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					surface->Line(p1, p2, Setup::TextColor);
					surface->SetPixel(p2.x - 1, p2.y, Setup::DividerDarkColor);
					surface->SetPixel(p2.x, p2.y, Setup::DividerDarkColor);

					p1.x++;
					p1.y++;
					p2.y++;

					surface->Line(p1, p2, Setup::DividerDarkColor);
				}
			}

			currentYPos = currentYPos + 16;
		}
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupView::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	PopupMenu *iPopup = myPopup;

	iPopup->EnterProtectedRegion();

	while (iPopup->prevPopup != NIL) { iPopup = iPopup->prevPopup; iPopup->EnterProtectedRegion(); }

	Rect	 popupRect;
	Int	 retVal = Success;
	Int	 i;
	Rect	*entryRect = new Rect [realMenu->GetNOfObjects()];
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

	Window	*rWnd = myPopup->GetContainer()->GetContainerWindow();

	while (True)
	{
		if (rWnd->GetContainer()->GetObjectType() != Application::classID)	rWnd = rWnd->GetContainer()->GetContainerWindow();
		else									break;
	}

	switch (message)
	{
		case SM_OPENPOPUP:
			Process(SM_CLOSEPOPUP, 0, 0);

			for (i = 0; i < realMenu->GetNOfObjects(); i++)
			{
				MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= pos.x + currentX;
					entryRect[i].right	= pos.x + maxX;
					entryRect[i].top	= pos.y + currentY;
					entryRect[i].bottom	= pos.y + currentY + 16 - 2;

					currentY = currentY + 16;
				}
				else
				{
					currentY = currentY + 5;
				}

				if (entry->checked && (entry->popup != NIL))
				{
					myPopup->nextPopup = new PopupMenu(entry->popup);

					myPopup->nextPopup->prevPopup = myPopup;
					myPopup->nextPopup->SetContainer(container);
					myPopup->nextPopup->GetSize();
					myPopup->nextPopup->pos.x = entryRect[i].right - 1;
					myPopup->nextPopup->pos.y = entryRect[i].top - 3;

					if (myPopup->nextPopup->pos.x + myPopup->nextPopup->popupsize.cx >= LiSAGetDisplaySizeX() - wnd->pos.x) myPopup->nextPopup->pos.x = entryRect[i].left + 1 - myPopup->nextPopup->popupsize.cx;
					if (myPopup->nextPopup->pos.y + myPopup->nextPopup->popupsize.cy >= LiSAGetDisplaySizeY() - wnd->pos.y) myPopup->nextPopup->pos.y = LiSAGetDisplaySizeY() - wnd->pos.y - myPopup->popupsize.cy - 1;

					wnd->RegisterObject(myPopup->nextPopup);

					retVal = Break;
				}
			}

			break;
		case SM_CLOSEPOPUP:
			if (myPopup->nextPopup != NIL)
			{
				if (closeTimer != NIL)
				{
					DeleteObject(closeTimer);

					closeTimer = NIL;
				}

				DeleteObject(myPopup->nextPopup);

				myPopup->nextPopup = NIL;
			}
			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (myPopup->nextPopup != NIL)
			{
				Process(SM_CLOSEPOPUP, 0, 0);

				if (openTimer != NIL) Process(SM_OPENPOPUP, 0, 0);

				retVal = Break;
			}
			else
			{
				if (openTimer != NIL) DeleteObject(openTimer);

				openTimer = NIL;

				Process(SM_OPENPOPUP, 0, 0);
			}

			break;
		case SM_LBUTTONUP:
			for (i = 0; i < realMenu->GetNOfObjects(); i++)
			{
				MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= pos.x + currentX;
					entryRect[i].right	= pos.x + maxX;
					entryRect[i].top	= pos.y + currentY;
					entryRect[i].bottom	= pos.y + currentY + 16 - 2;

					currentY = currentY + 16;
				}
				else
				{
					currentY = currentY + 5;
				}

				if (entry->checked && wnd->IsMouseOn(entryRect[i]) && (entry->popup == NIL && entry->bVar == NIL && entry->iVar == NIL))
				{
					entry->checked = False;

					if (entry->description != NIL) rWnd->SetStatusText(backupStatusText);

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					popup->onClick.Emit(0, 0);
					entry->onClick.Emit();

					retVal = Break;

					break;
				}

				if (entry->checked && wnd->IsMouseOn(entryRect[i]) && (entry->bVar != NIL))
				{
					Bool	 valueChanged = False;

					entry->checked = False;

					if (entry->description != NIL) rWnd->SetStatusText(backupStatusText);

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					if (*(entry->bVar) == False)	{ *(entry->bVar) = True; valueChanged = True; }
					else				{ *(entry->bVar) = False; valueChanged = True; }

					if (valueChanged)
					{
						for (int j = 0; j < Object::GetNOfObjects(); j++)
						{
							Object	*object = Object::GetNthObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == CheckBox::classID) ((CheckBox *) object)->Process(SM_CHECKCHECKBOXES, 0, 0);
							}
						}

						popup->onClick.Emit(0, 0);
						entry->onClick.Emit();
					}

					retVal = Break;

					break;
				}

				if (entry->checked && wnd->IsMouseOn(entryRect[i]) && (entry->iVar != NIL))
				{
					Bool	 valueChanged = False;

					entry->checked = False;

					if (entry->description != NIL) rWnd->SetStatusText(backupStatusText);

					PopupMenu *popup = myPopup;

					while (popup->prevPopup != NIL) popup = popup->prevPopup;

					popup->toolwnd->Hide();

					if (*(entry->iVar) != entry->iCode) { *(entry->iVar) = entry->iCode; valueChanged = True; }

					if (valueChanged)
					{
						for (int j = 0; j < Object::GetNOfObjects(); j++)
						{
							Object	*object = Object::GetNthObject(j);

							if (object != NIL)
							{
								if (object->GetObjectType() == OptionBox::classID) ((OptionBox *) object)->Process(SM_CHECKOPTIONBOXES, 0, 0);
							}
						}

						popup->onClick.Emit(0, 0);
						entry->onClick.Emit();
					}

					retVal = Break;

					break;
				}
			}

			break;
		case SM_MOUSEMOVE:
			for (i = 0; i < realMenu->GetNOfObjects(); i++)
			{
				MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + 16 - 2;

					currentY = currentY + 16;

					if (!wnd->IsMouseOn(entryRect[i]) && entry->checked)
					{
						if (entry->description != NIL) setOldStatus = True;

						if (entry->popup != NIL)
						{
							if (openTimer != NIL) DeleteObject(openTimer);

							openTimer = NIL;

							if (wnd->IsMouseOn(popupRect) || myPopup->nextPopup == NIL)
							{
								entry->checked = False;

								entryRect[i].right++;
								entryRect[i].bottom++;

								surface->Box(entryRect[i], Setup::BackgroundColor, FILLED);

								entryRect[i].left = entryRect[i].left + 17;

								surface->SetText(entry->GetText(), entryRect[i], font);

								p1.x = entryRect[i].right - 9;
								p2.x = p1.x;
								p1.y = entryRect[i].top + 3;
								p2.y = p1.y + 9;

								for (Int x = 0; x < 4; x++)
								{
									p1.x++;
									p2.x++;
									p1.y++;
									p2.y--;

									surface->Line(p1, p2, Setup::TextColor);
								}

								if (closeTimer == NIL)
								{
									closeTimer = new System::Timer();
									closeTimer->onInterval.Connect(&PopupView::CloseProc, this);
									closeTimer->Start(100);
								}
							}
						}
						else
						{
							entry->checked = False;

							entryRect[i].right++;
							entryRect[i].bottom++;

							surface->Box(entryRect[i], Setup::BackgroundColor, FILLED);

							entryRect[i].left = entryRect[i].left + 17;

							surface->SetText(entry->GetText(), entryRect[i], font);
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

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::DividerDarkColor);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::TextColor);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								surface->Line(p1, p2, Setup::TextColor);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::TextColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::TextColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::TextColor);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								surface->Line(p1, p2, Setup::TextColor);
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

								surface->Line(p1, p2, Setup::TextColor);

								p1.x--;
								p2.x++;

								for (Int j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									surface->Line(p1, p2, Setup::TextColor);
								}

								surface->SetPixel(p2.x, p2.y - 1, Setup::DividerDarkColor);
								surface->SetPixel(p2.x, p2.y, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								surface->Line(p1, p2, Setup::TextColor);
								surface->SetPixel(p2.x - 1, p2.y, Setup::DividerDarkColor);
								surface->SetPixel(p2.x, p2.y, Setup::DividerDarkColor);

								p1.x++;
								p1.y++;
								p2.y++;

								surface->Line(p1, p2, Setup::DividerDarkColor);
							}
						}
					}
				}
				else
				{
					currentY = currentY + 5;
				}
			}

			currentY = 3;

			for (i = 0; i < realMenu->GetNOfObjects(); i++)
			{
				MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

				if (entry->type != SM_SEPARATOR)
				{
					entryRect[i].left	= currentX;
					entryRect[i].right	= maxX;
					entryRect[i].top	= currentY;
					entryRect[i].bottom	= currentY + 16 - 2;

					currentY = currentY + 16;

					if (wnd->IsMouseOn(entryRect[i]) && !entry->checked)
					{
						entry->checked = True;

						if (entry->description != NIL)
						{
							newStatus = entry->description;

							updateStatus = True;
						}

						entryRect[i].right++;
						entryRect[i].bottom++;

						Surface	*surface = container->GetDrawSurface();

						surface->Gradient(entryRect[i], Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);

						entryRect[i].left = entryRect[i].left + 17;

						Font	 nFont = font;

						nFont.SetColor(Setup::GradientTextColor);

						surface->SetText(entry->GetText(), entryRect[i], nFont);

						if (entry->popup != NIL)
						{
							p1.x = entryRect[i].right - 9;
							p2.x = p1.x;
							p1.y = entryRect[i].top + 3;
							p2.y = p1.y + 9;

							for (Int x = 0; x < 4; x++)
							{
								p1.x++;
								p2.x++;
								p1.y++;
								p2.y--;

								surface->Line(p1, p2, Setup::GradientTextColor);
							}

							if (openTimer != NIL) DeleteObject(openTimer);

							openTimer = new System::Timer();
							openTimer->onInterval.Connect(&PopupView::OpenProc, this);
							openTimer->Start(150);
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

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.left + 4;
								p1.y = frame.top + 3;
								p2.x = frame.right;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.left + 3;
								p1.y = frame.top + 4;
								p2.x = frame.right - 1;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.right - 1;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.right - 1;
								p1.y = frame.top + 4;
								p2.x = frame.left + 3;
								p2.y = frame.bottom;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);

								p1.x = frame.left + 2;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x = frame.left + 3;
								p1.y = frame.top + 2;
								p2.x = frame.right - 1;
								p2.y = frame.bottom - 2;

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x = frame.left + 2;
								p1.y = frame.top + 3;
								p2.x = frame.right - 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x = frame.right - 2;
								p1.y = frame.top + 3;
								p2.x = frame.left + 2;
								p2.y = frame.bottom - 1;

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x = frame.right - 3;
								p1.y = frame.top + 2;
								p2.x = frame.left + 1;
								p2.y = frame.bottom - 2;

								surface->Line(p1, p2, Setup::GradientTextColor);
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

								surface->Line(p1, p2, Setup::GradientTextColor);

								p1.x--;
								p2.x++;

								for (Int j = 0; j < 3; j++)
								{
									p1.y++;
									p2.y++;

									surface->Line(p1, p2, Setup::GradientTextColor);
								}

								surface->SetPixel(p2.x, p2.y - 1, Setup::InactiveGradientTextColor);
								surface->SetPixel(p2.x, p2.y, Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								surface->Line(p1, p2, Setup::GradientTextColor);
								surface->SetPixel(p2.x - 1, p2.y, Setup::InactiveGradientTextColor);
								surface->SetPixel(p2.x, p2.y, Setup::InactiveGradientTextColor);

								p1.x++;
								p1.y++;
								p2.y++;

								surface->Line(p1, p2, Setup::InactiveGradientTextColor);
							}
						}
					}
				}
				else
				{
					currentY = currentY + 5;
				}
			}

			break;
	}

	if (updateStatus || setOldStatus)
	{
		if (setOldStatus && !updateStatus) rWnd->SetStatusText(backupStatusText);

		if (updateStatus)
		{
			if (!setOldStatus) backupStatusText = rWnd->GetStatusText();

			rWnd->SetStatusText(newStatus);
		}
	}

	if (realMenu->GetNOfObjects() > 0) delete [] entryRect;

	iPopup = myPopup;

	iPopup->LeaveProtectedRegion();

	while (iPopup->prevPopup != NIL) { iPopup = iPopup->prevPopup; iPopup->LeaveProtectedRegion(); }

	LeaveProtectedRegion();

	return retVal;
}

S::Void S::GUI::PopupView::OpenProc()
{
	if (myPopup->nextPopup != NIL)
	{
		DeleteObject(openTimer);

		openTimer = NIL;
	}
	else
	{
		Process(SM_OPENPOPUP, 0, 0);
	}
}

S::Void S::GUI::PopupView::CloseProc()
{
	DeleteObject(closeTimer);

	closeTimer = NIL;

	if (!registered) return;

	Window	*wnd = container->GetContainerWindow();
	Rect	 popupRect;

	popupRect.left		= 0;
	popupRect.top		= 0;
	popupRect.right		= myPopup->popupsize.cx;
	popupRect.bottom	= myPopup->popupsize.cy;

	if (wnd->IsMouseOn(popupRect) && myPopup->nextPopup != NIL)
	{
		Process(SM_CLOSEPOPUP, 0, 0);
	}
}
