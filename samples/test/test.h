 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TEST_
#define _H_TEST_

#include <smoothx.h>

class Test : public SMOOTHApplication
{
	private:
		SBool		 checkbox;
		SInt		 optionboxes;
		SInt		 slider1;
		SInt		 arrows1;
		SInt		 scrollbar1;

		SWindow		*mainWnd;
		STitlebar	*mainWnd_titlebar;
		SStatusbar	*mainWnd_statusbar;
		SClient		*mainWnd_client;
		SDivisionbar	*mainWnd_divisionbar;
		SMenubar	*mainWnd_menubar;
		SMenubar	*mainWnd_menubar2;
		SMenubar	*mainWnd_iconbar;
		SPopupMenu	*mainWnd_menubar_file;
		SLayer		*mainWnd_layer;
		SLayer		*rlayer1;
		SLayer		*rlayer2;
		SLayer		*rlayer3;
		SButton		*mainWnd_layer_button;
		SCheckBox	*mainWnd_layer_check1;
		SCheckBox	*mainWnd_layer_check2;
		SActiveArea	*mainWnd_layer_active1;
		SOptionBox	*mainWnd_layer_option1;
		SOptionBox	*mainWnd_layer_option2;
		SText		*mainWnd_layer_text1;
		SHyperlink	*mainWnd_layer_link1;
		SGroupBox	*mainWnd_layer_group1;
		STabRegister	*mainWnd_layer_reg1;
		SListBox	*mainWnd_layer_list1;
		SComboBox	*mainWnd_layer_combo1;
		STreeView	*mainWnd_layer_tree1;
		SListBox	*mainWnd_layer_list2;
		SSlider		*rlayer1_slider1;
		SArrows		*rlayer1_arrows1;
		SEditBox	*rlayer2_editbox1;
		SScrollbar	*rlayer3_scrollbar1;
		SProgressbar	*rlayer3_progress1;
		SWindow		*secWnd;
		STitlebar	*secWnd_titlebar;
		STree		*mainWnd_layer_subtree1;

		SThread		*messageBoxThread;

		SBool		 mainWnd_KillProc();
		void		 mainApp_QuitProc();
		void		 mainApp_InfoProc();
		void		 mainApp_ScrollbarProc();
		void		 ShowEdb();
		void		 HideEdb();
		void		 threadMessageBox(SThread *);

		SMOOTHVoid	 testDlgSelectColor();
		SMOOTHVoid	 testDlgOpenFile();
		SMOOTHVoid	 testDlgSelectDir();
	public:
				 Test();
				~Test();
};

#endif
