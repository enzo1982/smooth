 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BRETON_
#define __OBJSMOOTH_BRETON_

#include <smooth/i18n/breton.h>
#include <smooth/i18n.h>

void ActivateBretonLanguage()
{
	TXT_ERROR = _TEXT("Fazi");

	TXT_OK = _TEXT("Mat eo");
	TXT_CANCEL = _TEXT("Nullañ");
	TXT_YES = _TEXT("Ya");
	TXT_NO = _TEXT("Nann");
	TXT_RETRY = _TEXT("Klask adarre");
	TXT_ABORT = _TEXT("Leuskel");
	TXT_IGNORE = _TEXT("Tremen e-biou");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH obererezh");

	TXT_COLORSELECTION = _TEXT("Diuzit ul liv");

	TXT_OPENFILE = _TEXT("Digeriñ ur restr");
	TXT_SAVEFILE = _TEXT("Enrollañ ar restr");
	TXT_SAVEFILEAS = _TEXT("Enrollañ ar restr e");

	TXT_SELECTDIR = _TEXT("Renkell nevez:");
	TXT_SELECTFONT = _TEXT("Diuzit un nodrezh");
}

#endif
