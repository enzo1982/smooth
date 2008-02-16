 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "mditest.h"

using namespace smooth::GUI::Dialogs;

Int smooth::Main()
{
	MDITest	*app = new MDITest();

	app->Loop();

	Object::DeleteObject(app);

	return 0;
}

MDITest::MDITest()
{
	mainWnd			= new Window("MDITest", Point(50, 50), Size(700, 500));
	mainWnd_titlebar	= new Titlebar();
	mainWnd_statusbar	= new Statusbar("Ready");
	mainWnd_client		= new MDI::Client();
	mainWnd_menubar		= new Menubar();
	menu_file		= new PopupMenu();

	menu_file->AddEntry("New")->onAction.Connect(&MDITest::NewMDI, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Exit")->onAction.Connect(&MDITest::Close, this);

	mainWnd_menubar->AddEntry("File", NIL, menu_file);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(mainWnd_statusbar);
	mainWnd->Add(mainWnd_client);
	mainWnd->Add(mainWnd_menubar);

	mainWnd->SetIcon(NIL);
	mainWnd->doQuit.Connect(&MDITest::ExitProc, this);
}

MDITest::~MDITest()
{
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_client);
	DeleteObject(mainWnd_menubar);
	DeleteObject(menu_file);
	DeleteObject(mainWnd);
}

Bool MDITest::ExitProc()
{
	Int	 id = QuickMessage("Do you really want to quit?", "Exit program", MB_YESNO, IDI_QUESTION);

	switch (id)
	{
		case IDYES:
			return True;
		default:
		case IDNO:
			return False;
	}
}

Void MDITest::Close()
{
	mainWnd->Close();
}

Void MDITest::NewMDI()
{
}
