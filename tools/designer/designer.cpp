 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DESIGNER_
#define __OBJSMOOTH_DESIGNER_

#include <smooth.h>
#include "designer.h"
#include "edit_component.h"
#include "tools.h"

SMOOTHVoid SMOOTH::Main()
{
	Designer	*app = new Designer();

	SMOOTH::Loop();

	delete app;
}

Designer::Designer()
{
	dlgcounter = 1;

	POINT		 pos;
	SIZE		 size;

	pos.x = 2;
	pos.y = 2;
	size.cx = 389;
	size.cy = 169;

	wnd		= new SMOOTHWindow(SMOOTHString("SMOOTH Designer v").Append(SMOOTH_VERSION));
	title		= new SMOOTHTitlebar(true, false, true);
	menubar		= new SMOOTHMenubar();
	iconbar		= new SMOOTHMenubar();
	statusbar	= new SMOOTHStatusbar("Ready");

	timer1		= new SMOOTHTimer();

	timer1->SetProc(SMOOTHProc(Designer, this, TimerProc));

	menu_file	= new SMOOTHPopupMenu();
	menu_dialog	= new SMOOTHPopupMenu();

	menu_widgets		= new SMOOTHPopupMenu();
	menu_widgets_add	= new SMOOTHPopupMenu();
	menu_widgets_add_smooth	= new SMOOTHPopupMenu();

	menubar->AddEntry("&File", NIL, NULLPROC, menu_file);
	menubar->AddEntry("&Dialog", NIL, NULLPROC, menu_dialog);
	menubar->AddEntry("&Widgets", NIL, NULLPROC, menu_widgets);

	menu_file->AddEntry("E&xit", NIL, SMOOTHProc(Designer, this, CloseWindow));

	menu_dialog->AddEntry("&New", NIL, SMOOTHProc(Designer, this, NewDialog));

	menu_widgets->AddEntry("&Add", NIL, NULLPROC, menu_widgets_add);

	menu_widgets_add->AddEntry("&SMOOTH controls", NIL, NULLPROC, menu_widgets_add_smooth);

	menu_widgets_add_smooth->AddEntry("SMOOTH&Button", NIL, SMOOTHProc(Designer, this, AddButton));
	menu_widgets_add_smooth->AddEntry("SMOOTH&Layer", NIL, SMOOTHProc(Designer, this, AddLayer));
	menu_widgets_add_smooth->AddEntry("SMOOTH&Menubar", NIL, SMOOTHProc(Designer, this, AddMenubar));

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(menubar);
	wnd->RegisterObject(iconbar);
	wnd->RegisterObject(statusbar);
	wnd->RegisterObject(timer1);

	wnd->SetMetrics(SMOOTHPoint(50, 50), SMOOTHSize(600, 85));
	wnd->SetIcon(SI_DEFAULT);
	wnd->SetKillProc(SMOOTHKillProc(Designer, this, KillProc));

	wnd->Show();

	timer1->Start(100);
}

Designer::~Designer()
{
	timer1->Stop();

	wnd->UnregisterObject(title);
	wnd->UnregisterObject(menubar);
	wnd->UnregisterObject(iconbar);
	wnd->UnregisterObject(statusbar);
	wnd->UnregisterObject(timer1);

	UnregisterObject(wnd);

	delete title;
	delete wnd;
	delete menubar;
	delete iconbar;
	delete statusbar;
	delete timer1;
	delete menu_file;
	delete menu_dialog;
	delete menu_widgets;
	delete menu_widgets_add;
	delete menu_widgets_add_smooth;
}

void Designer::CloseWindow()
{
	SMOOTH::CloseWindow(wnd);
}

bool Designer::KillProc()
{
	for (int i = 0; i < dlgs.GetNOfEntries(); i++)
	{
		delete dlgs.GetNthEntry(i);
	}

	dlgs.DeleteAll();

	return true;
}

void Designer::NewDialog()
{
	Designer_EditComponent	*dlg = new Designer_EditComponent(this, SMOOTHString("Dialog").Append(SMOOTHString::IntToString(dlgcounter++)));

	dlgs.AddEntry(dlg);

	dlg->ShowDialog();
}

void Designer::ReportStatus(Designer_Status stat)
{
	switch (stat.event)
	{
		case STATUS_EVENT_REPORT_NAME:
			if (active_dlg != stat.dlg) wnd->SetText(SMOOTHString("SMOOTH Designer v").Append(SMOOTH_VERSION).Append(" - ").Append(stat.currdlgname));
			active_dlg = stat.dlg;
			break;
		case STATUS_EVENT_REPORT_MOUSEPOSITION:
			statusbar->SetText(SMOOTHString("Mouse: ").Append(SMOOTHString::IntToString(stat.mousex)).Append(", ").Append(SMOOTHString::IntToString(stat.mousey)));
			break;
		case STATUS_EVENT_REPORT_WINDOWPOSITION:
			statusbar->SetText(SMOOTHString("Position: ").Append(SMOOTHString::IntToString(stat.wndposx)).Append(", ").Append(SMOOTHString::IntToString(stat.wndposy)));
			break;
		case STATUS_EVENT_REPORT_WINDOWSIZE:
			statusbar->SetText(SMOOTHString("Size: ").Append(SMOOTHString::IntToString(stat.wndsizex)).Append(", ").Append(SMOOTHString::IntToString(stat.wndsizey)));
			break;
		case STATUS_EVENT_REPORT_QUIT:
			wnd->SetText(SMOOTHString("SMOOTH Designer v").Append(SMOOTH_VERSION));
			break;
	}

	status = stat;
}

void Designer::TimerProc()
{
	Designer_EditComponent	*operat;
	bool			 mouseonwindow = false;

	for (int i = 0; i < dlgs.GetNOfEntries(); i++)
	{
		operat = dlgs.GetNthEntry(i);

		if (MouseX(operat->GetWindow()->hwnd, WINDOW) > 0 && MouseY(operat->GetWindow()->hwnd, WINDOW) > 0 && MouseX(operat->GetWindow()->hwnd, WINDOW) < operat->GetWindow()->GetObjectProperties()->size.cx && MouseY(operat->GetWindow()->hwnd, WINDOW) < operat->GetWindow()->GetObjectProperties()->size.cy)
		{
			mouseonwindow = true;

			break;
		}
	}

	if (!mouseonwindow && (statusbar->GetText() != "Ready")) statusbar->SetText("Ready");
}

void Designer::AddButton()
{
	active_dlg->AddObject(SMOOTH_BUTTON);
}

void Designer::AddLayer()
{
	active_dlg->AddObject(SMOOTH_LAYER);
}

void Designer::AddMenubar()
{
	active_dlg->AddObject(SMOOTH_MENUBAR);
}

#endif
