 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_HEBREW_
#define __OBJSMOOTH_HEBREW_

#include <smooth/i18n/hebrew.h>
#include <smooth/i18n.h>

void ActivateHebrewLanguage()
{
	TXT_ERROR = _TEXT("האיגש");

	TXT_OK = _TEXT("רושיא");
	TXT_CANCEL = _TEXT("לוטיב");
	TXT_YES = _TEXT("ןכ");
	TXT_NO = _TEXT("אל");
	TXT_RETRY = _TEXT("תינש הסנ");
	TXT_ABORT = _TEXT("לוטיב");
	TXT_IGNORE = _TEXT("םלעתה");

	TXT_SMOOTHAPPLICATION = _TEXT("SMOOTH ־״־ע־®־ײ־ײ");

	TXT_COLORSELECTION = _TEXT("־ף־ז־× ־¿־ש־ז");

	TXT_OPENFILE = _TEXT("־ׁ־ז־ע־÷ ־ש־¬־ס");
	TXT_SAVEFILE = _TEXT("־ׁ־ז־ע־÷ ־¿־ע־׳־®");
	TXT_SAVEFILEAS = _TEXT("־״־®־ז ־ׁ־ז־ע־÷ ־¿־ע־׳־®");

	TXT_SELECTDIR = _TEXT("־צ־ײ־¿־ס־ם ־¿־ש־ז:");
	TXT_SELECTFONT = _TEXT("־ƒ־ס־ע־ֶ ־¿־ש־ז");
}

#endif
