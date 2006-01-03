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
#include <smooth/main.h>
#include "designer.h"
#include "edit_component.h"

Int smooth::Main()
{
	Designer	*app = new Designer();

	Loop();

	delete app;

	return 0;
}

Designer::Designer()
{
	dlgcounter = 1;
	active_dlg = NIL;

	wnd		= new Window(String("smooth Designer v").Append(SMOOTH_VERSION), Point(50, 50), Size(600, 85));
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	menubar		= new Menubar();
	iconbar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	timer1		= new Timer();
	timer1->onInterval.Connect(&Designer::TimerProc, this);

	menu_file	= new PopupMenu();
	menu_dialog	= new PopupMenu();

	menu_widgets		= new PopupMenu();
	menu_widgets_add	= new PopupMenu();
	menu_widgets_add_smooth	= new PopupMenu();

	menubar->AddEntry("File", NIL, menu_file);
	menubar->AddEntry("Dialog", NIL, menu_dialog);
	menubar->AddEntry("Widgets", NIL, menu_widgets);

	menu_file->AddEntry("Exit")->onAction.Connect(&Designer::Close, this);

	menu_dialog->AddEntry("New")->onAction.Connect(&Designer::NewDialog, this);

	menu_widgets->AddEntry("Add", NIL, menu_widgets_add);

	menu_widgets_add->AddEntry("smooth controls", NIL, menu_widgets_add_smooth);

	menu_widgets_add_smooth->AddEntry("Button")->onAction.Connect(&Designer::AddButton, this);
	menu_widgets_add_smooth->AddEntry("Layer")->onAction.Connect(&Designer::AddLayer, this);
	menu_widgets_add_smooth->AddEntry("Menubar")->onAction.Connect(&Designer::AddMenubar, this);

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(menubar);
	wnd->RegisterObject(iconbar);
	wnd->RegisterObject(statusbar);

	wnd->SetIcon(NIL);

	wnd->doQuit.Connect(&Designer::ExitProc, this);

	wnd->Show();

	timer1->Start(100);
}

Designer::~Designer()
{
	timer1->Stop();

	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(menubar);
	DeleteObject(iconbar);
	DeleteObject(statusbar);
	DeleteObject(timer1);

	delete menu_file;
	delete menu_dialog;
	delete menu_widgets;
	delete menu_widgets_add;
	delete menu_widgets_add_smooth;
}

void Designer::Close()
{
	wnd->Close();
}

Bool Designer::ExitProc()
{
	for (Int i = 0; i < dlgs.GetNOfEntries(); i++)
	{
		delete dlgs.GetNthEntry(i);
	}

	dlgs.RemoveAll();

	return True;
}

void Designer::NewDialog()
{
	Designer_EditComponent	*dlg = new Designer_EditComponent(this, String("Dialog").Append(String::FromInt(dlgcounter++)));

	dlgs.AddEntry(dlg);

	dlg->ShowDialog();
}

void Designer::ReportStatus(Designer_Status stat)
{
	switch (stat.event)
	{
		case STATUS_EVENT_REPORT_NAME:
			if (active_dlg != stat.dlg) wnd->SetText(String("smooth Designer v").Append(SMOOTH_VERSION).Append(" - ").Append(stat.currdlgname));

			active_dlg = stat.dlg;

			break;
		case STATUS_EVENT_REPORT_MOUSEPOSITION:
			statusbar->SetText(String("Mouse: ").Append(String::FromInt(stat.mousex)).Append(", ").Append(String::FromInt(stat.mousey)));

			break;
		case STATUS_EVENT_REPORT_WINDOWPOSITION:
			statusbar->SetText(String("Position: ").Append(String::FromInt(stat.wndposx)).Append(", ").Append(String::FromInt(stat.wndposy)));

			break;
		case STATUS_EVENT_REPORT_WINDOWSIZE:
			statusbar->SetText(String("Size: ").Append(String::FromInt(stat.wndsizex)).Append(", ").Append(String::FromInt(stat.wndsizey)));

			break;
		case STATUS_EVENT_REPORT_QUIT:
			wnd->SetText(String("smooth Designer v").Append(SMOOTH_VERSION));

			break;
	}

	status = stat;
}

void Designer::TimerProc()
{
	Designer_EditComponent	*object;
	Bool			 mouseOnWindow = False;

	for (Int i = 0; i < dlgs.GetNOfEntries(); i++)
	{
		object = dlgs.GetNthEntry(i);

		if (object->GetWindow()->MouseX() > 0 && object->GetWindow()->MouseY() > 0 && object->GetWindow()->MouseX() < object->GetWindow()->GetWidth() && object->GetWindow()->MouseY() < object->GetWindow()->GetHeight())
		{
			mouseOnWindow = True;

			break;
		}
	}

	if (!mouseOnWindow && (statusbar->GetText() != "Ready")) statusbar->SetText("Ready");
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
