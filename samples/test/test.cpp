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
#include "test.h"

Int smooth::Main()
{
	SMOOTH::SetLanguage(LNG_ENGLISH);
	String::SetInputFormat("UTF-8");

	Test	*app = new Test();

	Loop();

	Object::DeleteObject(app);

	return 0;
}

Test::Test()
{
	checkbox = false;
	optionboxes = 1;
	slider1 = 3;
	arrows1 = 50;
	scrollbar1 = 50;

	Point	 bp;
	Size	 bs;

	bp.x = 30;
	bp.y = 38;
	bs.cx = 0;
	bs.cy = 0;

	// create objects for our application:
	mainWnd			= new Window("smooth Test");
	mainWnd_titlebar	= new Titlebar();
	mainWnd_statusbar	= new Statusbar("Ready");
	mainWnd_client		= new Client();
	mainWnd_divisionbar	= new Divider(430, OR_VERT | OR_LEFT);
	mainWnd_menubar		= new Menubar();
	mainWnd_menubar2	= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd_menubar_file	= new Menu();
	mainWnd_layer		= new Layer();
	rlayer1			= new Layer("Register");
	rlayer2			= new Layer("Test");
	rlayer3			= new Layer("Other");
	mainWnd_layer_button	= new Button("OK", NIL, bp, bs);
	mainWnd_layer_button->onClick.Connect(&Test::Close, this);
	mainWnd_layer_button->SetTooltip("Don't click on this button!!!");
	mainWnd_layer_button->SetBackgroundColor(RGB(255, 0, 0));
	mainWnd_layer_button->SetFlags(BF_NOFRAME | BF_SHOWHIGHLIGHT);
	bp.y += 25;
	mainWnd_layer_check1	= new CheckBox("Checkable", bp, bs, &checkbox);
	bp.y += 20;
	mainWnd_layer_check2	= new CheckBox("Click me!", bp, bs, &checkbox);
	bp.y += 20;
	mainWnd_layer_active1	= new ActiveArea(RGB(0, 255, 0), bp, bs);
	mainWnd_layer_active1->onClick.Connect(&Test::testDlgSelectColor, this);
	bp.y -= 65;
	bp.x += 90;
	mainWnd_layer_option1	= new OptionBox("Chooseable", bp, bs, &optionboxes, 1);
	mainWnd_layer_option1->onClick.Connect(&Test::ShowEdb, this);
	bp.y += 20;
	mainWnd_layer_option2	= new OptionBox("Chooseable", bp, bs, &optionboxes, 2);
	mainWnd_layer_option2->onClick.Connect(&Test::HideEdb, this);
	bp.y += 20;
	mainWnd_layer_text1	= new Text("smooth::Text", bp);
	bp.y += 20;
	mainWnd_layer_link1	= new Hyperlink("www.cool.com", NIL, "www.cool.com", bp);
	bp.y = 23;
	bp.x = 20;
	bs.cx = 190;
	bs.cy = 110;
	mainWnd_layer_group1	= new GroupBox("Group 1", bp, bs);
	bp.y = 150;
	bs.cx = 190;
	bs.cy = 130;
	mainWnd_layer_reg1	= new TabWidget(bp, bs);
	bp.y = 300;
	bs.cy = 120;
	mainWnd_layer_list1	= new ListBox(bp, bs);
	mainWnd_layer_list1->SetFlags(LF_MULTICHECKBOX);
	bp.y = 28;
	bp.x = 220;
	bs.cx = 150;
	bs.cy = 0;
	mainWnd_layer_combo1	= new ComboBox(bp, bs);
	bp.y += 73;
	bp.x += 20;
	bs.cy = 200;
	mainWnd_layer_tree1	= new TreeView("Library files", bp, bs);
	bp.y += 220;
	bs.cy = 100;
	mainWnd_layer_list2	= new ListBox(bp, bs);
	bp.y = 40;
	bp.x = 20;
	bs.cx = 100;
	bs.cy = 0;
	rlayer1_slider1		= new Slider(bp, bs, OR_HORZ, &slider1, 0, 5);
	bp.x = 150;
	bs.cx = 0;
	bs.cy = 0;
	rlayer1_arrows1		= new Arrows(bp, bs, OR_HORZ, &arrows1, 0, 100);
	bp.x = 15;
	bp.y = 20;
	bs.cx = 150;
	bs.cy = 0;
	rlayer2_editbox1	= new EditBox("Hello to all testers!", bp, bs, 0);
	rlayer2_editbox1->SetDropDownList(mainWnd_layer_list2);
	bp.y = 40;
	bs.cx = 60;
	bs.cy = 0;
	rlayer3_scrollbar1	= new Scrollbar(bp, bs, OR_HORZ, &scrollbar1, 0, 100);
	rlayer3_scrollbar1->onClick.Connect(&Test::mainApp_ScrollbarProc, this);
	bp.y -= 30;
	bs.cx = 160;
	bs.cy = 0;
	rlayer3_progress1	= new Progressbar(bp, bs, OR_HORZ, PB_PERCENT, 0, 100, 50);

	secWnd			= new Window("Another window");
	secWnd_titlebar		= new Titlebar(TB_MINBUTTON | TB_CLOSEBUTTON);
	mainWnd_layer_subtree1	= new Tree();

	// fill the menus:

	Menu::Entry	*entry;

	entry = mainWnd_menubar_file->AddEntry("Exit");
	entry->onClick.Connect(&Test::Close, this);
	entry->SetStatusText("Exit this program");

	entry = mainWnd_menubar_file->AddEntry("Test");
	entry->onClick.Connect(&Test::HideEdb, this);
	entry->SetStatusText("Execute test proc");

	mainWnd_menubar_file->AddEntry("Exit", NIL, mainWnd_menubar_file);

	mainWnd_menubar->AddEntry("File test")->onClick.Connect(&Test::testDlgOpenFile, this);
	mainWnd_menubar->AddEntry("Directory test")->onClick.Connect(&Test::testDlgSelectDir, this);
	mainWnd_menubar->AddEntry("File", NIL, mainWnd_menubar_file);
	mainWnd_menubar->AddEntry("Quit")->onClick.Connect(&Test::Close, this);

	mainWnd_menubar2->AddEntry("ECM test");
	mainWnd_menubar2->AddEntry("Unicode: Да");

//mainWnd_menubar2->SetOrientation(OR_BOTTOM);

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("icons.pci", 1, NIL));//mainWnd_menubar_file);
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("icons.pci", 2, NIL))->onClick.Connect(&Test::mainApp_InfoProc, this);

