 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TEST_
#define _H_TEST_

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

class Test : public Application
{
	private:
		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Statusbar	*mainWnd_statusbar;

		Divider		*mainWnd_divider1;
		Divider		*mainWnd_divider2;

		Client		*mainWnd_client;

		Menubar		*mainWnd_menubar;

		PopupMenu	*menu_file;
		PopupMenu	*menu_dialogs;
		PopupMenu	*menu_menus;

		PopupMenu	*menu_2ndlevel;

		Text		*text_arrows;
		Arrows		*widget_arrows;
		Text		*text_arrows_value;

		Text		*text_activearea;
		ActiveArea	*widget_activearea;

		Text		*text_button;
		Button		*widget_button;

		Text		*text_hyperlink;
		Hyperlink	*widget_hyperlink;

		Text		*text_scrollbar;
		Scrollbar	*widget_scrollbar;
		Text		*text_scrollbar_value;

		Text		*text_progressbar;
		Progressbar	*widget_progressbar;

		Text		*text_checkbox;
		CheckBox	*widget_checkbox;
		Bool		 checkbox_var;

		Text		*text_optionbox;
		OptionBox	*widget_optionbox1;
		OptionBox	*widget_optionbox2;
		Int		 optionbox_var;

		Text		*text_slider;
		Slider		*widget_slider;

		Text		*text_editbox;
		EditBox		*widget_editbox;

		Text		*text_combobox;
		ComboBox	*widget_combobox;

		Button		*button_show;
		Button		*button_hide;
		Button		*button_activate;
		Button		*button_deactivate;
	public:
				 Test();
		virtual		~Test();
	slots:
		Void		 ColorDlg();

		Void		 OnArrowsValueChange(Int);
		Void		 OnScrollbarValueChange(Int);

		Void		 OnWidgetAction();
};

#endif
