 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TURKISH_
#define __OBJSMOOTH_TURKISH_

#include <smooth/i18n/turkish.h>
#include <smooth/i18n.h>

void ActivateTurkishLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Hata";

	TXT_OK = "Tamam";
	TXT_CANCEL = "Ýptal";
	TXT_YES = "Evet";
	TXT_NO = "Hayýr";
	TXT_RETRY = "Yeniden dene";
	TXT_ABORT = "Ýptal";
	TXT_IGNORE = "Gözardý et";

	TXT_SMOOTHAPPLICATION = "SMOOTH uygulama";
	TXT_LAYER = "Katman";

	TXT_COLORSELECTION = "Renk seç";
	TXT_HTMLCODE = "HTML kodu";
	TXT_REDSHORT = "K";
	TXT_GREENSHORT = "Y";
	TXT_BLUESHORT = "M";
	TXT_HUESHORT = "R";
	TXT_SATURATIONSHORT = "D";
	TXT_VALUESHORT = "T";

	TXT_OPENFILE = "Dosya aç";
	TXT_SAVEFILE = "Dosya kaydet";
	TXT_SAVEFILEAS = "Dosyayý farklý kaydet";

	TXT_SELECTDIR = "Dizin seç:";
	TXT_SELECTFONT = "Yazýtipi seç";

	TXT_SPLASHSCREEN = "Baþlangýç ekraný";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
