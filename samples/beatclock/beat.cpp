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
#include "beat.h"

Int smooth::Main()
{
	BeatClock	*app = new BeatClock();

	Loop();

	delete app;

	return 0;
}

BeatClock::BeatClock()
{
	actbeats = -1;
	actcbeats = -1;
	actccbeats = -1;
	wmpaint = true;
	timeformat = 0;
	centi = true;
	isalarm = false;
	alarmexec = false;
	alarmbeats = 0;
	alarmsecs = 0;
	alarmoption = 0;
	wpx = 16;
	wpy = 16;
	modechange = 3;
	timezone = 0;

	InputValues();

	wnd		= new Window("BeatClock");
	title		= new Titlebar(TB_CLOSEBUTTON);
	menubar		= new Menubar();
	timer		= new Timer();
	dragcontrol	= new DragControl();

	timer->onInterval.Connect(&BeatClock::PaintTime, this);

	Menu::Entry	*entry;

	menubar->AddEntry("Mode")->onClick.Connect(&BeatClock::Mode, this);
	menubar->AddEntry("Options")->onClick.Connect(&BeatClock::Options, this);
	menubar->AddEntry()->SetOrientation(OR_RIGHT);

	entry = menubar->AddEntry("Info");
	entry->onClick.Connect(&BeatClock::Info, this);
	entry->SetOrientation(OR_RIGHT);

	RegisterObject(wnd);

	wnd->RegisterObject(dragcontrol);
	wnd->RegisterObject(title);
	wnd->RegisterObject(menubar);

	wnd->SetIcon(SMOOTH::LoadImage("beat.pci", 0, NIL));
	wnd->onPaint.Connect(&BeatClock::PaintAll, this);
	wnd->SetFlags(WF_TOPMOST | WF_NOTASKBUTTON);
	wnd->SetMetrics(Point(wpx, wpy), Size(164 * Setup::FontSize, 103 * Setup::FontSize));
	wnd->onEvent.Connect(&BeatClock::EventProc, this);
	wnd->Show();

	timer->Start(50);
}

BeatClock::~BeatClock()
{
	timer->Stop();

	RegisterValues();

	UnregisterObject(wnd);

	wnd->UnregisterObject(title);
	wnd->UnregisterObject(menubar);
	wnd->UnregisterObject(dragcontrol);

	delete wnd;
	delete title;
	delete menubar;
	delete timer;
	delete dragcontrol;
}

Void BeatClock::EventProc(Int message, Int wParam, Int lParam)
{
	if (message == WM_WINDOWPOSCHANGED)
	{
		wpx = ((LPWINDOWPOS) lParam)->x;
		wpy = ((LPWINDOWPOS) lParam)->y;
	}
}

