 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SPLASHSCREEN_
#define __OBJSMOOTH_SPLASHSCREEN_

#include <smooth/splashscreen.h>
#include <smooth/array.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/toolkit.h>
#include <smooth/button.h>
#include <smooth/titlebar.h>
#include <smooth/layer.h>
#include <smooth/metrics.h>
#include <smooth/timer.h>
#include <smooth/objectproperties.h>

int SMOOTHSplashScreenApp::nOfSplashScreens = 0;

SMOOTHInt SMOOTH::SplashScreen(HBITMAP logo, SMOOTHInt time)
{
	SMOOTHInt		 rVal;
	SMOOTHSplashScreenApp	*app = new SMOOTHSplashScreenApp(logo, time);

	rVal = app->ShowSplashScreen();

	SMOOTH::DeleteObject(app);

	return rVal;
}

SMOOTHSplashScreenApp::SMOOTHSplashScreenApp(HBITMAP logo, int t)
{
	splashscreen = new SMOOTHWindow(TXT_SPLASHSCREEN);

	timer = new SMOOTHTimer();

	time = t;

	bitmap = logo;

	splashscreen->GetObjectProperties()->size.cx = GetBitmapSizeX(bitmap) + 2;
	splashscreen->GetObjectProperties()->size.cy = GetBitmapSizeY(bitmap) + 2;

	RegisterObject(splashscreen);

	splashscreen->RegisterObject(timer);

	splashscreen->SetPaintProc(SMOOTHProc(SMOOTHSplashScreenApp, this, SplashPaintProc));
	splashscreen->SetKillProc(SMOOTHKillProc(SMOOTHSplashScreenApp, this, SplashKillProc));

	splashscreen->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - splashscreen->GetObjectProperties()->size.cx) / 2;
	splashscreen->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - splashscreen->GetObjectProperties()->size.cy) / 2-40;

	splashscreen->SetStyle(SS_NORESIZE);

#ifdef __WIN32__
	splashscreen->SetExStyle(WS_EX_TOPMOST);
#endif

	timer->SetProc(SMOOTHProc(SMOOTHSplashScreenApp, this, TimerProc));
}

SMOOTHSplashScreenApp::~SMOOTHSplashScreenApp()
{
	splashscreen->UnregisterObject(timer);

	UnregisterObject(splashscreen);

	SMOOTH::DeleteObject(splashscreen);
	SMOOTH::DeleteObject(timer);
}

int SMOOTHSplashScreenApp::ShowSplashScreen()
{
	int	 rval;

	nOfSplashScreens++;

	splashscreen->Show();

	timer->Start(time);

	splashscreen->value = 1;

	rval = splashscreen->Stay();

	return rval;
}

void SMOOTHSplashScreenApp::SplashPaintProc()
{
	HDC		 dc = GetContext(splashscreen);
	SMOOTHRect	 bmprect;

	bmprect.left = 1;
	bmprect.top = 1;
	bmprect.right = GetBitmapSizeX(bitmap);
	bmprect.bottom = GetBitmapSizeY(bitmap);

	PaintBitmap(dc, bmprect, bitmap);

	FreeContext(splashscreen, dc);
}

bool SMOOTHSplashScreenApp::SplashKillProc()
{
	if (splashscreen->value == 0) splashscreen->value = SMOOTH::Success;

	timer->Stop();

	nOfSplashScreens--;

	return true;
}

void SMOOTHSplashScreenApp::TimerProc()
{
	if (splashscreen->value == 1)
	{
		SMOOTH::CloseWindow(splashscreen);
	}
	else
	{
		timer->Stop();
		timer->Start(10);
	}
}

#endif
