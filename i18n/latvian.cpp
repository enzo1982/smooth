 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LATVIAN_
#define __OBJSMOOTH_LATVIAN_

#include <smooth/i18n/latvian.h>
#include <smooth/i18n.h>

void ActivateLatvianLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Kļūda";

	TXT_OK = "OK";
	TXT_CANCEL = "Atcelt";
	TXT_YES = "Jā";
	TXT_NO = "Nē";
	TXT_RETRY = "Atkārtot";
	TXT_ABORT = "Atcelt";
	TXT_IGNORE = "Ignorēt";

	TXT_SMOOTHAPPLICATION = "SMOOTH aplikācija";

	TXT_COLORSELECTION = "Izvēlēties krāsu";
	TXT_HTMLCODE = "HTML kods:";
	TXT_REDSHORT = "R";
	TXT_GREENSHORT = "Z";
	TXT_BLUESHORT = "Z";
	TXT_HUESHORT = "N";
	TXT_SATURATIONSHORT = "P";
	TXT_VALUESHORT = "V";

	TXT_OPENFILE = "Atvērt failu";
	TXT_SAVEFILE = "Saglabāt failu";
	TXT_SAVEFILEAS = "Saglabāt Kā";

	TXT_SELECTDIR = "Izvēlēties direktoriju:";
	TXT_SELECTFONT = "Izvēlēties fontu";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
