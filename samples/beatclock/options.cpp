 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "options.h"
#include "time.h"

Options::Options(Int &itimeformat, Bool &icenti, Bool &iisalarm, Int &ialarmbeats, Int &ialarmsecs, Int &imodechange, Int &itimezone, String &ialarmtext, Int &ialarmoption) : timeformat(itimeformat), centi(icenti), isalarm(iisalarm), alarmbeats(ialarmbeats), alarmsecs(ialarmsecs), modechange(imodechange), timezone(itimezone), alarmtext(ialarmtext), alarmoption(ialarmoption)
{
	oldtf = timeformat;
	oldct = centi;
	oldmc = modechange;
	oldtz = timezone;
	oldia = isalarm;
	oldab = alarmbeats;
	oldas = alarmsecs;
	oldat = alarmtext;
	oldao = alarmoption;

	optionsdialog	= new GUI::Window("BeatClock Options", Point(100, 100), Size(419, 183));
	optitle		= new Titlebar(TB_CLOSEBUTTON);

	/* Configuring "Display" Layer:
	 */
	display		= new Layer("Display");

	display_group1	= new GroupBox("Time format", Point(7, 11), Size(250, 65));

	display_option1 = new OptionBox("Internet Beats", Point(10, 13), Size(110, 0), &timeformat, 0);
	display_option1->onAction.Connect(&Options::OptionsBeats, this);

	display_option2 = new OptionBox("Standard (STF)", Point(10, 38), Size(110, 0), &timeformat, 1);
	display_option2->onAction.Connect(&Options::OptionsSTF, this);

	if (timeformat == 0) display_check1 = new CheckBox("Show centibeats", Point(129, 13), Size(110, 0), &centi);
	else		     display_check1 = new CheckBox("Show seconds", Point(129, 13), Size(110, 0), &centi);

	display_check1->onAction.Connect(&onOptionChange);

	display_group1->Add(display_option1);
	display_group1->Add(display_option2);
	display_group1->Add(display_check1);

	display_group2	= new GroupBox("Timezone", Point(265, 11), Size(121, 65));

	display_option3 = new OptionBox("CET", Point(10, 13), Size(100, 0), &timezone, 0);
	display_option3->onAction.Connect(&onOptionChange);

	display_option4 = new OptionBox("Local time", Point(10, 38), Size(100, 0), &timezone, 1);
	display_option4->onAction.Connect(&onOptionChange);

	display_group2->Add(display_option3);
	display_group2->Add(display_option4);

	display->Add(display_group1);
	display->Add(display_group2);

	/* Configuring "Alarm" layer:
	 */
	alarm		= new Layer("Alarm");

	alarm_check1	= new CheckBox("Enable alarm", Point(7, 7), Size(100, 0), &isalarm);
	alarm_check1->onAction.Connect(&Options::ToggleAlarmState, this);

	alarm_text1	= new Text("Ring at:", Point(9, 35));

	if (timeformat == 0) alarmtext = String::FromInt(alarmbeats);
	else		     alarmtext = Time::SecondsToTimeString(alarmsecs);

	alarm_edit1	= new EditBox(alarmtext, Point(16 + alarm_text1->GetUnscaledTextWidth(), 32), Size(34, 0), 5);

	if (timeformat == 0) alarm_text2 = new Text("Internet Beats", Point(alarm_edit1->GetX() + 41, 35));
	else		     alarm_text2 = new Text("Hours/Minutes", Point(alarm_edit1->GetX() + 41, 35));

	alarm_option1	= new OptionBox("once", Point(180, 21), Size(0, 0), &alarmoption, 0);
	alarm_option2	= new OptionBox("every day", Point(180, 46), Size(0, 0), &alarmoption, 1);

	alarm->Add(alarm_check1);
	alarm->Add(alarm_text1);
	alarm->Add(alarm_text2);
	alarm->Add(alarm_edit1);
	alarm->Add(alarm_option1);
	alarm->Add(alarm_option2);

	ToggleAlarmState();

	/* Configuring "Misc" layer:
	 */
	misc		= new Layer("Misc");

	misc_group1	= new GroupBox("'Mode' button action", Point(7, 11), Size(379, 65));

	if (timeformat == 0) misc_option1 = new OptionBox("Show/hide centibeats", Point(10, 13), Size(174, 0), &modechange, 0);
	else		     misc_option1 = new OptionBox("Show/hide seconds", Point(10, 13), Size(174, 0), &modechange, 0);

	misc_option2	= new OptionBox("Change time format", Point(10, 38), Size(174, 0), &modechange, 1);
	misc_option3	= new OptionBox("Change timezone", Point(194, 13), Size(174, 0), &modechange, 2);
	misc_option4	= new OptionBox("Change format and timezone", Point(194, 38), Size(174, 0), &modechange, 3);

	misc_group1->Add(misc_option1);
	misc_group1->Add(misc_option2);
	misc_group1->Add(misc_option3);
	misc_group1->Add(misc_option4);

	misc->Add(misc_group1);

	/* Configuring "Info" layer:
	 */
	info		= new Layer("Info");

	info_text1	= new Text("BeatClock version 2.0\n\nDeveloped by Robert Kausch 2000-2017\nGive it to all your friends!", Point(6, 5));
	info_text2	= new Text("\n\neMail: robert.kausch@gmx.net", Point(227, 5));

	info->Add(info_text1);
	info->Add(info_text2);

	/* Configuring main layer:
	 */
	main_reg1 = new TabWidget(Point(7, 7), Size(396, 105));

	main_reg1->Add(display);
	main_reg1->Add(misc);
	main_reg1->Add(alarm);
	main_reg1->Add(info);

	main_button1 = new Button("OK", NIL, Point(175, 29), Size(0, 0));
	main_button1->onAction.Connect(&Options::OptionsOK, this);
	main_button1->SetOrientation(OR_LOWERRIGHT);

	main_button2 = new Button("Cancel", NIL, Point(87, 29), Size(0, 0));
	main_button2->onAction.Connect(&Options::OptionsCancel, this);
	main_button2->SetOrientation(OR_LOWERRIGHT);

	optionsdialog->Add(optitle);
	optionsdialog->Add(main_button1);
	optionsdialog->Add(main_button2);
	optionsdialog->Add(main_reg1);

	Add(optionsdialog);

	optionsdialog->SetFlags(optionsdialog->GetFlags() | WF_MODAL);
	optionsdialog->SetIcon(ImageLoader::Load("beat.pci:0"));
}

