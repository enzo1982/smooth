 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_BEAT_
#define _H_BEAT_

#include <smoothx.h>

class BeatClock : public SMOOTHApplication
{
	private:
		SMOOTHWindow		*wnd;
		SMOOTHTitlebar		*title;
		SMOOTHMenubar		*menubar;

		SMOOTHWindow		*optionsdialog;
		SMOOTHLayer		*mainlayer;
		SMOOTHLayer		*display;
		SMOOTHLayer		*alarm;
		SMOOTHLayer		*misc;
		SMOOTHLayer		*info;
		SMOOTHTitlebar		*optitle;
		SMOOTHDragControl	*dragcontrol;

		SMOOTHGroupBox 		*display_group1;
		SMOOTHGroupBox		*display_group2;
		SMOOTHOptionBox		*display_option1;
		SMOOTHOptionBox		*display_option2;
		SMOOTHCheckBox		*display_check1;
		SMOOTHOptionBox		*display_option3;
		SMOOTHOptionBox		*display_option4;

		SMOOTHCheckBox		*alarm_check1;
		SMOOTHText		*alarm_text1;
		SMOOTHEditBox		*alarm_edit1;
		SMOOTHText		*alarm_text2;
		SMOOTHOptionBox		*alarm_option1;
		SMOOTHOptionBox		*alarm_option2;

		SMOOTHGroupBox		*misc_group1;
		SMOOTHOptionBox		*misc_option1;
		SMOOTHOptionBox		*misc_option2;
		SMOOTHOptionBox		*misc_option3;
		SMOOTHOptionBox		*misc_option4;

		SMOOTHText		*info_text1;
		SMOOTHText		*info_text2;

		SMOOTHButton		*main_button1;
		SMOOTHButton		*main_button2;
		SMOOTHTabRegister	*main_reg1;

		SMOOTHTimer		*timer;

		SMOOTHInt		 actbeats;
		SMOOTHInt		 actcbeats;
		SMOOTHInt		 actccbeats;
		SMOOTHBool		 wmpaint;
		SMOOTHInt		 timeformat;
		SMOOTHBool		 centi;
		SMOOTHBool		 isalarm;
		SMOOTHBool		 alarmexec;
		SMOOTHInt		 alarmbeats;
		SMOOTHInt		 alarmsecs;
		SMOOTHInt		 wpx;
		SMOOTHInt		 wpy;
		SMOOTHInt		 modechange;
		SMOOTHInt		 timezone;
		SMOOTHInt		 centihandle1;
		SMOOTHInt		 centihandle2;
		SMOOTHString		 alarmtext;
		SMOOTHInt		 alarmoption;

		SMOOTHInt		 oldtf;
		SMOOTHBool		 oldct;
		SMOOTHInt		 oldmc;
		SMOOTHInt		 oldtz;
		SMOOTHBool		 oldia;
		SMOOTHInt		 oldab;
		SMOOTHInt		 oldas;
		SMOOTHString		 oldat;
		SMOOTHInt		 oldao;

		SMOOTHInt		 GetDayOfWeek(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHBool		 OutOfMonth(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHInt		 convertBeatsToSeconds(SMOOTHInt);
		SMOOTHInt		 convertSecondsToBeats(SMOOTHInt);
		SMOOTHString		 convertSecondsToTimeString(SMOOTHInt);
		SMOOTHInt		 convertTimeStringToSeconds(SMOOTHString);
		SMOOTHVoid		 RegisterValues();
		SMOOTHVoid		 InputValues();
	public:
					 BeatClock();
					~BeatClock();
		SMOOTHVoid		 MessageProc(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHVoid		 Options();
		SMOOTHVoid		 OptionsOK();
		SMOOTHVoid		 OptionsCancel();
		SMOOTHVoid		 OptionsBeats();
		SMOOTHVoid		 OptionsSTF();
		SMOOTHVoid		 OptionsPaint();
		SMOOTHBool		 OptionsKillProc();
		SMOOTHVoid		 toggleAlarmState();
		SMOOTHVoid		 Mode();
		SMOOTHVoid		 PaintAll();
		SMOOTHVoid		 PaintTime();
		SMOOTHVoid		 Info();
};

#endif
