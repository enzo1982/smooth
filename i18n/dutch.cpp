 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DUTCH_
#define __OBJSMOOTH_DUTCH_

#include <smooth/i18n/dutch.h>
#include <smooth/i18n.h>

void ActivateDutchLanguage()
{
	TXT_ERROR = _TEXT("Fout");

	TXT_OK = _TEXT("OK");
	TXT_CANCEL = _TEXT("Annuleren");
	TXT_YES = _TEXT("Ja");
	TXT_NO = _TEXT("Nee");
	TXT_RETRY = _TEXT("Herstel");
	TXT_ABORT = _TEXT("Breek af");
	TXT_IGNORE = _TEXT("Negeren");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH applicatie");
	TXT_LAYER = _TEXT("Laag");

	TXT_COLORSELECTION = _TEXT("Kleur selecteren");
	TXT_HTMLCODE = _TEXT("HTML-code:");
	TXT_REDSHORT = _TEXT("R");
	TXT_GREENSHORT = _TEXT("G");
	TXT_BLUESHORT = _TEXT("B");
	TXT_HUESHORT = _TEXT("T");
	TXT_SATURATIONSHORT = _TEXT("V");
	TXT_VALUESHORT = _TEXT("W");

	TXT_OPENFILE = _TEXT("Bestand openen");
	TXT_SAVEFILE = _TEXT("Bestand opslaan");
	TXT_SAVEFILEAS = _TEXT("Bestand opslaan als");

	TXT_SELECTDIR = _TEXT("Map selecteren:");
	TXT_SELECTFONT = _TEXT("Lettertype selecteren");

	TXT_SPLASHSCREEN = _TEXT("Plons scherm");
}

#endif
