 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ESPERANTO_
#define __OBJSMOOTH_ESPERANTO_

#include <smooth/i18n/esperanto.h>
#include <smooth/i18n.h>

void ActivateEsperantoLanguage()
{
	TXT_ERROR = _TEXT("Eraro");

	TXT_OK = _TEXT("Konfirmu");
	TXT_CANCEL = _TEXT("Rezignu");
	TXT_YES = _TEXT("Jes");
	TXT_NO = _TEXT("Ne");
	TXT_RETRY = _TEXT("Reprovu");
	TXT_ABORT = _TEXT("Interrompu");
	TXT_IGNORE = _TEXT("Ignoru");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH aplikajo");

	TXT_COLORSELECTION = _TEXT("Elektu koloron");
	TXT_HTMLCODE = _TEXT("HTML-Kodon:");
	TXT_REDSHORT = _TEXT("R");
	TXT_GREENSHORT = _TEXT("V");
	TXT_BLUESHORT = _TEXT("B");
	TXT_HUESHORT = _TEXT("K");
	TXT_SATURATIONSHORT = _TEXT("I");
	TXT_VALUESHORT = _TEXT("H");

	TXT_OPENFILE = _TEXT("Malfermu dosieron");
	TXT_SAVEFILE = _TEXT("Sekurigu dosieron");
	TXT_SAVEFILEAS = _TEXT("Sekurigu dosieron kiel");

	TXT_SELECTDIR = _TEXT("Elektu dosierujon:");
	TXT_SELECTFONT = _TEXT("Elektu tiparon");
}

#endif
