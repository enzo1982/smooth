 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_HUNGARIAN_
#define __OBJSMOOTH_HUNGARIAN_

#include <smooth/i18n/hungarian.h>
#include <smooth/i18n.h>

void ActivateHungarianLanguage()
{
	TXT_ERROR = _TEXT("Hiba");

	TXT_OK = _TEXT("Vazhi");
	TXT_CANCEL = _TEXT("Mégsem");
	TXT_YES = _TEXT("Igen");
	TXT_NO = _TEXT("Nem");
	TXT_RETRY = _TEXT("Ismét");
	TXT_ABORT = _TEXT("Elvet");
	TXT_IGNORE = _TEXT("Kihagy");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH alkalmazás");
	TXT_LAYER = _TEXT("Fólia");

	TXT_COLORSELECTION = _TEXT("Színválasztás");
	TXT_HTMLCODE = _TEXT("HTML kód:");
	TXT_REDSHORT = _TEXT("V");
	TXT_GREENSHORT = _TEXT("Z");
	TXT_BLUESHORT = _TEXT("K");
	TXT_HUESHORT = _TEXT("Á");
	TXT_SATURATIONSHORT = _TEXT("T");
	TXT_VALUESHORT = _TEXT("É");

	TXT_OPENFILE = _TEXT("Fájl megnyitása");
	TXT_SAVEFILE = _TEXT("Fájl mentése");
	TXT_SAVEFILEAS = _TEXT("A fájl mentése másként");

	TXT_SELECTDIR = _TEXT("Könyvtár kiválasztása:");
	TXT_SELECTFONT = _TEXT("Betûtípusválasztás");

	TXT_SPLASHSCREEN = _TEXT("Üdvözlõ képernyõ");
}

#endif
