 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_WELSH_
#define __OBJSMOOTH_WELSH_

#include <smooth/i18n/welsh.h>
#include <smooth/i18n.h>

void ActivateWelshLanguage()
{
	TXT_ERROR = _TEXT("Camgymeriad");

	TXT_OK = _TEXT("Iawn");
	TXT_CANCEL = _TEXT("Canslo");
	TXT_YES = _TEXT("Ia");
	TXT_NO = _TEXT("Na");
	TXT_ABORT = _TEXT("Canslo");
	TXT_IGNORE = _TEXT("Anwybyddu");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH pecyn");

	TXT_COLORSELECTION = _TEXT("Dewis Lliw");

	TXT_OPENFILE = _TEXT("Agor ffeil");

	TXT_SELECTDIR = _TEXT("Dewisiwch Cyfeiriadur:");
	TXT_SELECTFONT = _TEXT("Dewis Ffont");
}

#endif
