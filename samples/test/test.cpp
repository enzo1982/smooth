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
#include "test.h"

void SMOOTH::Main()
{
	SMOOTH::SetLanguage(LNG_ENGLISH);
	SString::SetInputFormat(SIF_UTF8);

	Test	*app = new Test();

	SMOOTH::Loop();

	SMOOTH::DeleteObject(app);
}

Test::Test()
{
	SetText("SMOOTH::Test");

	checkbox = false;
	optionboxes = 1;
	slider1 = 3;
	arrows1 = 50;
	scrollbar1 = 50;

	SPoint	 bp;
	SSize	 bs;

	bp.x = 30;
	bp.y = 38;
	bs.cx = 0;
	bs.cy = 0;

	// create objects for our application:
	mainWnd			= new SWindow("SMOOTH::Test");
	mainWnd_titlebar	= new STitlebar(true, true, true);
	mainWnd_statusbar	= new SStatusbar("Ready");
	mainWnd_client		= new SClient(NULLPROC);
	mainWnd_divisionbar	= new SDivisionbar(430, OR_VERT | OR_LEFT);
	mainWnd_menubar		= new SMenubar();
	mainWnd_menubar2	= new SMenubar();
	mainWnd_iconbar		= new SMenubar();
	mainWnd_menubar_file	= new SPopupMenu();
	mainWnd_layer		= new SLayer();
	rlayer1			= new SLayer("Register");
	rlayer2			= new SLayer("Test");
	rlayer3			= new SLayer("Other");
	mainWnd_layer_button	= new SButton("OK", NIL, bp, bs, SProc(Test, this, mainApp_QuitProc));
	mainWnd_layer_button->SetTooltip("Don't click on this button!!!");
	bp.y += 25;
	mainWnd_layer_check1	= new SCheckBox("Checkable", bp, bs, &checkbox, NULLPROC);
	bp.y += 20;
	mainWnd_layer_check2	= new SCheckBox("Click me!", bp, bs, &checkbox, NULLPROC);
	bp.y += 20;
	mainWnd_layer_active1	= new SActiveArea(RGB(0, 255, 0), bp, bs, SProc(Test, this, testDlgSelectColor));
	bp.y -= 65;
	bp.x += 90;
	mainWnd_layer_option1	= new SOptionBox("Chooseable", bp, bs, &optionboxes, 1, SProc(Test, this, ShowEdb));
	bp.y += 20;
	mainWnd_layer_option2	= new SOptionBox("Chooseable", bp, bs, &optionboxes, 2, SProc(Test, this, HideEdb));
	bp.y += 20;
	mainWnd_layer_text1	= new SText("SMOOTHText", bp);
	bp.y += 20;
	mainWnd_layer_link1	= new SHyperlink("www.cool.com", NIL, "www.cool.com", bp);
	bp.y = 23;
	bp.x = 20;
	bs.cx = 190;
	bs.cy = 110;
	mainWnd_layer_group1	= new SGroupBox("Group 1", bp, bs);
	bp.y = 150;
	bs.cx = 190;
	bs.cy = 130;
	mainWnd_layer_reg1	= new STabRegister(bp, bs);
	bp.y = 300;
	bs.cy = 120;
	mainWnd_layer_list1	= new SListBox(bp, bs, NULLPROC);
	bp.y = 28;
	bp.x = 220;
	bs.cx = 150;
	bs.cy = 0;
	mainWnd_layer_combo1	= new SComboBox(bp, bs, NULLPROC);
	bp.y += 73;
	bp.x += 20;
	bs.cy = 200;
	mainWnd_layer_tree1	= new STreeView("Library files", bp, bs, NULLPROC);
	bp.y += 220;
	bs.cy = 100;
	mainWnd_layer_list2	= new SListBox(bp, bs, NULLPROC);
	bp.y = 40;
	bp.x = 20;
	bs.cx = 100;
	bs.cy = 0;
	rlayer1_slider1		= new SSlider(bp, bs, OR_HORZ, &slider1, 0, 5, NULLPROC);
	bp.x = 150;
	bs.cx = 0;
	bs.cy = 0;
	rlayer1_arrows1		= new SArrows(bp, bs, OR_HORZ, &arrows1, 0, 100, NULLPROC);
	bp.x = 15;
	bp.y = 20;
	bs.cx = 150;
	bs.cy = 70;
	rlayer2_editbox1	= new SEditBox("Hello to all testers!", bp, bs, EDB_ALPHANUMERIC, 0, NULLPROC);
	bp.y = 40;
	bs.cx = 60;
	bs.cy = 0;
	rlayer3_scrollbar1	= new SScrollbar(bp, bs, OR_HORZ, &scrollbar1, 0, 100, SProc(Test, this, mainApp_ScrollbarProc));
	bp.y -= 30;
	bs.cx = 160;
	bs.cy = 0;
	rlayer3_progress1	= new SProgressbar(bp, bs, OR_HORZ, PB_PERCENT, 0, 100, 50);

	secWnd			= new SWindow("Another window");
	secWnd_titlebar		= new STitlebar(true, false, true);
	mainWnd_layer_subtree1	= new STree();

	// fill the menus:
	mainWnd_menubar_file->AddEntry("Exit", NIL, SProc(Test, this, mainApp_QuitProc))->SetStatusText("Exit this program");
	mainWnd_menubar_file->AddEntry("Test", NIL, SProc(Test, this, HideEdb))->SetStatusText("Execute test proc");
	mainWnd_menubar_file->AddEntry("Exit", NIL, NULLPROC, mainWnd_menubar_file);

	mainWnd_menubar->AddEntry("File test", NIL, SProc(Test, this, testDlgOpenFile));
	mainWnd_menubar->AddEntry("Directory test", NIL, SProc(Test, this, testDlgSelectDir));
	mainWnd_menubar->AddEntry("&File", NIL, NULLPROC, mainWnd_menubar_file);
	mainWnd_menubar->AddEntry("Quit", NIL, SProc(Test, this, mainApp_QuitProc));

	mainWnd_menubar2->AddEntry("ECM test");
	mainWnd_menubar2->AddEntry("Unicode: Да");

//mainWnd_menubar2->SetOrientation(OR_BOTTOM);

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("icons.pci", 1, NIL));//mainWnd_menubar_file);
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("icons.pci", 2, NIL), SProc(Test, this, mainApp_InfoProc));

