 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CATALAN_
#define __OBJSMOOTH_CATALAN_

#include <smooth/i18n/catalan.h>
#include <smooth/i18n.h>

void ActivateCatalanLanguage()
{
	TXT_ERROR = _TEXT("Error");

	TXT_OK = _TEXT("Bé");
	TXT_CANCEL = _TEXT("Cancel·la");
	TXT_YES = _TEXT("Sí");
	TXT_NO = _TEXT("No");
	TXT_RETRY = _TEXT("Torna a provar");
	TXT_ABORT = _TEXT("Aborta");
	TXT_IGNORE = _TEXT("Ignora-ho");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH aplicació");

	TXT_COLORSELECTION = _TEXT("Trieu color");

	TXT_OPENFILE = _TEXT("Obre fitxer");
	TXT_SAVEFILE = _TEXT("Desa fitxer");
	TXT_SAVEFILEAS = _TEXT("Desa fitxer com");

	TXT_SELECTDIR = _TEXT("Trieu directori:");
	TXT_SELECTFONT = _TEXT("Trieu font");
}

#endif
