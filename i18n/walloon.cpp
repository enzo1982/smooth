 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_WALLOON_
#define __OBJSMOOTH_WALLOON_

#include <smooth/i18n/walloon.h>
#include <smooth/i18n.h>

void ActivateWalloonLanguage()
{
	TXT_ERROR = _TEXT("Aroke");

	TXT_OK = _TEXT("I va");
	TXT_CANCEL = _TEXT("Rinoncî");
	TXT_YES = _TEXT("Oyi");
	TXT_NO = _TEXT("Neni");
	TXT_RETRY = _TEXT("Rissayî");
	TXT_ABORT = _TEXT("Rinoncî");
	TXT_IGNORE = _TEXT("Passer");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH programe");

	TXT_COLORSELECTION = _TEXT("Tchwezi ene coleur");

	TXT_OPENFILE = _TEXT("Drovi on fitchî");
	TXT_SAVEFILE = _TEXT("Schaper li fitchî");
	TXT_SAVEFILEAS = _TEXT("Dji schape ...");

	TXT_SELECTDIR = _TEXT("Tchwezi ene ridant:");
	TXT_SELECTFONT = _TEXT("Tchwezi ene fonte");
}

#endif