mainWnd_iconbar->SetOrientation(OR_LEFT);

	mainWnd_layer_list1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list1->AddEntry("libpicture.a", NULLPROC);

	mainWnd_layer_list2->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list2->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list2->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_list2->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_list2->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_list2->AddEntry("libpicture.a", NULLPROC);

	mainWnd_layer_combo1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_combo1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_combo1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_combo1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_combo1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_combo1->AddEntry("libpicture.a", NULLPROC);

	mainWnd_layer_subtree1->AddEntry("This", NULLPROC);
	mainWnd_layer_subtree1->AddEntry("is", NULLPROC);
	mainWnd_layer_subtree1->AddEntry("a", mainWnd_layer_subtree1);
	mainWnd_layer_subtree1->AddEntry("subtree!", mainWnd_layer_subtree1);

	mainWnd_layer_tree1->AddEntry("libpicture.a", NULLPROC);
	mainWnd_layer_tree1->AddEntry("libiolib.a", NULLPROC);
	mainWnd_layer_tree1->AddEntry("libmpstring.a", NULLPROC);
	mainWnd_layer_tree1->AddEntry("SubTree", mainWnd_layer_subtree1);
	mainWnd_layer_tree1->AddEntry("libpicture.a", NULLPROC);

	// alle Objekte dort registrieren, wo sie hingeh�ren:

	messageBoxThread = new SThread(SThreadProc(Test, this, threadMessageBox));

	RegisterObject(mainWnd);
	RegisterObject(messageBoxThread);

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
	mainWnd->SetMetrics(SMOOTHPoint(100, 50), SMOOTHSize(570, 550));
	mainWnd->SetIcon(SMOOTH::LoadImage("icons.pci", 0, NIL));
	mainWnd->SetApplicationIcon(LoadIconA(NULL, MAKEINTRESOURCEA(32517)));
	mainWnd->SetKillProc(SKillProc(Test, this, mainWnd_KillProc));

	secWnd->SetMetrics(SMOOTHPoint(480, 130), SMOOTHSize(170, 90));
	secWnd->SetIcon(SMOOTH::LoadImage("icons.pci", 0, NIL));
	secWnd->SetStyle(SS_APPTOPMOST);

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
	UnregisterObject(messageBoxThread);

	// alle Obbjekte werden aus dem Speicher entfernt:
	SMOOTH::DeleteObject(secWnd_titlebar);
	SMOOTH::DeleteObject(secWnd);

	SMOOTH::DeleteObject(mainWnd_layer_button);
	SMOOTH::DeleteObject(mainWnd_layer_check1);
	SMOOTH::DeleteObject(mainWnd_layer_check2);
	SMOOTH::DeleteObject(mainWnd_layer_option1);
	SMOOTH::DeleteObject(mainWnd_layer_option2);
	SMOOTH::DeleteObject(mainWnd_layer_group1);
	SMOOTH::DeleteObject(mainWnd_layer_text1);
	SMOOTH::DeleteObject(mainWnd_layer_link1);
	SMOOTH::DeleteObject(mainWnd_layer_active1);
	SMOOTH::DeleteObject(mainWnd_layer_reg1);
	SMOOTH::DeleteObject(mainWnd_layer_list1);
	SMOOTH::DeleteObject(mainWnd_layer_list2);
	SMOOTH::DeleteObject(mainWnd_layer_combo1);
	SMOOTH::DeleteObject(mainWnd_layer_tree1);

	delete mainWnd_layer_subtree1;

	SMOOTH::DeleteObject(rlayer1_slider1);
	SMOOTH::DeleteObject(rlayer1_arrows1);
	SMOOTH::DeleteObject(rlayer2_editbox1);
	SMOOTH::DeleteObject(rlayer3_scrollbar1);
	SMOOTH::DeleteObject(rlayer3_progress1);

	SMOOTH::DeleteObject(rlayer1);
	SMOOTH::DeleteObject(rlayer2);
	SMOOTH::DeleteObject(rlayer3);

	SMOOTH::DeleteObject(mainWnd_layer);
	SMOOTH::DeleteObject(mainWnd_iconbar);
	SMOOTH::DeleteObject(mainWnd_menubar);
	SMOOTH::DeleteObject(mainWnd_menubar2);
	SMOOTH::DeleteObject(mainWnd_menubar_file);
	SMOOTH::DeleteObject(mainWnd_divisionbar);
	SMOOTH::DeleteObject(mainWnd_client);
	SMOOTH::DeleteObject(mainWnd_statusbar);
	SMOOTH::DeleteObject(mainWnd_titlebar);
	SMOOTH::DeleteObject(mainWnd);

	SMOOTH::DeleteObject(messageBoxThread);
}

