 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_GALICIAN_
#define __OBJSMOOTH_GALICIAN_

#include <smooth/i18n/galician.h>
#include <smooth/i18n.h>

void ActivateGalicianLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Erro";

	TXT_OK = "Aceptar";
	TXT_CANCEL = "Cancelar";
	TXT_YES = "Si";
	TXT_NO = "Non";
	TXT_RETRY = "Retentar";
	TXT_ABORT = "Abortar";
	TXT_IGNORE = "Ignorar";

	TXT_SMOOTHAPPLICATION = "SMOOTH aplicación";

	TXT_COLORSELECTION = "Seleccionar cor";
	TXT_HTMLCODE = "Código HTML:";
	TXT_REDSHORT = "V";
	TXT_GREENSHORT = "V";
	TXT_BLUESHORT = "A";
	TXT_HUESHORT = "T";
	TXT_SATURATIONSHORT = "S";
	TXT_VALUESHORT = "V";

	TXT_OPENFILE = "Abrir un ficheiro";
	TXT_SAVEFILE = "Gardar un ficheiro";
	TXT_SAVEFILEAS = "Gardar como";

	TXT_SELECTDIR = "Seleccionar directorio:";
	TXT_SELECTFONT = "Seleccionar fonte";

	TXT_SPLASHSCREEN = "Pantalla inicial";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
