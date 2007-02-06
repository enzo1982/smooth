 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/splashscreen.h>
#include <smooth/i18n/i18n.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>

S::Int S::GUI::Dialogs::SplashScreen::nOfSplashScreens = 0;

S::GUI::Dialogs::SplashScreen::SplashScreen(const GUI::Bitmap &iBitmap, Int iTime)
{
	timer	= new System::Timer();
	time	= iTime;

	bitmap	= iBitmap;

	splashscreen = new Window("Splash screen", Point((LiSAGetDisplaySizeX() - (bitmap.GetSize().cx + 2)) / 2, (LiSAGetDisplaySizeY() - (bitmap.GetSize().cy + 2)) / 2 - 40), bitmap.GetSize() + Size(2, 2));

	splashscreen->onPaint.Connect(&SplashScreen::SplashPaintProc, this);
	splashscreen->doQuit.Connect(&SplashScreen::SplashKillProc, this);

	splashscreen->SetFlags(WF_NORESIZE | WF_TOPMOST);

	Add(splashscreen);

	timer->onInterval.Connect(&SplashScreen::TimerProc, this);
}

S::GUI::Dialogs::SplashScreen::~SplashScreen()
{
	DeleteObject(splashscreen);
	DeleteObject(timer);
}

const Error &S::GUI::Dialogs::SplashScreen::ShowDialog()
{
	nOfSplashScreens++;

	splashscreen->Show();

	timer->Start(time);

	splashscreen->Stay();

	return error;
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
	timer->Stop();

	nOfSplashScreens--;

	return True;
}

S::Void S::GUI::Dialogs::SplashScreen::TimerProc()
{
	splashscreen->Close();
}
