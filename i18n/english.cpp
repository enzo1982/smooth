 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/english.h>
#include <smooth/i18n.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>

#ifdef __WIN32__
int CALLBACK EnumFontProcA(ENUMLOGFONTEXA *lpelfe, NEWTEXTMETRICEXA *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}

int CALLBACK EnumFontProcW(ENUMLOGFONTEXW *lpelfe, NEWTEXTMETRICEXW *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}
#endif

void S::ActivateEnglishLanguage()
{
	HDC		 dc = GetContext(0);

#ifdef __WIN32__
	LOGFONTA	 fontInfoA;
	LOGFONTW	 fontInfoW;

	fontInfoA.lfCharSet = DEFAULT_CHARSET;
	fontInfoA.lfFaceName[0] = 0;
	fontInfoA.lfPitchAndFamily = 0;

	fontInfoW.lfCharSet = DEFAULT_CHARSET;
	fontInfoW.lfFaceName[0] = 0;
	fontInfoW.lfPitchAndFamily = 0;

	int		 result;

	if (SMOOTH::Setup::enableUnicode)	result = EnumFontFamiliesExW(dc, &fontInfoW, (FONTENUMPROCW) &EnumFontProcW, 0, 0);
	else					result = EnumFontFamiliesExA(dc, &fontInfoA, (FONTENUMPROCA) &EnumFontProcA, 0, 0);

	if (result == 0)	I18N_DEFAULTFONT = "Microsoft Sans Serif";
	else			I18N_DEFAULTFONT = "MS Sans Serif";

#endif

	FreeContext(0, dc);

	I18N_DEFAULTFONTSIZE = 8;
	I18N_SMALLFONTSIZE = 8;

	String	 inputFormat = String::SetInputFormat("UTF-8");

	TXT_ERROR = "Error";
	TXT_ERROR_OBJECTMANAGEREXISTS = "ObjectManager already exists! Creation refused!";
	TXT_ERROR_THREADMANAGEREXISTS = "ThreadManager already exists! Creation refused!";
	TXT_ERROR_POPUPMANAGEREXISTS = "PopupManager already exists! Creation refused!";

	TXT_OK = "OK";
	TXT_CANCEL = "Cancel";
	TXT_YES = "Yes";
	TXT_NO = "No";
	TXT_RETRY = "Retry";
	TXT_ABORT = "Abort";
	TXT_IGNORE = "Ignore";

	TXT_SMOOTHAPPLICATION = "smooth application";
	TXT_LAYER = "Layer";

	TXT_COLORSELECTION = "Color selection";
	TXT_HTMLCODE = "HTML code:";
	TXT_REDSHORT = "R";
	TXT_GREENSHORT = "G";
	TXT_BLUESHORT = "B";
	TXT_HUESHORT = "H";
	TXT_SATURATIONSHORT = "S";
	TXT_VALUESHORT = "V";

	TXT_OPENFILE = "Open file";
	TXT_SAVEFILE = "Save file";
	TXT_SAVEFILEAS = "Save file as";

	TXT_SELECTDIR = "Select directory:";
	TXT_SELECTFONT = "Select font";

	TXT_SPLASHSCREEN = "Splash screen";

	TXT_SMOOTHBACKGROUNDAPPLICATION = "smooth Background Application";
	TXT_SMOOTHTOOLWINDOW = "smooth::ToolWindow";

	String::SetInputFormat(inputFormat);
}