SBool Test::mainWnd_KillProc()
{
	SMOOTH::MessageBox("Leaving application now!", "Info", MB_OK, IDI_INFORMATION);

	SMOOTH::CloseWindow(secWnd);

	return true;
}

void Test::mainApp_QuitProc()
{
	SMOOTH::CloseWindow(mainWnd);
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
	rlayer2_editbox1->Activate();
}

void Test::HideEdb()
{
	rlayer2_editbox1->Deactivate();
}

void Test::threadMessageBox(SThread *thread)
{
	thread->SetKillFlag(THREAD_KILLFLAG_WAIT);	// for this thread can quit itself (after closing the MessageBox) it needn't be terminated by SMOOTH (KILLTHREAD_KILL is default)

	SMOOTH::MessageBox("This MessageBox is running in a separate thread!", "Info", MB_OK, IDI_INFORMATION);

	thread->Stop();
}

SMOOTHVoid Test::testDlgSelectColor()
{
	SDialogColorSelection	*dialog = new SDialogColorSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetColor(mainWnd_layer_active1->GetColor());

	dialog->ShowDialog();

	mainWnd_layer_active1->SetColor(dialog->GetColor());

	delete dialog;
}

SMOOTHVoid Test::testDlgOpenFile()
{
	SDialogFileSelection	*dialog = new SDialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	dialog->AddFilter("All Files", "*.*");
	dialog->AddFilter("JPEG Files (*.jpg; *.jpeg)", "*.jpg; *.jpeg");
	dialog->AddFilter("Text Files (*.txt)", "*.txt");

	if (dialog->ShowDialog() == SMOOTH::Success)
	{
		SMOOTHString	 message = "Selected files:\n";

		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			message.Append("\n").Append(dialog->GetNthFileName(i));
		}

		SMOOTH::MessageBox(message, "Info", MB_OK, IDI_INFORMATION);
	}

	delete dialog;
}

SMOOTHVoid Test::testDlgSelectDir()
{
	SDialogDirSelection	*dialog = new SDialogDirSelection();

	dialog->SetParentWindow(mainWnd);

	if (dialog->ShowDialog() == SMOOTH::Success)
	{
		SMOOTH::MessageBox(dialog->GetDirName(), "Info", MB_OK, IDI_INFORMATION);
	}

	delete dialog;
}
