 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BULGARIAN_
#define __OBJSMOOTH_BULGARIAN_

#include <smooth/i18n/bulgarian.h>
#include <smooth/i18n.h>

void ActivateBulgarianLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "ÃƒÃ°Ã¥Ã¸ÃªÃ ";

	TXT_OK = "Ã„Ã ";
	TXT_CANCEL = "ÃŽÃ²ÃªÃ Ã§";
	TXT_YES = "Ã„Ã ï¿½";
	TXT_NO = "ÃÃ¥ï¿½";
	TXT_ABORT = "ÃŽÃ²ÃªÃ Ã§";
	TXT_IGNORE = "ÃˆÃ£Ã­Ã®Ã°Ã¨Ã°Ã Ã©";

	TXT_SMOOTHAPPLICATION = "SMOOTH Ã¯Ã°Ã®Ã£Ã°Ã Ã¬Ã ";

	TXT_COLORSELECTION = "ÃˆÃ§Ã¡Ã¥Ã°Ã¨ Ã¶Ã¢Ã¿Ã²";
	TXT_REDSHORT = "Ã—";
	TXT_GREENSHORT = "Ã‡";
	TXT_BLUESHORT = "Ã‘";
	TXT_SATURATIONSHORT = "ÃŠ";

	TXT_OPENFILE = "ÃŽÃ²Ã¢Ã®Ã°Ã¨ Ã´Ã Ã©Ã«";
	TXT_SAVEFILE = "Ã‡Ã Ã¯Ã¨Ã± Ã­Ã  Ã´Ã Ã©Ã«";
	TXT_SAVEFILEAS = "Ã‡Ã Ã¯Ã¨Ã¸Ã¨ ÃªÃ Ã²Ã®";

	TXT_SELECTDIR = "ÃˆÃ§Ã¡Ã¥Ã°Ã¨ Ã¤Ã¥Ã°Ã¥ÃªÃ²Ã®Ã°Ã¨Ã¿ï¿½:";
	TXT_SPLASHSCREEN = "Ã‘Ã²Ã Ã°Ã²";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