mainWnd_iconbar->SetOrientation(OR_LEFT);

	mainWnd_layer_list1->AddEntry("libiolib.a");
	mainWnd_layer_list1->AddEntry("libmpstring.a");
	mainWnd_layer_list1->AddEntry("libpicture.a");
	mainWnd_layer_list1->AddEntry("libiolib.a");
	mainWnd_layer_list1->AddEntry("libmpstring.a");
	mainWnd_layer_list1->AddEntry("libpicture.a");
	mainWnd_layer_list1->AddEntry("libiolib.a");
	mainWnd_layer_list1->AddEntry("libmpstring.a");
	mainWnd_layer_list1->AddEntry("libpicture.a");
	mainWnd_layer_list1->AddEntry("libiolib.a");
	mainWnd_layer_list1->AddEntry("libmpstring.a");
	mainWnd_layer_list1->AddEntry("libpicture.a");
	mainWnd_layer_list1->AddEntry("libiolib.a");
	mainWnd_layer_list1->AddEntry("libmpstring.a");
	mainWnd_layer_list1->AddEntry("libpicture.a");

	mainWnd_layer_list2->AddEntry("libiolib.a");
	mainWnd_layer_list2->AddEntry("libmpstring.a");
	mainWnd_layer_list2->AddEntry("libpicture.a");
	mainWnd_layer_list2->AddEntry("libiolib.a");
	mainWnd_layer_list2->AddEntry("libmpstring.a");
	mainWnd_layer_list2->AddEntry("libpicture.a");

	mainWnd_layer_combo1->AddEntry("libmpstring.a");
	mainWnd_layer_combo1->AddEntry("libiolib.a");
	mainWnd_layer_combo1->AddEntry("libpicture.a");
	mainWnd_layer_combo1->AddEntry("libiolib.a");
	mainWnd_layer_combo1->AddEntry("libmpstring.a");
	mainWnd_layer_combo1->AddEntry("libpicture.a");

	mainWnd_layer_subtree1->AddEntry("This");
	mainWnd_layer_subtree1->AddEntry("is");
	mainWnd_layer_subtree1->AddEntry("a", mainWnd_layer_subtree1);
	mainWnd_layer_subtree1->AddEntry("subtree!", mainWnd_layer_subtree1);

	mainWnd_layer_tree1->AddEntry("libpicture.a");
	mainWnd_layer_tree1->AddEntry("libiolib.a");
	mainWnd_layer_tree1->AddEntry("libmpstring.a");
	mainWnd_layer_tree1->AddEntry("SubTree", mainWnd_layer_subtree1);
	mainWnd_layer_tree1->AddEntry("libpicture.a");

	// alle Objekte dort registrieren, wo sie hingeh�ren:

	messageBoxThread = new Thread();
	messageBoxThread->threadMain.Connect(&Test::threadMessageBox, this);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_client);
	mainWnd->RegisterObject(mainWnd_divisionbar);
	mainWnd->RegisterObject(mainWnd_menubar);
	mainWnd->RegisterObject(mainWnd_iconbar);
	mainWnd->RegisterObject(mainWnd_menubar2);
	mainWnd->RegisterObject(mainWnd_layer);

	mainWnd_layer->RegisterObject(mainWnd_layer_button);
	mainWnd_layer->RegisterObject(mainWnd_layer_check1);
	mainWnd_layer->RegisterObject(mainWnd_layer_check2);
	mainWnd_layer->RegisterObject(mainWnd_layer_option1);
	mainWnd_layer->RegisterObject(mainWnd_layer_option2);
	mainWnd_layer->RegisterObject(mainWnd_layer_group1);
	mainWnd_layer->RegisterObject(mainWnd_layer_text1);
	mainWnd_layer->RegisterObject(mainWnd_layer_link1);
	mainWnd_layer->RegisterObject(mainWnd_layer_active1);
	mainWnd_layer->RegisterObject(mainWnd_layer_reg1);
	mainWnd_layer->RegisterObject(mainWnd_layer_list1);
	mainWnd_layer->RegisterObject(mainWnd_layer_list2);
	mainWnd_layer->RegisterObject(mainWnd_layer_combo1);
	mainWnd_layer->RegisterObject(mainWnd_layer_tree1);

	rlayer1->RegisterObject(rlayer1_slider1);
	rlayer1->RegisterObject(rlayer1_arrows1);
	rlayer2->RegisterObject(rlayer2_editbox1);
	rlayer3->RegisterObject(rlayer3_scrollbar1);
	rlayer3->RegisterObject(rlayer3_progress1);

	mainWnd_layer_reg1->RegisterObject(rlayer1);
	mainWnd_layer_reg1->RegisterObject(rlayer2);
	mainWnd_layer_reg1->RegisterObject(rlayer3);

	RegisterObject(secWnd);

	secWnd->RegisterObject(secWnd_titlebar);

	// jetzt werden einige Fensterparameter festgelegt und die Fenster werden erzeugt:
	mainWnd->SetMetrics(Point(100, 50), Size(570, 550));
	mainWnd->SetIcon(SMOOTH::LoadImage("icons.pci", 0, NIL));
	mainWnd->SetApplicationIcon(MAKEINTRESOURCEW(32517));
	mainWnd->getTrackMenu.Connect(&Test::GetTrackMenu, this);
	mainWnd->doQuit.Connect(&Test::mainWnd_KillProc, this);

	secWnd->SetMetrics(Point(480, 130), Size(170, 90));
	secWnd->SetIcon(SMOOTH::LoadImage("icons.pci", 0, NIL));
	secWnd->SetFlags(WF_APPTOPMOST);

	messageBoxThread->Start();
}

