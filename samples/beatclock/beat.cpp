 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>

#include "beat.h"
#include "options.h"
#include "time.h"

using namespace smooth::GUI::Dialogs;

Int smooth::Main()
{
	BeatClock	*app = new BeatClock();

	app->Loop();

	delete app;

	return 0;
}

BeatClock::BeatClock() : Application("BeatClock")
{
	actbeats    = -1;
	actcbeats   = -1;
	actccbeats  = -1;
	timeformat  = 0;
	centi	    = True;
	isalarm	    = False;
	alarmexec   = False;
	alarmbeats  = 0;
	alarmsecs   = 0;
	alarmoption = 0;
	modechange  = 3;
	timezone    = 0;

	wmpaint	    = True;
	wpx	    = 16;
	wpy	    = 16;

	ReadConfig();

	wnd		= new GUI::Window("BeatClock", Point(wpx, wpy), Size(164, 103));
	title		= new Titlebar(TB_CLOSEBUTTON);
	menubar		= new Menubar();
	timer		= new Timer();
	dragcontrol	= new DragControl();

	timer->onInterval.Connect(&BeatClock::PaintTime, this);

	MenuEntry	*entry;

	menubar->AddEntry("Mode")->onAction.Connect(&BeatClock::Mode, this);
	menubar->AddEntry("Options")->onAction.Connect(&BeatClock::ShowOptionsDialog, this);
	menubar->AddEntry()->SetOrientation(OR_RIGHT);

	entry = menubar->AddEntry("Info");
	entry->onAction.Connect(&BeatClock::Info, this);
	entry->SetOrientation(OR_RIGHT);

	Add(wnd);

	wnd->Add(dragcontrol);
	wnd->Add(title);
	wnd->Add(menubar);

	wnd->SetIcon(ImageLoader::Load("beat.pci:0"));
	wnd->onPaint.Connect(&BeatClock::PaintAll, this);
	wnd->SetFlags(WF_TOPMOST | WF_NOTASKBUTTON | WF_NORESIZE);
	wnd->onChangePosition.Connect(&BeatClock::OnChangePosition, this);
	wnd->Show();

	timer->Start(50);
}

BeatClock::~BeatClock()
{
	timer->Stop();

	StoreConfig();

	DeleteObject(wnd);
	DeleteObject(title);
	DeleteObject(menubar);
	DeleteObject(timer);
	DeleteObject(dragcontrol);
}

Void BeatClock::OnChangePosition(const Point &windowPos)
{
	wpx = windowPos.x;
	wpy = windowPos.y;
}

Void BeatClock::ShowOptionsDialog()
{
	Options	 options(timeformat, centi, isalarm, alarmbeats, alarmsecs, modechange, timezone, alarmtext, alarmoption);

	options.onOptionChange.Connect(&BeatClock::PaintAll, this);

	options.ShowDialog();
}

