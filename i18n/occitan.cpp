 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_OCCITAN_
#define __OBJSMOOTH_OCCITAN_

#include <smooth/i18n/occitan.h>
#include <smooth/i18n.h>

void ActivateOccitanLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Error";

	TXT_OK = "Bé";
	TXT_CANCEL = "Cancel·la";
	TXT_YES = "Oc";
	TXT_NO = "Neni";
	TXT_IGNORE = "Inhora-ho";

	TXT_SMOOTHAPPLICATION = "SMOOTH aplicacion";

	TXT_COLORSELECTION = "Trieu color";
	TXT_HTMLCODE = "Codi HTML:";
	TXT_REDSHORT = "R";
	TXT_GREENSHORT = "V";
	TXT_BLUESHORT = "B";
	TXT_HUESHORT = "M";
	TXT_SATURATIONSHORT = "S";
	TXT_VALUESHORT = "V";

	TXT_OPENFILE = "Dobrir un fiquièr";
	TXT_SAVEFILE = "Salva un fiquièr";
	TXT_SAVEFILEAS = "Salva com";

	TXT_SELECTDIR = "Selectatz directori:";
	TXT_SELECTFONT = "Trieu font";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