Test::~Test()
{
	messageBoxThread->Stop();

	// das Programm wurde vom User beendet, alle Objekte werden dort abgemeldet, wo sie registriert wurden:
	secWnd->UnregisterObject(secWnd_titlebar);

	UnregisterObject(secWnd);

	rlayer1->UnregisterObject(rlayer1_slider1);
	rlayer1->UnregisterObject(rlayer1_arrows1);
	rlayer2->UnregisterObject(rlayer2_editbox1);
	rlayer3->UnregisterObject(rlayer3_scrollbar1);
	rlayer3->UnregisterObject(rlayer3_progress1);

	mainWnd_layer_reg1->UnregisterObject(rlayer1);
	mainWnd_layer_reg1->UnregisterObject(rlayer2);
	mainWnd_layer_reg1->UnregisterObject(rlayer3);

	mainWnd_layer->UnregisterObject(mainWnd_layer_button);
	mainWnd_layer->UnregisterObject(mainWnd_layer_check1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_check2);
	mainWnd_layer->UnregisterObject(mainWnd_layer_option1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_option2);
	mainWnd_layer->UnregisterObject(mainWnd_layer_group1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_text1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_link1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_active1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_reg1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_list1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_list2);
	mainWnd_layer->UnregisterObject(mainWnd_layer_combo1);
	mainWnd_layer->UnregisterObject(mainWnd_layer_tree1);

	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(mainWnd_iconbar);
	mainWnd->UnregisterObject(mainWnd_menubar);
	mainWnd->UnregisterObject(mainWnd_menubar2);
	mainWnd->UnregisterObject(mainWnd_divisionbar);
	mainWnd->UnregisterObject(mainWnd_client);
	mainWnd->UnregisterObject(mainWnd_statusbar);
	mainWnd->UnregisterObject(mainWnd_titlebar);

	UnregisterObject(mainWnd);

	// alle Obbjekte werden aus dem Speicher entfernt:
	DeleteObject(secWnd_titlebar);
	DeleteObject(secWnd);

	DeleteObject(mainWnd_layer_button);
	DeleteObject(mainWnd_layer_check1);
	DeleteObject(mainWnd_layer_check2);
	DeleteObject(mainWnd_layer_option1);
	DeleteObject(mainWnd_layer_option2);
	DeleteObject(mainWnd_layer_group1);
	DeleteObject(mainWnd_layer_text1);
	DeleteObject(mainWnd_layer_link1);
	DeleteObject(mainWnd_layer_active1);
	DeleteObject(mainWnd_layer_reg1);
	DeleteObject(mainWnd_layer_list1);
	DeleteObject(mainWnd_layer_list2);
	DeleteObject(mainWnd_layer_combo1);
	DeleteObject(mainWnd_layer_tree1);

	delete mainWnd_layer_subtree1;

	DeleteObject(rlayer1_slider1);
	DeleteObject(rlayer1_arrows1);
	DeleteObject(rlayer2_editbox1);
	DeleteObject(rlayer3_scrollbar1);
	DeleteObject(rlayer3_progress1);

	DeleteObject(rlayer1);
	DeleteObject(rlayer2);
	DeleteObject(rlayer3);

	delete mainWnd_menubar_file;

	DeleteObject(mainWnd_layer);
	DeleteObject(mainWnd_iconbar);
	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_menubar2);
	DeleteObject(mainWnd_divisionbar);
	DeleteObject(mainWnd_client);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);

	DeleteObject(messageBoxThread);
}

