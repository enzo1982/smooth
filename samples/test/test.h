 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TEST_
#define _H_TEST_

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class Test : public Application
{
	private:
		Bool		 checkbox;
		Int		 optionboxes;
		Int		 slider1;
		Int		 arrows1;
		Int		 scrollbar1;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Statusbar	*mainWnd_statusbar;
		Client		*mainWnd_client;
		Divider		*mainWnd_divisionbar;
		Menubar		*mainWnd_menubar;
		Menubar		*mainWnd_menubar2;
		Menubar		*mainWnd_iconbar;
		Menu		*mainWnd_menubar_file;
		Layer		*mainWnd_layer;
		Layer		*rlayer1;
		Layer		*rlayer2;
		Layer		*rlayer3;
		Button		*mainWnd_layer_button;
		CheckBox	*mainWnd_layer_check1;
		CheckBox	*mainWnd_layer_check2;
		ActiveArea	*mainWnd_layer_active1;
		OptionBox	*mainWnd_layer_option1;
		OptionBox	*mainWnd_layer_option2;
		Text		*mainWnd_layer_text1;
		Hyperlink	*mainWnd_layer_link1;
		GroupBox	*mainWnd_layer_group1;
		TabWidget	*mainWnd_layer_reg1;
		ListBox		*mainWnd_layer_list1;
		ComboBox	*mainWnd_layer_combo1;
		TreeView	*mainWnd_layer_tree1;
		ListBox		*mainWnd_layer_list2;
		Slider		*rlayer1_slider1;
		Arrows		*rlayer1_arrows1;
		EditBox		*rlayer2_editbox1;
		Scrollbar	*rlayer3_scrollbar1;
		Progressbar	*rlayer3_progress1;
		Window		*secWnd;
		Titlebar	*secWnd_titlebar;
		Tree		*mainWnd_layer_subtree1;

		Thread		*messageBoxThread;

		Bool		 mainWnd_KillProc();
		void		 mainApp_InfoProc();
		void		 mainApp_ScrollbarProc();
		void		 ShowEdb();
		void		 HideEdb();
		Menu		*GetTrackMenu(Int, Int);
		Int		 threadMessageBox(Thread *);
		void		 Close();

		Void		 testDlgSelectColor();
		Void		 testDlgOpenFile();
		Void		 testDlgSelectDir();
	public:
				 Test();
				~Test();
};

#endif
