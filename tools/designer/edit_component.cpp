 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DESIGNER_EDITCOMPONENT_
#define __OBJSMOOTH_DESIGNER_EDITCOMPONENT_

#include <smoothx.h>
#include <time.h>
#include "edit_component.h"
#include "tools.h"

Designer_EditComponent::Designer_EditComponent(Designer *des, SMOOTHString name)
{
	designer = des;

	status.dlg = this;
	status.currdlgname = name;
	status.wndposx = 150;
	status.wndposy = 150;
	status.wndsizex = 400;
	status.wndsizey = 400;
	status.event = STATUS_EVENT_REPORT_NAME;

	designer->ReportStatus(status);

	wnd		= new SMOOTHWindow(name);
	title		= new SMOOTHTitlebar(true, true, true);

	objects.AddEntry(wnd);
	objects.AddEntry(title);

	RegisterObject(wnd);

	wnd->RegisterObject(title);

	wnd->SetMetrics(SMOOTHPoint(150, 150), SMOOTHSize(400, 400));
	wnd->SetMessageProc(SMOOTHMessageProc(Designer_EditComponent, this, MessageProc));
	wnd->SetKillProc(SMOOTHKillProc(Designer_EditComponent, this, KillProc));
}

Designer_EditComponent::~Designer_EditComponent()
{
	if (wnd->IsVisible()) SMOOTH::CloseWindow(wnd);

	wnd->UnregisterObject(title);

	UnregisterObject(wnd);

	delete title;
	delete wnd;
}

SMOOTHVoid Designer_EditComponent::MessageProc(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	WINDOWPOS	*wndpos;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if ((status.mousex != MouseX(wnd->hwnd, WINDOW) || status.mousey != MouseY(wnd->hwnd, WINDOW)) && !(status.event == STATUS_EVENT_REPORT_WINDOWPOSITION && (clock() - status.ticks) < 100))
			{
				status.mousex = MouseX(wnd->hwnd, WINDOW);
				status.mousey = MouseY(wnd->hwnd, WINDOW);

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

SMOOTHBool Designer_EditComponent::KillProc()
{
	status.event = STATUS_EVENT_REPORT_QUIT;

	designer->ReportStatus(status);

	return true;
}

SMOOTHVoid Designer_EditComponent::ShowDialog()
{
	wnd->Show();
}

SMOOTHString Designer_EditComponent::GetName()
{
	return wnd->GetText();
}

SMOOTHWindow *Designer_EditComponent::GetWindow()
{
	return wnd;
}

SMOOTHObject *Designer_EditComponent::AddObject(SMOOTHInt objid)
{
	SMOOTHObject	*newobj;
	SMOOTHContainer	*registrar;
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	switch (objid)
	{
		case SMOOTH_BUTTON:
			registrar = (SMOOTHContainer *) GetFirstObject(OBJ_LAYER);

			if (registrar == NIL) registrar = (SMOOTHContainer *) AddObject(SMOOTH_LAYER);

			pos.x = 100;
			pos.y = 100;
			size.cx = 0;
			size.cy = 0;

			newobj = new SMOOTHButton("Button", NIL, pos, size, NULLPROC);
			break;
		case SMOOTH_MENUBAR:
			registrar = wnd;
			newobj = new SMOOTHMenubar();
			break;
		case SMOOTH_LAYER:
			registrar = wnd;
			newobj = new SMOOTHLayer();
			break;
		default:
			SMOOTH::MessageBox("Unknown object ID!", "Error", MB_OK, IDI_HAND);
			return NIL;
	}

	registrar->RegisterObject(newobj);

	objects.AddEntry(newobj);

	return newobj;
}

SMOOTHObject *Designer_EditComponent::GetFirstObject(SMOOTHInt objtype)
{
	for (SMOOTHInt i = 0; i < objects.GetNOfEntries(); i++)
	{
		if (objects.GetNthEntry(i)->GetObjectType() == objtype) return objects.GetNthEntry(i);
	}

	return NIL;
}

#endif
