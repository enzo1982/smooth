 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SWEDISH_
#define __OBJSMOOTH_SWEDISH_

#include <smooth/i18n/swedish.h>
#include <smooth/i18n.h>

void ActivateSwedishLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Fel";

	TXT_OK = "OK";
	TXT_CANCEL = "Avbryt";
	TXT_YES = "Ja";
	TXT_NO = "Nej";
	TXT_RETRY = "Försök igen";
	TXT_ABORT = "Avbryt";
	TXT_IGNORE = "Ignorera";

	TXT_SMOOTHAPPLICATION = "SMOOTH program";
	TXT_LAYER = "Lager";

	TXT_COLORSELECTION = "Välj färg";
	TXT_HTMLCODE = "HTML-koden:";
	TXT_REDSHORT = "R";
	TXT_GREENSHORT = "G";
	TXT_BLUESHORT = "B";
	TXT_HUESHORT = "F";
	TXT_SATURATIONSHORT = "F";
	TXT_VALUESHORT = "V";

	TXT_OPENFILE = "Öppna fil";
	TXT_SAVEFILE = "Spara fil";
	TXT_SAVEFILEAS = "Spara fil som";

	TXT_SELECTDIR = "Välj katalog:";
	TXT_SELECTFONT = "Välj teckensnitt";

	TXT_SPLASHSCREEN = "Startskärm";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