Void BeatClock::Options()
{
	optionsdialog	= new Window("BeatClock Options");
	display		= new Layer("Display");
	alarm		= new Layer("Alarm");
	misc		= new Layer("Misc");
	info		= new Layer("Info");
	optitle		= new Titlebar(TB_CLOSEBUTTON);

	Point	 pos;
	Size	 size;

// Configuring "Display" Layer:

	pos.x = 7;
	pos.y = 11;
	size.cx = 230;
	size.cy = 65;

	display_group1 = new GroupBox("Time format", pos, size);

	pos.x = 245;
	size.cx = 121;

	display_group2 = new GroupBox("Timezone", pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 100;
	size.cy = 0;

	display_option1 = new OptionBox("Internet Beats", pos, size, &timeformat, 0);
	display_option1->onClick.Connect(&BeatClock::OptionsBeats, this);

	pos.y = 49;

	display_option2 = new OptionBox("Standard (STF)", pos, size, &timeformat, 1);
	display_option2->onClick.Connect(&BeatClock::OptionsSTF, this);

	pos.x = 126;
	pos.y = 24;

	if (timeformat == 0)	display_check1 = new CheckBox("Show centibeats", pos, size, &centi);
	else			display_check1 = new CheckBox("Show seconds", pos, size, &centi);

	display_check1->onClick.Connect(&BeatClock::OptionsPaint, this);

	pos.x = 255;

	display_option3 = new OptionBox("CET", pos, size, &timezone, 0);
	display_option3->onClick.Connect(&BeatClock::OptionsPaint, this);

	pos.y = 49;

	display_option4 = new OptionBox("Local time", pos, size, &timezone, 1);
	display_option4->onClick.Connect(&BeatClock::OptionsPaint, this);

	display->RegisterObject(display_group1);
	display->RegisterObject(display_group2);
	display->RegisterObject(display_option1);
	display->RegisterObject(display_option2);
	display->RegisterObject(display_option3);
	display->RegisterObject(display_option4);
	display->RegisterObject(display_check1);

// Configuring "Alarm" layer:

	pos.x = 7;
	pos.y = 7;

	alarm_check1 = new CheckBox("Enable alarm", pos, size, &isalarm);
	alarm_check1->onClick.Connect(&BeatClock::toggleAlarmState, this);

	pos.y = 35;
	pos.x = 9;

	alarm_text1 = new Text("Ring at:", pos);

	pos.x = 53;
	pos.y = 32;
	size.cx = 34;

	if (timeformat == 0)	alarmtext = String::FromInt(alarmbeats);
	else			alarmtext = convertSecondsToTimeString(alarmsecs);

	alarm_edit1 = new EditBox(alarmtext, pos, size, EDB_ALPHANUMERIC, 5);

	pos.y = 35;
	pos.x = 94;

	if (timeformat == 0)	alarm_text2 = new Text("Internet Beats", pos);
	else			alarm_text2 = new Text("Hours/Minutes", pos);

	pos.y = 21;
	pos.x = 172;
	size.cx = 0;

	alarm_option1 = new OptionBox("only once", pos, size, &alarmoption, 0);

	pos.y += 25;

	alarm_option2 = new OptionBox("every day", pos, size, &alarmoption, 1);

	alarm->RegisterObject(alarm_check1);
	alarm->RegisterObject(alarm_text1);
	alarm->RegisterObject(alarm_text2);
	alarm->RegisterObject(alarm_edit1);
	alarm->RegisterObject(alarm_option1);
	alarm->RegisterObject(alarm_option2);

	toggleAlarmState();

// Configuring "Misc" layer:

	pos.x = 7;
	pos.y = 11;
	size.cx = 359;
	size.cy = 65;

	misc_group1 = new GroupBox("'Mode' button action", pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 164;
	size.cy = 0;

	if (timeformat == 0)	misc_option1 = new OptionBox("Show/hide centibeats", pos, size, &modechange, 0);
	else			misc_option1 = new OptionBox("Show/hide seconds", pos, size, &modechange, 0);

	pos.y = 49;

	misc_option2 = new OptionBox("Change time format", pos, size, &modechange, 1);

	pos.x = 191;
	pos.y = 24;

	misc_option3 = new OptionBox("Change timezone", pos, size, &modechange, 2);

	pos.y = 49;

	misc_option4 = new OptionBox("Change format and timezone", pos, size, &modechange, 3);

	misc->RegisterObject(misc_group1);
	misc->RegisterObject(misc_option1);
	misc->RegisterObject(misc_option2);
	misc->RegisterObject(misc_option3);
	misc->RegisterObject(misc_option4);

// Configuring "Info" layer:

	pos.x = 6;
	pos.y = 5;

	info_text1 = new Text("BeatClock version 2.0\n\nDeveloped by Robert Kausch 2000-2003\nGive it to all your friends!", pos);

	pos.x = 222;

	info_text2 = new Text("\n\neMail: robert.kausch@gmx.net", pos);

	info->RegisterObject(info_text1);
	info->RegisterObject(info_text2);

// Configuring main layer:

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	main_button1 = new Button("OK", NIL, pos, size);
	main_button1->onClick.Connect(&BeatClock::OptionsOK, this);
	main_button1->SetOrientation(OR_LOWERRIGHT);

	pos.x = 87;

	main_button2 = new Button("Cancel", NIL, pos, size);
	main_button2->onClick.Connect(&BeatClock::OptionsCancel, this);
	main_button2->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 376;
	size.cy = 105;

	main_reg1 = new TabWidget(pos, size);

	RegisterObject(optionsdialog);

	optionsdialog->RegisterObject(optitle);
	optionsdialog->RegisterObject(main_button1);
	optionsdialog->RegisterObject(main_button2);
	optionsdialog->RegisterObject(main_reg1);

	main_reg1->RegisterObject(display);
	main_reg1->RegisterObject(misc);
	main_reg1->RegisterObject(alarm);
	main_reg1->RegisterObject(info);

	optionsdialog->SetIcon(SMOOTH::LoadImage("beat.pci", 0, NIL));
	optionsdialog->SetMetrics(Point(100, 100), Size(397, 181));
	optionsdialog->doQuit.Connect(&BeatClock::OptionsKillProc, this);

	oldtf = timeformat;
	oldct = centi;
	oldmc = modechange;
	oldtz = timezone;
	oldia = isalarm;
	oldab = alarmbeats;
	oldas = alarmsecs;
	oldat = alarmtext;
	oldao = alarmoption;

	optionsdialog->Stay();

	display->UnregisterObject(display_group1);
	display->UnregisterObject(display_group2);
	display->UnregisterObject(display_option1);
	display->UnregisterObject(display_option2);
	display->UnregisterObject(display_option3);
	display->UnregisterObject(display_option4);
	display->UnregisterObject(display_check1);

	alarm->UnregisterObject(alarm_check1);
	alarm->UnregisterObject(alarm_text1);
	alarm->UnregisterObject(alarm_text2);
	alarm->UnregisterObject(alarm_edit1);
	alarm->UnregisterObject(alarm_option1);
	alarm->UnregisterObject(alarm_option2);

	misc->UnregisterObject(misc_group1);
	misc->UnregisterObject(misc_option1);
	misc->UnregisterObject(misc_option2);
	misc->UnregisterObject(misc_option3);
	misc->UnregisterObject(misc_option4);

	info->UnregisterObject(info_text1);
	info->UnregisterObject(info_text2);

	main_reg1->UnregisterObject(display);
	main_reg1->UnregisterObject(alarm);
	main_reg1->UnregisterObject(misc);
	main_reg1->UnregisterObject(info);

	optionsdialog->UnregisterObject(optitle);
	optionsdialog->UnregisterObject(main_button1);
	optionsdialog->UnregisterObject(main_button2);
	optionsdialog->UnregisterObject(main_reg1);

	UnregisterObject(optionsdialog);

	delete optitle;
	delete optionsdialog;
	delete display;
	delete alarm;
	delete misc;
	delete info;
	delete main_button1;
	delete main_button2;
	delete main_reg1;
	delete info_text1;
	delete info_text2;
	delete alarm_check1;
	delete alarm_text1;
	delete alarm_text2;
	delete alarm_edit1;
	delete alarm_option1;
	delete alarm_option2;
	delete misc_group1;
	delete misc_option1;
	delete misc_option2;
	delete misc_option3;
	delete misc_option4;
	delete display_group1;
	delete display_group2;
	delete display_check1;
	delete display_option1;
	delete display_option2;
	delete display_option3;
	delete display_option4;
}

Void BeatClock::OptionsOK()
{
	if (timeformat == 0)
	{
		alarmbeats = min(999, alarm_edit1->GetText().ToInt());
		alarmsecs = convertBeatsToSeconds(alarmbeats);
	}
	else
	{
		alarmsecs = min(86340, convertTimeStringToSeconds(alarm_edit1->GetText()));
		alarmbeats = convertSecondsToBeats(alarmsecs);
	}

	optionsdialog->Close();
}

Void BeatClock::OptionsCancel()
{
	optionsdialog->Close();

	timeformat = oldtf;
	centi = oldct;
	modechange = oldmc;
	timezone = oldtz;
	isalarm = oldia;
	alarmbeats = oldab;
	alarmsecs = oldas;
	alarmtext = oldat;
	alarmoption = oldao;
}

Void BeatClock::OptionsBeats()
{
	display_check1->SetText("Show centibeats");
	misc_option1->SetText("Show/hide centibeats");
	alarm_text2->SetText("Internet Beats");
	alarm_edit1->SetText(String::FromInt(min(999, convertSecondsToBeats(convertTimeStringToSeconds(alarm_edit1->GetText())))));
}

Void BeatClock::OptionsSTF()
{
	display_check1->SetText("Show seconds");
	misc_option1->SetText("Show/hide seconds");
	alarm_text2->SetText("Hours/Minutes");
	alarm_edit1->SetText(convertSecondsToTimeString(min(86340, convertBeatsToSeconds(alarm_edit1->GetText().ToInt()))));
}

Void BeatClock::OptionsPaint()
{
	PaintAll();
}

Bool BeatClock::OptionsKillProc()
{
	return True;
}

Void BeatClock::toggleAlarmState()
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

Void BeatClock::Mode()
{
	if (modechange == 0)
	{
		switch (centi)
		{
			case true:
				centi = false;
				break;
			case false:
				centi = true;
				break;
		}
	}
	else if (modechange == 1)
	{
		switch (timeformat)
		{
			case 0:
				timeformat = 1;
				break;
			case 1:
				timeformat = 0;
				break;
		}
	}
	else if (modechange == 2)
	{
		switch (timezone)
		{
			case 0:
				timezone = 1;
				break;
			case 1:
				timezone = 0;
				break;
		}
	}
	else if (modechange == 3)
	{
		switch (timeformat)
		{
			case 0:
				timeformat = 1;
				break;
			case 1:
				timeformat = 0;
				break;
		}

		switch (timezone)
		{
			case 0:
				timezone = 1;
				break;
			case 1:
				timezone = 0;
				break;
		}
	}

	PaintAll();
}

Void BeatClock::PaintTime()
{
	wmpaint = false;
	PaintAll();
	wmpaint = true;
}

Void BeatClock::PaintAll()
{
	Surface			*surface = wnd->GetDrawSurface();

	surface->StartPaint(Rect(Point(10, 50), Size(wnd->GetObjectProperties()->size.cx - 20, 50)));

	HDC			 dc = ((SurfaceGDI *) surface)->GetContext();
	RECT			 textrect;
	SYSTEMTIME		 time;
	TIME_ZONE_INFORMATION	 tzi;
	double			 mseconds = 0;
	int			 first;
	int			 day;
	int			 beats = 0;
	int			 cbeats = 0;
	int			 ccbeats = 0;
	String			 btext = "@";
	String			 btext2 = "@";
	HBRUSH			 brush = CreateSolidBrush(Setup::BackgroundColor);
	HFONT			 hfont;
	HFONT			 holdfont;

	GetLocalTime(&time);
	GetTimeZoneInformation(&tzi);

	if (timezone == 1)
	{
		mseconds = time.wHour*3600000 + time.wMinute*60000 + time.wSecond*1000 + time.wMilliseconds;
	}
	else if (timezone == 0)
	{
		mseconds = time.wHour*3600000 + (time.wMinute+tzi.Bias+60)*60000 + time.wSecond*1000 + time.wMilliseconds;

		if (tzi.StandardDate.wYear == 0)
		{
			day = 1;
			first = GetDayOfWeek(1, tzi.StandardDate.wMonth, time.wYear);

			while (first != tzi.StandardDate.wDayOfWeek)
			{
				day++;
				first++;
				if (first == 7) first = 0;
			}

			for (int i = 1; i < tzi.StandardDate.wDay; i++)
			{
				day += 7;
			}

			if (OutOfMonth(day, tzi.StandardDate.wMonth, time.wYear)) day -= 7;

			tzi.StandardDate.wDay = day;
		}

		if (tzi.DaylightDate.wYear == 0)
		{
			day = 1;
			first = GetDayOfWeek(1, tzi.DaylightDate.wMonth, time.wYear);

			while (first != tzi.DaylightDate.wDayOfWeek)
			{
				day++;
				first++;
				if (first == 7) first = 0;
			}

			for (int i = 1; i < tzi.DaylightDate.wDay; i++)
			{
				day += 7;
			}

			if (OutOfMonth(day, tzi.DaylightDate.wMonth, time.wYear)) day -= 7;

			tzi.DaylightDate.wDay = day;
		}

		if (tzi.StandardDate.wMonth != 0 && tzi.DaylightDate.wMonth != 0)
		{
			if ((tzi.StandardDate.wMonth > tzi.DaylightDate.wMonth) && ((tzi.StandardDate.wMonth > time.wMonth && tzi.DaylightDate.wMonth < time.wMonth) || ((tzi.StandardDate.wMonth == time.wMonth && tzi.StandardDate.wDay > time.wDay) || (tzi.DaylightDate.wMonth == time.wMonth && tzi.DaylightDate.wDay < time.wDay)) || ((tzi.StandardDate.wMonth == time.wMonth && tzi.StandardDate.wDay == time.wDay && tzi.StandardDate.wHour > time.wHour) || (tzi.DaylightDate.wMonth == time.wMonth && tzi.DaylightDate.wDay == time.wDay && tzi.DaylightDate.wHour < time.wHour))))
			{
				mseconds = mseconds + tzi.DaylightBias * 60000;
			}
			else if ((tzi.StandardDate.wMonth < tzi.DaylightDate.wMonth) && !((tzi.StandardDate.wMonth > time.wMonth && tzi.DaylightDate.wMonth < time.wMonth) || ((tzi.StandardDate.wMonth == time.wMonth && tzi.StandardDate.wDay > time.wDay) || (tzi.DaylightDate.wMonth == time.wMonth && tzi.DaylightDate.wDay < time.wDay)) || ((tzi.StandardDate.wMonth == time.wMonth && tzi.StandardDate.wDay == time.wDay && tzi.StandardDate.wHour > time.wHour) || (tzi.DaylightDate.wMonth == time.wMonth && tzi.DaylightDate.wDay == time.wDay && tzi.DaylightDate.wHour < time.wHour))))
			{
				mseconds = mseconds + tzi.DaylightBias * 60000;
			}
		}

		while (mseconds > 86400000) mseconds = mseconds - 86400000;
		while (mseconds < 0) mseconds = mseconds + 86400000;
	}

	if (timeformat == 0)
	{
		beats = (int) (mseconds / 86400);
		cbeats = (int) (((double) mseconds / 86400 - (double) beats) * 100);
		ccbeats = 0;

		btext[1] = beats/100 + 48;
		btext[2] = beats%100/10 + 48;
		btext[3] = beats%100%10 + 48;
		btext[4] = 0;

		btext2 = btext;

		btext[4] = 58;
		btext[5] = 47;
		btext[6] = 47;

		btext[7] = cbeats/10 + 48;
		btext[8] = cbeats%10 + 48;
		btext[9] = 0;
	}
	else if (timeformat == 1)
	{
		beats = (int) (mseconds / 3600000);
		cbeats = (int) (((double) mseconds / 3600000 - (double) beats) * 60);
		ccbeats = (int) ((((double) mseconds / 3600000 - (double) beats) * 60 - (double) cbeats) * 60);

		btext[0] = beats/10 + 48;
		btext[1] = beats%10 + 48;
		btext[2] = 58;
		btext[3] = cbeats/10 + 48;
		btext[4] = cbeats%10 + 48;
		btext[5] = 0;

		btext2 = btext;

		btext[5] = 58;
		btext[6] = ccbeats/10 + 48;
		btext[7] = ccbeats%10 + 48;
		btext[8] = 0;
	}

	if (isalarm && timeformat == 0)
	{
		if (beats == alarmbeats && !alarmexec)
		{
			alarmexec = true;

			if (alarmoption == 0) isalarm = false;

			SMOOTH::MessageBox(String("It is @").Append(String::FromInt(beats)).Append(" internet beats!"), "BeatClock alarm", MB_OK, IDI_INFORMATION);
		}

		if (beats != alarmbeats && alarmexec)
		{
			alarmexec = false;
		}
	}
	else if (isalarm && timeformat == 1)
	{
		if (((int) (mseconds / 1000)) == alarmsecs && !alarmexec)
		{
			alarmexec = true;

			if (alarmoption == 0) isalarm = false;

			SMOOTH::MessageBox(String("The time is ").Append(convertSecondsToTimeString(alarmsecs)).Append("!"), "BeatClock alarm", MB_OK, IDI_INFORMATION);
		}

		if (((int) (mseconds / 1000)) != alarmsecs && alarmexec)
		{
			alarmexec = false;
		}
	}

	if (actbeats != beats || actcbeats != cbeats || actccbeats != ccbeats || wmpaint)
	{
		switch (centi)
		{
			case true:
				textrect.left = (int) ((16 * Setup::FontSize) + 0.5);
				textrect.top = (int) ((53 * Setup::FontSize) + 0.5);
				textrect.right = wnd->GetObjectProperties()->size.cx - (int) ((16 * Setup::FontSize) + 0.5);
				textrect.bottom = textrect.top + (int) ((40 * Setup::FontSize) + 0.5);
				FillRect(dc, &textrect, brush);
				if (timeformat == 1) textrect.left = (int) ((25 * Setup::FontSize) + 0.5);

				SetBkMode(dc, TRANSPARENT);
				SetTextColor(dc, RGB(0, 0, 0));

				if (Setup::enableUnicode)	hfont = CreateFontW(-MulDiv(21, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, String("Arial"));
				else					hfont = CreateFontA(-MulDiv(21, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, String("Arial"));

				holdfont = (HFONT) SelectObject(dc, hfont);

				if (Setup::enableUnicode)	DrawTextW(dc, btext, -1, &textrect, DT_LEFT);
				else					DrawTextA(dc, btext, -1, &textrect, DT_LEFT);

				SelectObject(dc, holdfont);
				::DeleteObject(hfont);

				actbeats = beats;
				actcbeats = cbeats;
				actccbeats = ccbeats;
				break;
			case false:
				if (actbeats != beats || (timeformat == 1 && actcbeats != cbeats) || wmpaint)
				{
					textrect.left = (int) ((16 * Setup::FontSize) + 0.5);
					textrect.top = (int) ((53 * Setup::FontSize) + 0.5);
					textrect.right = wnd->GetObjectProperties()->size.cx - (int) ((16 * Setup::FontSize) + 0.5);
					textrect.bottom = textrect.top + (int) ((40 * Setup::FontSize) + 0.5);
					FillRect(dc, &textrect, brush);
					textrect.left = (int) (44 * Setup::FontSize);
					if (timeformat == 1) textrect.left = (int) ((45 * Setup::FontSize) + 0.5);

					SetBkMode(dc, TRANSPARENT);
					SetTextColor(dc, RGB(0, 0, 0));

					if (Setup::enableUnicode)	hfont = CreateFontW(-MulDiv(21, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, String("Arial"));
					else					hfont = CreateFontA(-MulDiv(21, GetDeviceCaps(dc, LOGPIXELSY), 72), 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FF_ROMAN, String("Arial"));

					holdfont = (HFONT) SelectObject(dc, hfont);


					if (Setup::enableUnicode)	DrawTextW(dc, btext2, -1, &textrect, DT_LEFT);
					else					DrawTextA(dc, btext2, -1, &textrect, DT_LEFT);

					SelectObject(dc, holdfont);
					::DeleteObject(hfont);
				}
				actbeats = beats;
				actcbeats = cbeats;
				actccbeats = ccbeats;
				break;
		}
	}

	::DeleteObject(brush);

	surface->EndPaint();
}

Void BeatClock::Info()
{
	SMOOTH::MessageBox("BeatClock version 2.0\n\nDeveloped by Robert Kausch 2000-2003\nGive it to all your friends!\n\neMail: robert.kausch@gmx.net", "Info", MB_OK, IDI_ASTERISK);
}

Int BeatClock::GetDayOfWeek(Int day, Int month, Int year)
{
	month += 10;
	year = (month - 24) / 12 + year;
	day = (((13 * (month - (month / 13) * 12) - 1) / 5 + day + 77 + 5 * (year - (year / 100) * 100) / 4 + year / 400 - (year / 100) * 2) % 7);

	return day;
}

Bool BeatClock::OutOfMonth(Int day, Int month, Int year)
{
	if (day > 31) return true;
	else if (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) return true;
	else if (day > 29 && month == 2) return true;
	else if (day > 28 && month == 2 && (year%4 != 0 || (year%100 == 0 && year%400 != 0))) return true;
	else return false;
}

Int BeatClock::convertBeatsToSeconds(Int beats)
{
	return (int) (beats * 86.4);
}

Int BeatClock::convertSecondsToBeats(Int seconds)
{
	return (int) (seconds / 86.4);
}

String BeatClock::convertSecondsToTimeString(Int seconds)
{
	String	 rstring = "";

	if (((int) (seconds / 3600)) < 10) rstring.Append("0");

	rstring.Append(String::FromInt(seconds / 3600));
	rstring.Append(":");

	if (((int) ((seconds % 3600) / 60)) < 10) rstring.Append("0");

	rstring.Append(String::FromInt((seconds % 3600) / 60));

	return rstring;
}

Int BeatClock::convertTimeStringToSeconds(String time)
{
	String	 buffer;
	int	 counter = 0;
	int	 rval;

	buffer[0] = time[counter];
	counter++;

	if (time[counter] != ':')
	{
		buffer[1] = time[counter];
		counter++;
	}

	counter++;

	rval = buffer.ToInt() * 3600;

	buffer[0] = time[counter];
	counter++;

	if (time[counter] != ':')
	{
		buffer[1] = time[counter];
		counter++;
	}

	rval = rval + buffer.ToInt() * 60;

	return rval;
}

Void BeatClock::RegisterValues()
{
	HKEY	 beatclock;

	RegCreateKey(HKEY_LOCAL_MACHINE, _TEXT("Software\\BeatClock"), &beatclock);

	RegSetValueEx(beatclock, _TEXT("TimeFormat"), 0, REG_DWORD, (BYTE *) &timeformat, 4);
	RegSetValueEx(beatclock, _TEXT("TimeZone"), 0, REG_DWORD, (BYTE *) &timezone, 4);
	RegSetValueEx(beatclock, _TEXT("CentiBeats"), 0, REG_DWORD, (BYTE *) &centi, 4);
	RegSetValueEx(beatclock, _TEXT("ModeChange"), 0, REG_DWORD, (BYTE *) &modechange, 4);
	RegSetValueEx(beatclock, _TEXT("Alarm"), 0, REG_DWORD, (BYTE *) &isalarm, 4);
	RegSetValueEx(beatclock, _TEXT("AlarmBeats"), 0, REG_DWORD, (BYTE *) &alarmbeats, 4);
	RegSetValueEx(beatclock, _TEXT("AlarmSecs"), 0, REG_DWORD, (BYTE *) &alarmsecs, 4);
	RegSetValueEx(beatclock, _TEXT("AlarmOption"), 0, REG_DWORD, (BYTE *) &alarmoption, 4);
	RegSetValueEx(beatclock, _TEXT("WindowPosX"), 0, REG_DWORD, (BYTE *) &wpx, 4);
	RegSetValueEx(beatclock, _TEXT("WindowPosY"), 0, REG_DWORD, (BYTE *) &wpy, 4);

	RegCloseKey(beatclock);
}

Void BeatClock::InputValues()
{
	HKEY	beatclock;
	DWORD	size;
	DWORD	type;

	if (RegOpenKey(HKEY_LOCAL_MACHINE, _TEXT("Software\\BeatClock"), &beatclock) != ERROR_SUCCESS)
	{
		RegisterValues();
	}
	else
	{
		RegQueryValueEx(beatclock, _TEXT("TimeFormat"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("TimeFormat"), 0, &type, (BYTE *) &timeformat, &size);

		RegQueryValueEx(beatclock, _TEXT("TimeZone"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("TimeZone"), 0, &type, (BYTE *) &timezone, &size);

		RegQueryValueEx(beatclock, _TEXT("CentiBeats"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("CentiBeats"), 0, &type, (BYTE *) &centi, &size);

		RegQueryValueEx(beatclock, _TEXT("ModeChange"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("ModeChange"), 0, &type, (BYTE *) &modechange, &size);

		RegQueryValueEx(beatclock, _TEXT("Alarm"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("Alarm"), 0, &type, (BYTE *) &isalarm, &size);

		RegQueryValueEx(beatclock, _TEXT("AlarmBeats"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("AlarmBeats"), 0, &type, (BYTE *) &alarmbeats, &size);

		RegQueryValueEx(beatclock, _TEXT("AlarmSecs"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("AlarmSecs"), 0, &type, (BYTE *) &alarmsecs, &size);

		RegQueryValueEx(beatclock, _TEXT("AlarmOption"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("AlarmOption"), 0, &type, (BYTE *) &alarmoption, &size);

		RegQueryValueEx(beatclock, _TEXT("WindowPosX"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("WindowPosX"), 0, &type, (BYTE *) &wpx, &size);

		RegQueryValueEx(beatclock, _TEXT("WindowPosY"), 0, &type, 0, &size);
		RegQueryValueEx(beatclock, _TEXT("WindowPosY"), 0, &type, (BYTE *) &wpy, &size);

		RegCloseKey(beatclock);
	}
}
