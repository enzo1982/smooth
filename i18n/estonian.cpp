 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ESTONIAN_
#define __OBJSMOOTH_ESTONIAN_

#include <smooth/i18n/estonian.h>
#include <smooth/i18n.h>

void ActivateEstonianLanguage()
{
	TXT_ERROR = _TEXT("Viga");

	TXT_OK = _TEXT("OK");
	TXT_CANCEL = _TEXT("Katkesta");
	TXT_YES = _TEXT("Jah");
	TXT_NO = _TEXT("Ei");
	TXT_RETRY = _TEXT("Proovi uuesti");
	TXT_ABORT = _TEXT("Katkesta");
	TXT_IGNORE = _TEXT("Ignoreeri");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH rakendus");
	TXT_LAYER = _TEXT("Kiht");

	TXT_COLORSELECTION = _TEXT("Vali värv");
	TXT_REDSHORT = _TEXT("P");
	TXT_GREENSHORT = _TEXT("R");
	TXT_BLUESHORT = _TEXT("S");
	TXT_HUESHORT = _TEXT("T");
	TXT_SATURATIONSHORT = _TEXT("K");
	TXT_VALUESHORT = _TEXT("V");

	TXT_OPENFILE = _TEXT("Ava fail");
	TXT_SAVEFILE = _TEXT("Salvesta fail");
	TXT_SAVEFILEAS = _TEXT("Salvesta fail kui");

	TXT_SELECTDIR = _TEXT("Vali kataloog:");
	TXT_SELECTFONT = _TEXT("Vali font");

	TXT_SPLASHSCREEN = _TEXT("Stardiekraan");
}

#endif
