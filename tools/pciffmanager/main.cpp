 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

	Loop();

	delete app;

	return 0;
}

PCIFFManager::PCIFFManager()
{
	filename = "";

	wnd		= new Window(String("smooth PCIFF Manager v").Append(SMOOTH_VERSION));
	title		= new Titlebar(true, false, true);
	menubar		= new Menubar();
	statusbar	= new Statusbar("Ready");

	menu_file	= new PopupMenu();

	menubar->AddEntry("&File", NIL, NULLPROC, menu_file);

	menu_file->AddEntry("&Open", NIL, Proc(PCIFFManager, this, OpenFile));
	menu_file->AddEntry();
	menu_file->AddEntry("E&xit", NIL, Proc(Window, wnd, Close));

	RegisterObject(wnd);

	wnd->RegisterObject(title);
	wnd->RegisterObject(menubar);
	wnd->RegisterObject(statusbar);

	wnd->SetMetrics(Point(50, 50), Size(600, 200));
	wnd->SetIcon(SI_DEFAULT);

	wnd->Show();
}

PCIFFManager::~PCIFFManager()
{
	wnd->UnregisterObject(title);
	wnd->UnregisterObject(menubar);
	wnd->UnregisterObject(statusbar);

	UnregisterObject(wnd);

	delete title;
	delete wnd;
	delete menubar;
	delete statusbar;
	delete menu_file;
}

void PCIFFManager::OpenFile()
{
	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(wnd);

	dialog->AddFilter("PCI Image Files", "*.pci");
	dialog->AddFilter("All Files", "*.*");

	if (dialog->ShowDialog() == Success)
	{
		filename = dialog->GetFileName();

		String	 file = "";
		Int	 lastBs = -1;

		for (int i = 0; i < filename.Length(); i++) if (filename[i] == '\\') lastBs = i;

		for (int j = ++lastBs; j < filename.Length(); j++) file[j - lastBs] = filename[j];

		wnd->SetText(String("smooth PCIFF Manager v").Append(SMOOTH_VERSION).Append(" - ").Append(file));
	}

	delete dialog;
}
