 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_AFRIKAANS_
#define __OBJSMOOTH_AFRIKAANS_

#include <smooth/i18n/afrikaans.h>
#include <smooth/i18n.h>

void ActivateAfrikaansLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Fout";

	TXT_OK = "OK";
	TXT_CANCEL = "Kanselleer";
	TXT_YES = "Ja";
	TXT_NO = "Nee";
	TXT_RETRY = "Herstel";
	TXT_ABORT = "Stop";
	TXT_IGNORE = "Ignoreer";

	TXT_SMOOTHAPPLICATION = "SMOOTH applikasie";

	TXT_COLORSELECTION = "Selekteer kleur";
	TXT_HTMLCODE = "HTML kode:";

	TXT_OPENFILE = "Maak 'n lêer oop";
	TXT_SAVEFILE = "Stoor 'n lêer";
	TXT_SAVEFILEAS = "Stoor 'n lêer as";

	TXT_SELECTDIR = "Selekteer lêergids:";
	TXT_SELECTFONT = "Selekteer lettertipe";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
