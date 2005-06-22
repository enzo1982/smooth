 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>

#include "test.h"

Int smooth::Main()
{
	Test	*application = new Test();

	Loop();

	Object::DeleteObject(application);

	return 0;
}

Test::Test()
{
	mainWnd			= new Window("smooth Test");
	mainWnd_titlebar	= new Titlebar();
	mainWnd_statusbar	= new Statusbar("Ready");

	mainWnd_menubar		= new Menubar();

	menu_file		= new Menu();
	menu_file->AddEntry("Exit")->onClick.Connect(&Window::Close, mainWnd);

	menu_dialogs		= new Menu();
	menu_dialogs->AddEntry("File chooser...");
	menu_dialogs->AddEntry("Directory chooser...");
	menu_dialogs->AddEntry();
	menu_dialogs->AddEntry("Color selector...");

	mainWnd_menubar->AddEntry("File", NIL, menu_file);
	mainWnd_menubar->AddEntry("Dialogs", NIL, menu_dialogs);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_menubar);

	mainWnd->SetMetrics(Point(100, 100), Size(600, 500));
	mainWnd->SetIcon(SI_DEFAULT);
}

Test::~Test()
{
	DeleteObject(menu_file);
	DeleteObject(menu_dialogs);

	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
}