Void BeatClock::Mode()
{
	if (modechange == 0)
	{
		if (centi) centi = False;
		else	   centi = True;
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
	wmpaint = False;

	PaintAll();

	wmpaint = True;
}

Void BeatClock::PaintAll()
{
	Surface			*surface = wnd->GetDrawSurface();

	surface->StartPaint(Rect(Point(10, 50) * surface->GetSurfaceDPI() / 96.0, Size(wnd->GetWidth() - 20, 50) * surface->GetSurfaceDPI() / 96.0));

#ifdef __WIN32__
	Rect			 textRect;
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
	Font			 font("Arial", 21, Font::Bold, Color(0, 0, 0));

	GetLocalTime(&time);
	GetTimeZoneInformation(&tzi);

	if (timezone == 1)
	{
		mseconds = time.wHour * 3600000 + time.wMinute * 60000 + time.wSecond * 1000 + time.wMilliseconds;
	}
	else if (timezone == 0)
	{
		mseconds = time.wHour * 3600000 + (time.wMinute + tzi.Bias + 60) * 60000 + time.wSecond * 1000 + time.wMilliseconds;

		if (tzi.StandardDate.wYear == 0)
		{
			day = 1;
			first = Time::GetDayOfWeek(1, tzi.StandardDate.wMonth, time.wYear);

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

			if (Time::OutOfMonth(day, tzi.StandardDate.wMonth, time.wYear)) day -= 7;

			tzi.StandardDate.wDay = day;
		}

		if (tzi.DaylightDate.wYear == 0)
		{
			day = 1;
			first = Time::GetDayOfWeek(1, tzi.DaylightDate.wMonth, time.wYear);

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

			if (Time::OutOfMonth(day, tzi.DaylightDate.wMonth, time.wYear)) day -= 7;

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

		btext[1] = beats / 100 + 48;
		btext[2] = beats % 100 / 10 + 48;
		btext[3] = beats % 100 % 10 + 48;
		btext[4] = 0;

		btext2 = btext;

		btext[4] = 58;
		btext[5] = 47;
		btext[6] = 47;

		btext[7] = cbeats / 10 + 48;
		btext[8] = cbeats % 10 + 48;
		btext[9] = 0;
	}
	else if (timeformat == 1)
	{
		beats = (int) (mseconds / 3600000);
		cbeats = (int) (((double) mseconds / 3600000 - (double) beats) * 60);
		ccbeats = (int) ((((double) mseconds / 3600000 - (double) beats) * 60 - (double) cbeats) * 60);

		btext[0] = beats / 10 + 48;
		btext[1] = beats % 10 + 48;
		btext[2] = 58;
		btext[3] = cbeats / 10 + 48;
		btext[4] = cbeats % 10 + 48;
		btext[5] = 0;

		btext2 = btext;

		btext[5] = 58;
		btext[6] = ccbeats / 10 + 48;
		btext[7] = ccbeats % 10 + 48;
		btext[8] = 0;
	}

	if (isalarm && timeformat == 0)
	{
		if (beats == alarmbeats && !alarmexec)
		{
			alarmexec = True;

			if (alarmoption == 0) isalarm = False;

			QuickMessage(String("It is @").Append(String::FromInt(beats)).Append(" internet beats!"), "BeatClock alarm", Message::Buttons::Ok, Message::Icon::Information);
		}

		if (beats != alarmbeats && alarmexec)
		{
			alarmexec = False;
		}
	}
	else if (isalarm && timeformat == 1)
	{
		if (((int) (mseconds / 1000)) == alarmsecs && !alarmexec)
		{
			alarmexec = True;

			if (alarmoption == 0) isalarm = False;

			QuickMessage(String("The time is ").Append(Time::SecondsToTimeString(alarmsecs)).Append("!"), "BeatClock alarm", Message::Buttons::Ok, Message::Icon::Information);
		}

		if (((int) (mseconds / 1000)) != alarmsecs && alarmexec)
		{
			alarmexec = False;
		}
	}

	if (actbeats != beats || actcbeats != cbeats || actccbeats != ccbeats || wmpaint)
	{
		textRect.left	= 16 * surface->GetSurfaceDPI() / 96.0;
		textRect.top	= 53 * surface->GetSurfaceDPI() / 96.0;
		textRect.right	= wnd->GetWidth() * surface->GetSurfaceDPI() / 96.0 - 16 * surface->GetSurfaceDPI() / 96.0;
		textRect.bottom = textRect.top + 40 * surface->GetSurfaceDPI() / 96.0;

		switch (centi)
		{
			case True:
				surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

				if (timeformat == 1) textRect.left = 25 * surface->GetSurfaceDPI() / 96.0;

				surface->SetText(btext, textRect, font);

				break;
			case False:
				if (actbeats != beats || (timeformat == 1 && actcbeats != cbeats) || wmpaint)
				{
					surface->Box(textRect, Setup::BackgroundColor, Rect::Filled);

					textRect.left = 44 * surface->GetSurfaceDPI() / 96.0;

					if (timeformat == 1) textRect.left = 45 * surface->GetSurfaceDPI() / 96.0;

					surface->SetText(btext2, textRect, font);
				}

				break;
		}

		actbeats = beats;
		actcbeats = cbeats;
		actccbeats = ccbeats;
	}
#endif
	surface->EndPaint();
}

Void BeatClock::Info()
{
	QuickMessage("BeatClock version 2.0\n\nDeveloped by Robert Kausch 2000-2019\nGive it to all your friends!\n\neMail: robert.kausch@gmx.net", "Info", Message::Buttons::Ok, Message::Icon::Information);
}

Void BeatClock::StoreConfig()
{
#ifdef __WIN32__
	HKEY	 beatclock;

	RegCreateKey(HKEY_LOCAL_MACHINE, L"Software\\BeatClock", &beatclock);

	RegSetValueEx(beatclock, L"TimeFormat", 0, REG_DWORD, (BYTE *) &timeformat, 4);
	RegSetValueEx(beatclock, L"TimeZone", 0, REG_DWORD, (BYTE *) &timezone, 4);
	RegSetValueEx(beatclock, L"CentiBeats", 0, REG_DWORD, (BYTE *) &centi, 4);
	RegSetValueEx(beatclock, L"ModeChange", 0, REG_DWORD, (BYTE *) &modechange, 4);
	RegSetValueEx(beatclock, L"Alarm", 0, REG_DWORD, (BYTE *) &isalarm, 4);
	RegSetValueEx(beatclock, L"AlarmBeats", 0, REG_DWORD, (BYTE *) &alarmbeats, 4);
	RegSetValueEx(beatclock, L"AlarmSecs", 0, REG_DWORD, (BYTE *) &alarmsecs, 4);
	RegSetValueEx(beatclock, L"AlarmOption", 0, REG_DWORD, (BYTE *) &alarmoption, 4);
	RegSetValueEx(beatclock, L"WindowPosX", 0, REG_DWORD, (BYTE *) &wpx, 4);
	RegSetValueEx(beatclock, L"WindowPosY", 0, REG_DWORD, (BYTE *) &wpy, 4);

	RegCloseKey(beatclock);
#endif
}

Void BeatClock::ReadConfig()
{
#ifdef __WIN32__
	HKEY	beatclock;
	DWORD	size;
	DWORD	type;

	if (RegOpenKey(HKEY_LOCAL_MACHINE, L"Software\\BeatClock", &beatclock) != ERROR_SUCCESS)
	{
		StoreConfig();
	}
	else
	{
		RegQueryValueEx(beatclock, L"TimeFormat", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"TimeFormat", 0, &type, (BYTE *) &timeformat, &size);

		RegQueryValueEx(beatclock, L"TimeZone", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"TimeZone", 0, &type, (BYTE *) &timezone, &size);

		RegQueryValueEx(beatclock, L"CentiBeats", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"CentiBeats", 0, &type, (BYTE *) &centi, &size);

		RegQueryValueEx(beatclock, L"ModeChange", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"ModeChange", 0, &type, (BYTE *) &modechange, &size);

		RegQueryValueEx(beatclock, L"Alarm", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"Alarm", 0, &type, (BYTE *) &isalarm, &size);

		RegQueryValueEx(beatclock, L"AlarmBeats", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"AlarmBeats", 0, &type, (BYTE *) &alarmbeats, &size);

		RegQueryValueEx(beatclock, L"AlarmSecs", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"AlarmSecs", 0, &type, (BYTE *) &alarmsecs, &size);

		RegQueryValueEx(beatclock, L"AlarmOption", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"AlarmOption", 0, &type, (BYTE *) &alarmoption, &size);

		RegQueryValueEx(beatclock, L"WindowPosX", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"WindowPosX", 0, &type, (BYTE *) &wpx, &size);

		RegQueryValueEx(beatclock, L"WindowPosY", 0, &type, 0, &size);
		RegQueryValueEx(beatclock, L"WindowPosY", 0, &type, (BYTE *) &wpy, &size);

		RegCloseKey(beatclock);
	}
#endif
}
