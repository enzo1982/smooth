 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_POLISH_
#define __OBJSMOOTH_POLISH_

#include <smooth/i18n/polish.h>
#include <smooth/i18n.h>

void ActivatePolishLanguage()
{
	TXT_ERROR = _TEXT("B¶¶d");

	TXT_OK = _TEXT("Konfirmu");
	TXT_CANCEL = _TEXT("Anuluj");
	TXT_YES = _TEXT("Tak");
	TXT_NO = _TEXT("Nie");
	TXT_RETRY = _TEXT("Ponæw");
	TXT_ABORT = _TEXT("Przerwij");
	TXT_IGNORE = _TEXT("Ignoruj");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH aplikacja");

	TXT_COLORSELECTION = _TEXT("Wybierz kolor");
	TXT_HTMLCODE = _TEXT("Kod HTML:");
	TXT_REDSHORT = _TEXT("C");
	TXT_GREENSHORT = _TEXT("Z");
	TXT_BLUESHORT = _TEXT("N");
	TXT_HUESHORT = _TEXT("O");
	TXT_SATURATIONSHORT = _TEXT("N");
	TXT_VALUESHORT = _TEXT("W");

	TXT_OPENFILE = _TEXT("Otwærz plik");
	TXT_SAVEFILE = _TEXT("Zapisz plik");
	TXT_SAVEFILEAS = _TEXT("Zapisz plik jako");

	TXT_SELECTDIR = _TEXT("Wybierz katalog:");
	TXT_SELECTFONT = _TEXT("Wybierz czcionk€");
}

#endif
