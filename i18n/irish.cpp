 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_IRISH_
#define __OBJSMOOTH_IRISH_

#include <smooth/i18n/irish.h>
#include <smooth/i18n.h>

void ActivateIrishLanguage()
{
	TXT_ERROR = _TEXT("Earráid");

	TXT_OK = _TEXT("Ceart go leor");
	TXT_CANCEL = _TEXT("Cealaigh");
	TXT_YES = _TEXT("Is ea");
	TXT_NO = _TEXT("Ní hea");
	TXT_ABORT = _TEXT("Cealaigh");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH roghanna");

	TXT_OPENFILE = _TEXT("Foscail");
	TXT_SAVEFILE = _TEXT("Sábháil");
	TXT_SAVEFILEAS = _TEXT("Sábháil mar");
}

#endif
