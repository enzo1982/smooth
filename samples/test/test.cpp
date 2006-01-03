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

#include "test.h"

using namespace smooth;

Int smooth::Main()
{
	Test	*application = new Test();

	Loop();

	Object::DeleteObject(application);

	return 0;
}

Test::Test()
{
	mainWnd			= new Window("smooth Test", Point(100, 100), Size(600, 500));
	mainWnd_titlebar	= new Titlebar();
	mainWnd_statusbar	= new Statusbar("Ready");

	mainWnd_divider1	= new Divider(280, OR_HORZ | OR_TOP);
	mainWnd_divider2	= new Divider(200, OR_VERT | OR_LEFT);

	mainWnd_client		= new Client();

	mainWnd_menubar		= new Menubar();

	menu_file		= new PopupMenu();
	menu_file->AddEntry("Exit")->onAction.Connect(&Window::Close, mainWnd);

	menu_dialogs		= new PopupMenu();
	menu_dialogs->AddEntry("File chooser...");
	menu_dialogs->AddEntry("Directory chooser...");
	menu_dialogs->AddEntry();
	menu_dialogs->AddEntry("Color selector...")->onAction.Connect(&Test::ColorDlg, this);

	menu_2ndlevel		= new PopupMenu();
	menu_2ndlevel->AddEntry("Click")->onAction.Connect(&Test::OnWidgetAction, this);

	checkbox_var		= True;
	optionbox_var		= 0;

	menu_menus		= new PopupMenu();
	menu_menus->AddEntry("2nd level popup menu", NIL, menu_2ndlevel);
	menu_menus->AddEntry();
	menu_menus->AddEntry("CheckBox menu entry", NIL, NIL, &checkbox_var);
	menu_menus->AddEntry();
	menu_menus->AddEntry("OptionBox menu entry", NIL, NIL, NIL, &optionbox_var, 0);
	menu_menus->AddEntry("OptionBox menu entry", NIL, NIL, NIL, &optionbox_var, 1);

	mainWnd_menubar->AddEntry("File", NIL, menu_file);
	mainWnd_menubar->AddEntry("Dialogs", NIL, menu_dialogs);
	mainWnd_menubar->AddEntry("Menus", NIL, menu_menus);

	text_arrows		= new Text("Arrows:", Point(10, 10));
	widget_arrows		= new Arrows(Point(70, 10), Size(24, 17), OR_HORZ);
	widget_arrows->onValueChange.Connect(&Test::OnArrowsValueChange, this);
	text_arrows_value	= new Text("0", Point(100, 10));

	text_activearea		= new Text("ActiveArea:", Point(10, 35));
	widget_activearea	= new ActiveArea(Color(255, 0, 0), Point(70, 35), Size(24, 17));
	widget_activearea->onAction.Connect(&Test::OnWidgetAction, this);

	text_button		= new Text("Button:", Point(10, 60));
	widget_button		= new Button("Click", NIL, Point(70, 60), Size(80, 22));
	widget_button->onAction.Connect(&Test::OnWidgetAction, this);

	text_hyperlink		= new Text("Hyperlink:", Point(10, 85));
	widget_hyperlink	= new Hyperlink("Click", NIL, "http://www.smooth-project.org/", Point(70, 85));

	text_scrollbar		= new Text("Scrollbar:", Point(10, 110));
	widget_scrollbar	= new Scrollbar(Point(70, 110), Size(120, 17), OR_HORZ);
	widget_scrollbar->onValueChange.Connect(&Test::OnScrollbarValueChange, this);
	text_scrollbar_value	= new Text("0", Point(200, 110));

	text_progressbar	= new Text("Progressbar:", Point(10, 135));
	widget_progressbar	= new Progressbar(Point(70, 135), Size(120, 19), OR_HORZ);

	text_slider		= new Text("Slider:", Point(10, 160));
	widget_slider		= new Slider(Point(70, 160), Size(120, 18), OR_HORZ);
	widget_slider->onValueChange.Connect(&Progressbar::SetValue, widget_progressbar);

	text_checkbox		= new Text("CheckBox:", Point(10, 185));
	widget_checkbox		= new CheckBox("Click", Point(70, 185), Size(60, 17), &checkbox_var);

	text_optionbox		= new Text("OptionBox:", Point(10, 210));
	widget_optionbox1	= new OptionBox("Click", Point(70, 210), Size(60, 17), &optionbox_var, 0);
	widget_optionbox2	= new OptionBox("Click", Point(140, 210), Size(60, 17), &optionbox_var, 1);

	text_editbox		= new Text("EditBox:", Point(250, 10));
	widget_editbox		= new EditBox("MyEditBox", Point(310, 10), Size(100, 19));
	widget_editbox->SetBackgroundColor(Color(255, 255, 255));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);

	mainWnd->RegisterObject(mainWnd_divider1);
	mainWnd->RegisterObject(mainWnd_divider2);
	mainWnd->RegisterObject(mainWnd_client);

	mainWnd->RegisterObject(mainWnd_menubar);

	mainWnd->RegisterObject(text_arrows);
	mainWnd->RegisterObject(widget_arrows);
	mainWnd->RegisterObject(text_arrows_value);

	mainWnd->RegisterObject(text_activearea);
	mainWnd->RegisterObject(widget_activearea);

	mainWnd->RegisterObject(text_button);
	mainWnd->RegisterObject(widget_button);

	mainWnd->RegisterObject(text_hyperlink);
	mainWnd->RegisterObject(widget_hyperlink);

	mainWnd->RegisterObject(text_scrollbar);
	mainWnd->RegisterObject(widget_scrollbar);
	mainWnd->RegisterObject(text_scrollbar_value);

	mainWnd->RegisterObject(text_progressbar);
	mainWnd->RegisterObject(widget_progressbar);

	mainWnd->RegisterObject(text_slider);
	mainWnd->RegisterObject(widget_slider);

	mainWnd->RegisterObject(text_checkbox);
	mainWnd->RegisterObject(widget_checkbox);

	mainWnd->RegisterObject(text_optionbox);
	mainWnd->RegisterObject(widget_optionbox1);
	mainWnd->RegisterObject(widget_optionbox2);

	mainWnd->RegisterObject(text_editbox);
	mainWnd->RegisterObject(widget_editbox);

	mainWnd->SetIcon(NIL);
}