Options::~Options()
{
	DeleteObject(optionsdialog);
	DeleteObject(optitle);

	DeleteObject(display);
	DeleteObject(alarm);
	DeleteObject(misc);
	DeleteObject(info);

	DeleteObject(main_button1);
	DeleteObject(main_button2);
	DeleteObject(main_reg1);

	DeleteObject(info_text1);
	DeleteObject(info_text2);

	DeleteObject(alarm_check1);
	DeleteObject(alarm_text1);
	DeleteObject(alarm_text2);
	DeleteObject(alarm_edit1);
	DeleteObject(alarm_option1);
	DeleteObject(alarm_option2);

	DeleteObject(misc_group1);
	DeleteObject(misc_option1);
	DeleteObject(misc_option2);
	DeleteObject(misc_option3);
	DeleteObject(misc_option4);

	DeleteObject(display_group1);
	DeleteObject(display_group2);
	DeleteObject(display_check1);
	DeleteObject(display_option1);
	DeleteObject(display_option2);
	DeleteObject(display_option3);
	DeleteObject(display_option4);
}

const Error &Options::ShowDialog()
{
	optionsdialog->WaitUntilClosed();

	return error;
}

Void Options::OptionsOK()
{
	if (timeformat == 0)
	{
		alarmbeats = Math::Min(999, (Int) alarm_edit1->GetText().ToInt());
		alarmsecs  = Time::BeatsToSeconds(alarmbeats);
	}
	else
	{
		alarmsecs  = Math::Min(86340, Time::TimeStringToSeconds(alarm_edit1->GetText()));
		alarmbeats = Time::SecondsToBeats(alarmsecs);
	}

	optionsdialog->Close();
}

Void Options::OptionsCancel()
{
	timeformat  = oldtf;
	centi	     = oldct;
	modechange  = oldmc;
	timezone    = oldtz;
	isalarm     = oldia;
	alarmbeats  = oldab;
	alarmsecs   = oldas;
	alarmtext   = oldat;
	alarmoption = oldao;

	optionsdialog->Close();
}

Void Options::OptionsBeats()
{
	display_check1->SetText("Show centibeats");
	misc_option1->SetText("Show/hide centibeats");
	alarm_text2->SetText("Internet Beats");
	alarm_edit1->SetText(String::FromInt(Math::Min(999, Time::SecondsToBeats(Time::TimeStringToSeconds(alarm_edit1->GetText())))));
}

Void Options::OptionsSTF()
{
	display_check1->SetText("Show seconds");
	misc_option1->SetText("Show/hide seconds");
	alarm_text2->SetText("Hours/Minutes");
	alarm_edit1->SetText(Time::SecondsToTimeString(Math::Min(86340, Time::BeatsToSeconds(alarm_edit1->GetText().ToInt()))));
}

Void Options::ToggleAlarmState()
{
	if (isalarm)
	{
		alarm_edit1->Activate();
		alarm_option1->Activate();
		alarm_option2->Activate();
	}
	else
	{
		alarm_edit1->Deactivate();
		alarm_option1->Deactivate();
		alarm_option2->Deactivate();
	}
}
