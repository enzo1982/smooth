 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include "mditest.h"

void SMOOTH::Main()
{
	MDITest	*app = new MDITest();

	SMOOTH::Loop();

	delete app;
}

MDITest::MDITest()
{
	SetText("MDITest");

	mainWnd			= new SMOOTHWindow("MDITest");
	mainWnd_titlebar	= new SMOOTHTitlebar(true, true, true);
	mainWnd_statusbar	= new SMOOTHStatusbar("Bereit");
	mainWnd_client		= new SMOOTHMDIClient();
	mainWnd_menubar		= new SMOOTHMenubar();
	menu_file		= new SMOOTHPopupMenu();

	menu_file->AddEntry("Neu", NIL, SMOOTHProc(MDITest, this, NewMDI));
	menu_file->AddEntry();
	menu_file->AddEntry("Beenden", NIL, SMOOTHProc(MDITest, this, QuitProc));

	mainWnd_menubar->AddEntry("Datei", NIL, NULLPROC, menu_file);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_client);
	mainWnd->RegisterObject(mainWnd_menubar);

	mainWnd->SetMetrics(SMOOTHPoint(50, 50), SMOOTHSize(700, 500));
	mainWnd->SetIcon(SI_DEFAULT);
	mainWnd->SetKillProc(SMOOTHKillProc(MDITest, this, KillProc));
}

MDITest::~MDITest()
{
	mainWnd->UnregisterObject(mainWnd_statusbar);
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_client);
	mainWnd->UnregisterObject(mainWnd_menubar);

	UnregisterObject(mainWnd);

	delete mainWnd_statusbar;
	delete mainWnd_titlebar;
	delete mainWnd_client;
	delete mainWnd_menubar;
	delete menu_file;
	delete mainWnd;
}

SMOOTHVoid MDITest::QuitProc()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHBool MDITest::KillProc()
{
	SMOOTHInt	 id = SMOOTH::MessageBox("Möchten sie das Programm wirklich beenden?", "Frage", MB_YESNO, IDI_QUESTION);

	switch (id)
	{
		case IDYES:
			return true;
		default:
		case IDNO:
			return false;
	}
}

SMOOTHVoid MDITest::NewMDI()
{
}