Test::~Test()
{
	DeleteObject(text_arrows);
	DeleteObject(widget_arrows);
	DeleteObject(text_arrows_value);

	DeleteObject(text_activearea);
	mainWnd->RegisterObject(widget_activearea);

	DeleteObject(text_button);
	DeleteObject(widget_button);

	DeleteObject(text_hyperlink);
	DeleteObject(widget_hyperlink);

	DeleteObject(text_scrollbar);
	DeleteObject(widget_scrollbar);
	DeleteObject(text_scrollbar_value);

	DeleteObject(text_progressbar);
	DeleteObject(widget_progressbar);

	DeleteObject(text_slider);
	DeleteObject(widget_slider);

	DeleteObject(text_checkbox);
	DeleteObject(widget_checkbox);

	DeleteObject(text_optionbox);
	DeleteObject(widget_optionbox1);
	DeleteObject(widget_optionbox2);

	DeleteObject(text_editbox);
	DeleteObject(widget_editbox);

	DeleteObject(menu_file);
	DeleteObject(menu_dialogs);

	DeleteObject(mainWnd_menubar);

	DeleteObject(mainWnd_divider1);
	DeleteObject(mainWnd_divider2);
	DeleteObject(mainWnd_client);

	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
}

Void Test::ColorDlg()
{
	ColorSelection	*dialog = new ColorSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->ShowDialog();

	DeleteObject(dialog);
}

Void Test::OnArrowsValueChange(Int value)
{
	text_arrows_value->SetText(String::FromInt(value));
}

Void Test::OnScrollbarValueChange(Int value)
{
	text_scrollbar_value->SetText(String::FromInt(value));
}

Void Test::OnWidgetAction()
{
	QuickMessage("Some widget action occurred!", "Info", MB_OK, IDI_INFORMATION);
}
