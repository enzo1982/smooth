 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/splashscreen.h>
#include <smooth/i18n/translator.h>
#include <smooth/system/timer.h>
#include <smooth/system/screen.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>

S::GUI::Dialogs::SplashScreen::SplashScreen(const GUI::Bitmap &iBitmap, Int iTime)
{
	timer	= new System::Timer();
	time	= iTime;

	bitmap	= iBitmap;

	Rect	 workArea    = System::Screen::GetActiveScreenWorkArea();
	Float	 scaleFactor = Surface().GetSurfaceDPI() / 96.0;

	Size	 size = bitmap.GetSize();

	splashscreen = new Window("Splash screen", Point(), size + Size(2, 2));

	splashscreen->SetPosition(workArea.GetPosition() + Point((workArea.GetWidth()  - Math::Round((size.cx + 2) * scaleFactor)) / 2,
								 (workArea.GetHeight() - Math::Round((size.cy + 2) * scaleFactor)) / 2));

	splashscreen->onPaint.Connect(&SplashScreen::SplashPaintProc, this);
	splashscreen->doClose.Connect(&SplashScreen::SplashKillProc, this);

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
	splashscreen->Show();

	timer->Start(time);

	splashscreen->WaitUntilClosed();

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

	return True;
}

S::Void S::GUI::Dialogs::SplashScreen::TimerProc()
{
	splashscreen->Close();
}
