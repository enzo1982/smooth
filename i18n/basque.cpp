 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BASQUE_
#define __OBJSMOOTH_BASQUE_

#include <smooth/i18n/basque.h>
#include <smooth/i18n.h>

void ActivateBasqueLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Okerra";

	TXT_OK = "Ados";
	TXT_CANCEL = "Ezeztatu";
	TXT_YES = "Bai";
	TXT_NO = "Ez";
	TXT_RETRY = "Berriz saiatu";
	TXT_ABORT = "Ezeztatu";
	TXT_IGNORE = "Ezjakin";

	TXT_SMOOTHAPPLICATION = "SMOOTH aplikazioa";
	TXT_LAYER = "Layer";

	TXT_COLORSELECTION = "Kolorea aukeratu";

	TXT_OPENFILE = "Fitxategia ireki";
	TXT_SAVEFILE = "Gorde Fitxategia";
	TXT_SAVEFILEAS = "Gorde Fitxategia honela";

	TXT_SELECTDIR = "Direktorio berria:";
	TXT_SELECTFONT = "Fuentea aukeratu";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);


}

#endif
