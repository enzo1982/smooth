 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_GREEK_
#define __OBJSMOOTH_GREEK_

#include <smooth/i18n/greek.h>
#include <smooth/i18n.h>

void ActivateGreekLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Σφάλμα";

	TXT_OK = "OK";
	TXT_CANCEL = "Ακύρωση";
	TXT_YES = "Ναι";
	TXT_NO = "Όχι";
	TXT_RETRY = "Ξαναπροσπάθησε";
	TXT_ABORT = "Διακοπή";
	TXT_IGNORE = "Αγνόησε";

	TXT_SMOOTHAPPLICATION = "SMOOTH εφαρμογή";

	TXT_COLORSELECTION = "Επιλέξτε Χρώμα";
	TXT_HTMLCODE = "HTML στις:";
	TXT_REDSHORT = "Κ";
	TXT_GREENSHORT = "Π";
	TXT_BLUESHORT = "Μ";
	TXT_HUESHORT = "Α";
	TXT_SATURATIONSHORT = "Κ";
	TXT_VALUESHORT = "Τ";

	TXT_OPENFILE = "Άνοιγμα ενός αρχείου";
	TXT_SAVEFILE = "Σώσιμο ενός αρχείου";
	TXT_SAVEFILEAS = "Αποθήκευση ως";

	TXT_SELECTDIR = "Επιλέξτε Κατάλογο:";
	TXT_SELECTFONT = "Επιλογή Γραμματοσειράς";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