Bool Test::mainWnd_KillProc()
{
	SMOOTH::MessageBox("Leaving application now!", "Info", MB_OK, IDI_INFORMATION);

	secWnd->Close();

	return True;
}

void Test::mainApp_InfoProc()
{
	SMOOTH::MessageBox("Hallo!", "Info", MB_OKCANCEL, IDI_INFORMATION);

	mainWnd_layer_check1->SetText("Changed!");
}

void Test::mainApp_ScrollbarProc()
{
	rlayer3_progress1->SetValue(scrollbar1);

	mainWnd->SetStatusText("kick it!");
}

void Test::ShowEdb()
{
	rlayer2_editbox1->Show();
}

void Test::HideEdb()
{
	rlayer2_editbox1->Hide();
}

void Test::Close()
{
	SMOOTH::MessageBox("You clicked the button!", "Hello", MB_OK, IDI_INFORMATION);
//	mainWnd->Close();
}

Int Test::threadMessageBox(Thread *thread)
{
	thread->SetFlags(THREAD_KILLFLAG_WAIT);	// for this thread can quit itself (after closing the MessageBox) it needn't be terminated by smooth (KILLTHREAD_KILL is default)

	SMOOTH::MessageBox("This MessageBox is running in a separate thread!", "Info", MB_OK, IDI_INFORMATION);

	thread->Stop();

	return Success;
}

Void Test::testDlgSelectColor()
{
	DialogColorSelection	*dialog = new DialogColorSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetColor(mainWnd_layer_active1->GetColor());

	dialog->ShowDialog();

	mainWnd_layer_active1->SetColor(dialog->GetColor());

	delete dialog;
}

Void Test::testDlgOpenFile()
{
	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	dialog->AddFilter("All Files", "*.*");
	dialog->AddFilter("JPEG Files (*.jpg; *.jpeg)", "*.jpg; *.jpeg");
	dialog->AddFilter("Text Files (*.txt)", "*.txt");

	if (dialog->ShowDialog() == Success)
	{
		String	 message = "Selected files:\n";

		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			message.Append("\n").Append(dialog->GetNthFileName(i));
		}

		SMOOTH::MessageBox(message, "Info", MB_OK, IDI_INFORMATION);
	}

	delete dialog;
}

Void Test::testDlgSelectDir()
{
	DialogDirSelection	*dialog = new DialogDirSelection();

	dialog->SetParentWindow(mainWnd);

	if (dialog->ShowDialog() == Success)
	{
		SMOOTH::MessageBox(dialog->GetDirName(), "Info", MB_OK, IDI_INFORMATION);
	}

	delete dialog;
}

Menu *Test::GetTrackMenu(Int mouseX, Int mouseY)
{
	return mainWnd_menubar_file;
}
