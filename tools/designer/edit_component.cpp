 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <time.h>
#include "edit_component.h"

using namespace smooth::GUI::Dialogs;

Designer_EditComponent::Designer_EditComponent(Designer *des, String name)
{
	designer = des;

	status.dlg = this;
	status.currdlgname = name;
	status.mousex = 0;
	status.mousey = 0;
	status.wndposx = 150;
	status.wndposy = 150;
	status.wndsizex = 400;
	status.wndsizey = 400;
	status.event = STATUS_EVENT_REPORT_NAME;

	designer->ReportStatus(status);

	wnd	= new Window(name, Point(150, 150), Size(400, 400));
	title	= new Titlebar();

	objects.AddEntry(wnd);
	objects.AddEntry(title);

	RegisterObject(wnd);

	wnd->RegisterObject(title);

	wnd->onEvent.Connect(&Designer_EditComponent::EventProc, this);
	wnd->doQuit.Connect(&Designer_EditComponent::ExitProc, this);
}

Designer_EditComponent::~Designer_EditComponent()
{
	if (wnd->IsVisible()) wnd->Close();

	DeleteObject(title);
	DeleteObject(wnd);
}

Void Designer_EditComponent::EventProc(Int message, Int wParam, Int lParam)
{
	WINDOWPOS	*wndpos;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if ((status.mousex != wnd->MouseX() || status.mousey != wnd->MouseY()) && !(status.event == STATUS_EVENT_REPORT_WINDOWPOSITION && (clock() - status.ticks) < 100))
			{
				status.mousex = wnd->MouseX();
				status.mousey = wnd->MouseY();

				status.event = STATUS_EVENT_REPORT_MOUSEPOSITION;

				designer->ReportStatus(status);
			}
			break;
		case WM_WINDOWPOSCHANGED:
			wndpos = (LPWINDOWPOS) lParam;

			if (status.wndposx != wndpos->x || status.wndposy != wndpos->y)
			{
				status.wndposx	= wndpos->x;
				status.wndposy	= wndpos->y;

				status.event = STATUS_EVENT_REPORT_WINDOWPOSITION;

				status.ticks = clock();
			}
			else if (status.wndsizex != wndpos->cx || status.wndsizey != wndpos->cy)
			{
				status.wndsizex	= wndpos->cx;
				status.wndsizey	= wndpos->cy;

				status.event = STATUS_EVENT_REPORT_WINDOWSIZE;
			}

			designer->ReportStatus(status);
			break;
		case SM_LBUTTONDOWN:
		case SM_RBUTTONDOWN:
		case WM_ACTIVATE:
			status.currdlgname = wnd->GetText();
			status.event = STATUS_EVENT_REPORT_NAME;

			designer->ReportStatus(status);

			break;
	}
}

Bool Designer_EditComponent::ExitProc()
{
	status.event = STATUS_EVENT_REPORT_QUIT;

	designer->ReportStatus(status);

	return True;
}

Void Designer_EditComponent::ShowDialog()
{
	wnd->Show();
}

String Designer_EditComponent::GetName()
{
	return wnd->GetText();
}

Window *Designer_EditComponent::GetWindow()
{
	return wnd;
}

Widget *Designer_EditComponent::AddObject(Int objid)
{
	Widget	*newobj;
	Widget	*registrar;
	Point	 pos;
	Size	 size;

	switch (objid)
	{
		case SMOOTH_BUTTON:
			registrar = wnd;

			pos.x = 100;
			pos.y = 100;
			size.cx = 0;
			size.cy = 0;

			newobj = new Button("Button", NIL, pos, size);
			break;
		case SMOOTH_MENUBAR:
			registrar = wnd;
			newobj = new Menubar();
			break;
		default:
			QuickMessage("Unknown object ID!", "Error", MB_OK, IDI_HAND);
			return NIL;
	}

	registrar->RegisterObject(newobj);

	objects.AddEntry(newobj);

	return newobj;
}

Widget *Designer_EditComponent::GetFirstObject(Int objtype)
{
	for (Int i = 0; i < objects.GetNOfEntries(); i++)
	{
		if (objects.GetNthEntry(i)->GetObjectType() == objtype) return objects.GetNthEntry(i);
	}

	return NIL;
}
