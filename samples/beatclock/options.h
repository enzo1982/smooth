 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BEATCLOCK_OPTIONS
#define H_BEATCLOCK_OPTIONS

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

class Options : public Dialog
{
	private:
		GUI::Window	*optionsdialog;
		Layer		*display;
		Layer		*alarm;
		Layer		*misc;
		Layer		*info;
		Titlebar	*optitle;

		GroupBox 	*display_group1;
		GroupBox	*display_group2;
		OptionBox	*display_option1;
		OptionBox	*display_option2;
		CheckBox	*display_check1;
		OptionBox	*display_option3;
		OptionBox	*display_option4;

		CheckBox	*alarm_check1;
		Text		*alarm_text1;
		EditBox		*alarm_edit1;
		Text		*alarm_text2;
		OptionBox	*alarm_option1;
		OptionBox	*alarm_option2;

		GroupBox	*misc_group1;
		OptionBox	*misc_option1;
		OptionBox	*misc_option2;
		OptionBox	*misc_option3;
		OptionBox	*misc_option4;

		Text		*info_text1;
		Text		*info_text2;

		Button		*main_button1;
		Button		*main_button2;
		TabWidget	*main_reg1;

		Int		&timeformat;
		Bool		&centi;
		Bool		&isalarm;
		Int		&alarmbeats;
		Int		&alarmsecs;
		Int		&modechange;
		Int		&timezone;
		String		&alarmtext;
		Int		&alarmoption;

		Int		 oldtf;
		Bool		 oldct;
		Int		 oldmc;
		Int		 oldtz;
		Bool		 oldia;
		Int		 oldab;
		Int		 oldas;
		String		 oldat;
		Int		 oldao;
	public:
				 Options(Int &, Bool &, Bool &, Int &, Int &, Int &, Int &, String &, Int &);
				~Options();

		const Error	&ShowDialog();
	signals:
		Signal0<Void>	 onOptionChange;
	slots:
		Void		 OptionsOK();
		Void		 OptionsCancel();
		Void		 OptionsBeats();
		Void		 OptionsSTF();
		Void		 ToggleAlarmState();
};

#endif
