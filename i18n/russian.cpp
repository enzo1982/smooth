 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_RUSSIAN_
#define __OBJSMOOTH_RUSSIAN_

#include <smooth/i18n/russian.h>
#include <smooth/i18n.h>

void ActivateRussianLanguage()
{
	SMOOTHString::SetInputFormat(SIF_UTF8);

	TXT_ERROR = "Ошибка";

	TXT_OK = "ОК";
	TXT_CANCEL = "Отмена";
	TXT_YES = "Да";
	TXT_NO = "Нет";
	TXT_RETRY = "Повторить";
	TXT_ABORT = "Прервать";
	TXT_IGNORE = "Игнорировать";

	TXT_SMOOTHAPPLICATION = "SMOOTH приложение";
	TXT_LAYER = "Слой";

	TXT_COLORSELECTION = "Выбрать цвет";
	TXT_HTMLCODE = "HTML-кода:";
	TXT_REDSHORT = "К";
	TXT_GREENSHORT = "З";
	TXT_BLUESHORT = "С";
	TXT_HUESHORT = "О";
	TXT_SATURATIONSHORT = "Н";
	TXT_VALUESHORT = "З";

	TXT_OPENFILE = "Открыть файл";
	TXT_SAVEFILE = "Сохранить файл";
	TXT_SAVEFILEAS = "Сохранить как";

	TXT_SELECTDIR = "Выберите каталог:";
	TXT_SELECTFONT = "Выбрать шрифт";

	TXT_SPLASHSCREEN = "Заставка";

	SMOOTHString::SetInputFormat(SIF_PREVIOUS);
}

#endif
