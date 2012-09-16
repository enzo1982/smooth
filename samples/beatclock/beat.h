 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BEATCLOCK
#define H_BEATCLOCK

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::System;

class BeatClock : public Application
{
	private:
		GUI::Window	*wnd;

		Titlebar	*title;
		Menubar		*menubar;
		DragControl	*dragcontrol;

		Timer		*timer;

		Int		 actbeats;
		Int		 actcbeats;
		Int		 actccbeats;
		Int		 timeformat;
		Bool		 centi;
		Bool		 isalarm;
		Bool		 alarmexec;
		Int		 alarmbeats;
		Int		 alarmsecs;
		Int		 modechange;
		Int		 timezone;
		String		 alarmtext;
		Int		 alarmoption;

		Bool		 wmpaint;
		Int		 wpx;
		Int		 wpy;

		Void		 StoreConfig();
		Void		 ReadConfig();
	public:
				 BeatClock();
				~BeatClock();
	slots:
		Void		 ShowOptionsDialog();

		Void		 Mode();
		Void		 Info();

		Void		 PaintAll();
		Void		 PaintTime();
	slots:
		Void		 OnChangePosition(const Point &);
};

#endif
