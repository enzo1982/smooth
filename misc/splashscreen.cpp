 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

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
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>

using namespace smooth::GUI;

S::Int S::SplashScreenApp::nOfSplashScreens = 0;

S::Int S::SMOOTH::SplashScreen(HBITMAP logo, Int time)
{
	Int		 rVal;
	SplashScreenApp	*app = new SplashScreenApp(logo, time);

	rVal = app->ShowSplashScreen();

	Object::DeleteObject(app);

	return rVal;
}

S::SplashScreenApp::SplashScreenApp(HBITMAP logo, Int t)
{
	splashscreen = new Window("Splash screen");

	timer = new Timer();

	time = t;

	bitmap = logo;

	splashscreen->GetObjectProperties()->size.cx = GetBitmapSizeX(bitmap) + 2;
	splashscreen->GetObjectProperties()->size.cy = GetBitmapSizeY(bitmap) + 2;

	RegisterObject(splashscreen);

	splashscreen->RegisterObject(timer);

	splashscreen->onPaint.Connect(&SplashScreenApp::SplashPaintProc, this);
	splashscreen->doQuit.Connect(&SplashScreenApp::SplashKillProc, this);

	splashscreen->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - splashscreen->GetObjectProperties()->size.cx) / 2;
	splashscreen->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - splashscreen->GetObjectProperties()->size.cy) / 2-40;

	splashscreen->SetFlags(WF_NORESIZE | WF_TOPMOST);

	timer->onInterval.Connect(&SplashScreenApp::TimerProc, this);
}

S::SplashScreenApp::~SplashScreenApp()
{
	splashscreen->UnregisterObject(timer);

	UnregisterObject(splashscreen);

	DeleteObject(splashscreen);
	DeleteObject(timer);
}

S::Int S::SplashScreenApp::ShowSplashScreen()
{
	int	 rval;

	nOfSplashScreens++;

	splashscreen->Show();

	timer->Start(time);

	splashscreen->value = 1;

	rval = splashscreen->Stay();

	return rval;
}

S::Void S::SplashScreenApp::SplashPaintProc()
{
	Surface	*surface = splashscreen->GetDrawSurface();
	Rect	 bmprect;

	bmprect.left = 1;
	bmprect.top = 1;
	bmprect.right = GetBitmapSizeX(bitmap);
	bmprect.bottom = GetBitmapSizeY(bitmap);

	surface->BlitFromBitmap(Bitmap(bitmap), Rect(Point(0, 0), Size(GetBitmapSizeX(bitmap), GetBitmapSizeY(bitmap))), bmprect);
}

S::Bool S::SplashScreenApp::SplashKillProc()
{
	if (splashscreen->value == 0) splashscreen->value = Success;

	timer->Stop();

	nOfSplashScreens--;

	return True;
}

S::Void S::SplashScreenApp::TimerProc()
{
	if (splashscreen->value == 1)
	{
		splashscreen->Close();
	}
	else
	{
		timer->Stop();
		timer->Start(10);
	}
}
