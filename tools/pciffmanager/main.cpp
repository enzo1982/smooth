 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include "main.h"

Int smooth::Main()
{
	PCIFFManager	*app = new PCIFFManager();

	app->Loop();

	delete app;

	return 0;
}

PCIFFManager::PCIFFManager()
{
	filename = "";

	wnd		= new GUI::Window(String("smooth PCIFF Manager v").Append(SMOOTH_VERSION), Point(50, 50), Size(600, 200));
	title		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	menu_file	= new PopupMenu();

	menubar->AddEntry("File", NIL, menu_file);

	menu_file->AddEntry("Open")->onAction.Connect(&PCIFFManager::OpenFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry("Exit")->onAction.Connect(&PCIFFManager::Close, this);

	Add(wnd);

	wnd->Add(title);
	wnd->Add(menubar);
	wnd->Add(statusbar);

	wnd->SetIcon(NIL);

	wnd->Show();
}

PCIFFManager::~PCIFFManager()
{
	DeleteObject(title);
	DeleteObject(wnd);
	DeleteObject(menubar);
	DeleteObject(menu_file);
	DeleteObject(statusbar);
}

Void PCIFFManager::Close()
{
	wnd->Close();
}

Void PCIFFManager::OpenFile()
{
	FileSelection	 dialog;

	dialog.SetParentWindow(wnd);

	dialog.AddFilter("PCI Image Files", "*.pci");
	dialog.AddFilter("All Files", "*.*");

	if (dialog.ShowDialog() == Success())
	{
		filename = dialog.GetFileName();

		String	 file = filename.Tail(filename.Length() - filename.FindLast(Directory::GetDirectoryDelimiter()) - 1);

		wnd->SetText(String("smooth PCIFF Manager v").Append(SMOOTH_VERSION).Append(" - ").Append(file));
	}
}
