 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ICELANDIC_
#define __OBJSMOOTH_ICELANDIC_

#include <smooth/i18n/icelandic.h>
#include <smooth/i18n.h>

void ActivateIcelandicLanguage()
{
	TXT_ERROR = _TEXT("Villa");

	TXT_OK = _TEXT("Í lagi");
	TXT_CANCEL = _TEXT("Hætta við");
	TXT_YES = _TEXT("Já");
	TXT_NO = _TEXT("Nei");
	TXT_RETRY = _TEXT("Reyna aftur");
	TXT_ABORT = _TEXT("Hætta við");
	TXT_IGNORE = _TEXT("Hundsa");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH Forrit");
	TXT_LAYER = _TEXT("Lag");

	TXT_COLORSELECTION = _TEXT("Veldu lit");

	TXT_OPENFILE = _TEXT("Opna skjal");
	TXT_SAVEFILE = _TEXT("Vista skjal");
	TXT_SAVEFILEAS = _TEXT("Vista skjal sem");

	TXT_SELECTDIR = _TEXT("Velja möppu:");
	TXT_SELECTFONT = _TEXT("Velja letur");
}

#endif
