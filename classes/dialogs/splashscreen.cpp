 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dialogs/splashscreen.h>
#include <smooth/array.h>
#include <smooth/i18n.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/system/timer.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/window.h>

S::Int S::GUI::Dialogs::SplashScreen::nOfSplashScreens = 0;

S::GUI::Dialogs::SplashScreen::SplashScreen(const GUI::Bitmap &logo, Int t)
{
	splashscreen = new Window("Splash screen");

	timer = new System::Timer();

	time = t;

	bitmap = logo;

	splashscreen->GetObjectProperties()->size.cx = bitmap.GetSize().cx + 2;
	splashscreen->GetObjectProperties()->size.cy = bitmap.GetSize().cy + 2;

	RegisterObject(splashscreen);

	splashscreen->RegisterObject(timer);

	splashscreen->onPaint.Connect(&SplashScreen::SplashPaintProc, this);
	splashscreen->doQuit.Connect(&SplashScreen::SplashKillProc, this);

	splashscreen->GetObjectProperties()->pos.x = (LiSAGetDisplaySizeX() - splashscreen->GetObjectProperties()->size.cx) / 2;
	splashscreen->GetObjectProperties()->pos.y = (LiSAGetDisplaySizeY() - splashscreen->GetObjectProperties()->size.cy) / 2-40;

	splashscreen->SetFlags(WF_NORESIZE | WF_TOPMOST);

	timer->onInterval.Connect(&SplashScreen::TimerProc, this);
}

S::GUI::Dialogs::SplashScreen::~SplashScreen()
{
	splashscreen->UnregisterObject(timer);

	UnregisterObject(splashscreen);

	DeleteObject(splashscreen);
	DeleteObject(timer);
}

S::Int S::GUI::Dialogs::SplashScreen::ShowDialog()
{
	nOfSplashScreens++;

	splashscreen->Show();

	timer->Start(time);

	splashscreen->value = 1;

	return splashscreen->Stay();
}

S::Void S::GUI::Dialogs::SplashScreen::SplashPaintProc()
{
	Surface	*surface = splashscreen->GetDrawSurface();
	Rect	 bmpRect;

	bmpRect.left	= 1;
	bmpRect.top	= 1;
	bmpRect.right	= bmpRect.left + bitmap.GetSize().cx;
	bmpRect.bottom	= bmpRect.top + bitmap.GetSize().cy;

	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
}

S::Bool S::GUI::Dialogs::SplashScreen::SplashKillProc()
{
	if (splashscreen->value == 0) splashscreen->value = Success;

	timer->Stop();

	nOfSplashScreens--;

	return True;
}

S::Void S::GUI::Dialogs::SplashScreen::TimerProc()
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
