 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CZECH_
#define __OBJSMOOTH_CZECH_

#include <smooth/i18n/czech.h>
#include <smooth/i18n.h>

void ActivateCzechLanguage()
{
	TXT_ERROR = _TEXT("Chyba");

	TXT_OK = _TEXT("OK");
	TXT_CANCEL = _TEXT("Zru¹it");
	TXT_YES = _TEXT("Ano");
	TXT_NO = _TEXT("Ne");
	TXT_RETRY = _TEXT("Zkusit znovu");
	TXT_ABORT = _TEXT("Pøeru¹it");
	TXT_IGNORE = _TEXT("Ignorovat");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH aplikace");
	TXT_LAYER = _TEXT("Vrstva");

	TXT_COLORSELECTION = _TEXT("Vyberte barvul");
	TXT_REDSHORT = _TEXT("È");
	TXT_GREENSHORT = _TEXT("Z");
	TXT_BLUESHORT = _TEXT("M");
	TXT_HUESHORT = _TEXT("T");
	TXT_SATURATIONSHORT = _TEXT("M");
	TXT_VALUESHORT = _TEXT("H");

	TXT_OPENFILE = _TEXT("Otevøít soubor");
	TXT_SAVEFILE = _TEXT("Ulo¾it soubor");
	TXT_SAVEFILEAS = _TEXT("Ulo¾it jako");

	TXT_SELECTDIR = _TEXT("Vybrat adresáø:");
	TXT_SELECTFONT = _TEXT("Vybrat písmo");
}

#endif
