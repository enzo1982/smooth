 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_I18N_
#define __OBJSMOOTH_I18N_

#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/i18n/afrikaans.h>
#include <smooth/i18n/basque.h>
#include <smooth/i18n/breton.h>
#include <smooth/i18n/bulgarian.h>
#include <smooth/i18n/catalan.h>
#include <smooth/i18n/chinese.h>
#include <smooth/i18n/chinese-simplified.h>
#include <smooth/i18n/croatian.h>
#include <smooth/i18n/czech.h>
#include <smooth/i18n/danish.h>
#include <smooth/i18n/dutch.h>
#include <smooth/i18n/english.h>
#include <smooth/i18n/esperanto.h>
#include <smooth/i18n/estonian.h>
#include <smooth/i18n/finnish.h>
#include <smooth/i18n/french.h>
#include <smooth/i18n/german.h>
#include <smooth/i18n/greek.h>
#include <smooth/i18n/hebrew.h>
#include <smooth/i18n/hungarian.h>
#include <smooth/i18n/icelandic.h>
#include <smooth/i18n/irish.h>
#include <smooth/i18n/italian.h>
#include <smooth/i18n/japanese.h>
#include <smooth/i18n/korean.h>
#include <smooth/i18n/latin.h>
#include <smooth/i18n/lithuanian.h>
#include <smooth/i18n/macedonian.h>
#include <smooth/i18n/maori.h>
#include <smooth/i18n/norwegian.h>
#include <smooth/i18n/polish.h>
#include <smooth/i18n/portuguese.h>
#include <smooth/i18n/portuguese-brazil.h>
#include <smooth/i18n/romanian.h>
#include <smooth/i18n/russian.h>
#include <smooth/i18n/serbian.h>
#include <smooth/i18n/slovak.h>
#include <smooth/i18n/slovenian.h>
#include <smooth/i18n/spanish.h>
#include <smooth/i18n/swedish.h>
#include <smooth/i18n/tamil.h>
#include <smooth/i18n/thai.h>
#include <smooth/i18n/turkish.h>
#include <smooth/i18n/ukrainian.h>
#include <smooth/i18n/walloon.h>
#include <smooth/i18n/welsh.h>
#include <smooth/i18n/galician.h>
#include <smooth/i18n/latvian.h>
#include <smooth/i18n/occitan.h>
#include <smooth/string.h>
#include <smooth/definitions.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

int SMOOTHDefaultLanguage = LNG_ENGLISH;

#ifdef __WIN32__
__declspec (dllexport)
#endif

int I18N_DEFAULTFONTSIZE;

#ifdef __WIN32__
__declspec (dllexport)
#endif

int I18N_SMALLFONTSIZE;

SMOOTHString I18N_DEFAULTFONT;

SMOOTHString TXT_ERROR;
SMOOTHString TXT_ERROR_OBJECTMANAGEREXISTS;
SMOOTHString TXT_ERROR_THREADMANAGEREXISTS;
SMOOTHString TXT_ERROR_POPUPMANAGEREXISTS;

SMOOTHString TXT_OK;
SMOOTHString TXT_CANCEL;
SMOOTHString TXT_YES;
SMOOTHString TXT_NO;
SMOOTHString TXT_RETRY;
SMOOTHString TXT_ABORT;
SMOOTHString TXT_IGNORE;

SMOOTHString TXT_SMOOTHAPPLICATION;
SMOOTHString TXT_LAYER;

SMOOTHString TXT_COLORSELECTION;
SMOOTHString TXT_HTMLCODE;
SMOOTHString TXT_REDSHORT;
SMOOTHString TXT_GREENSHORT;
SMOOTHString TXT_BLUESHORT;
SMOOTHString TXT_HUESHORT;
SMOOTHString TXT_SATURATIONSHORT;
SMOOTHString TXT_VALUESHORT;

SMOOTHString TXT_OPENFILE;
SMOOTHString TXT_SAVEFILE;
SMOOTHString TXT_SAVEFILEAS;

SMOOTHString TXT_SELECTDIR;
SMOOTHString TXT_SELECTFONT;

SMOOTHString TXT_SPLASHSCREEN;

SMOOTHString TXT_SMOOTHBACKGROUNDAPPLICATION;
SMOOTHString TXT_SMOOTHTOOLWINDOW;

int SMOOTHGetSystemLanguage();
void SMOOTHActivateLanguage(int);

int SMOOTHGetDefaultLanguage()
{
#ifdef __WIN32__
	switch (PRIMARYLANGID(GetUserDefaultLangID()))
	{
		case LANG_AFRIKAANS:
			SMOOTHDefaultLanguage = LNG_AFRIKAANS;
			break;
		case LANG_BASQUE:
			SMOOTHDefaultLanguage = LNG_BASQUE;
			break;
		case LANG_BULGARIAN:
			SMOOTHDefaultLanguage = LNG_BULGARIAN;
			break;
		case LANG_CATALAN:
			SMOOTHDefaultLanguage = LNG_CATALAN;
			break;
		case LANG_CHINESE:
#ifdef __WIN32__
			switch (SUBLANGID(GetUserDefaultLangID()))
			{
				default:
				case SUBLANG_CHINESE_TRADITIONAL:
					SMOOTHDefaultLanguage = LNG_CHINESE_TAIWAN;
					break;
				case SUBLANG_CHINESE_SIMPLIFIED:
					SMOOTHDefaultLanguage = LNG_CHINESE;
					break;
			}
#endif
			break;
		case LANG_CROATIAN:
			SMOOTHDefaultLanguage = LNG_CROATIAN;
			break;
		case LANG_CZECH:
			SMOOTHDefaultLanguage = LNG_CZECH;
			break;
		case LANG_DANISH:
			SMOOTHDefaultLanguage = LNG_DANISH;
			break;
		case LANG_DUTCH:
			SMOOTHDefaultLanguage = LNG_DUTCH;
			break;
		case LANG_ENGLISH:
			SMOOTHDefaultLanguage = LNG_ENGLISH;
			break;
		case LANG_ESTONIAN:
			SMOOTHDefaultLanguage = LNG_ESTONIAN;
			break;
		case LANG_FINNISH:
			SMOOTHDefaultLanguage = LNG_FINNISH;
			break;
		case LANG_FRENCH:
			SMOOTHDefaultLanguage = LNG_FRENCH;
			break;
		case LANG_GERMAN:
			SMOOTHDefaultLanguage = LNG_GERMAN;
			break;
		case LANG_GREEK:
			SMOOTHDefaultLanguage = LNG_GREEK;
			break;
		case LANG_HEBREW:
			SMOOTHDefaultLanguage = LNG_HEBREW;
			break;
		case LANG_HUNGARIAN:
			SMOOTHDefaultLanguage = LNG_HUNGARIAN;
			break;
		case LANG_ICELANDIC:
			SMOOTHDefaultLanguage = LNG_ICELANDIC;
			break;
		case LANG_ITALIAN:
			SMOOTHDefaultLanguage = LNG_ITALIAN;
			break;
		case LANG_JAPANESE:
			SMOOTHDefaultLanguage = LNG_JAPANESE;
			break;
		case LANG_KOREAN:
			SMOOTHDefaultLanguage = LNG_KOREAN;
			break;
		case LANG_LITHUANIAN:
			SMOOTHDefaultLanguage = LNG_LITHUANIAN;
			break;
		case LANG_MACEDONIAN:
			SMOOTHDefaultLanguage = LNG_MACEDONIAN;
			break;
		case LANG_NEUTRAL:
			SMOOTHDefaultLanguage = SMOOTHGetSystemLanguage();
			break;
		case LANG_NORWEGIAN:
			SMOOTHDefaultLanguage = LNG_NORWEGIAN;
			break;
		case LANG_POLISH:
			SMOOTHDefaultLanguage = LNG_POLISH;
			break;
		case LANG_PORTUGUESE:
#ifdef __WIN32__
			switch (SUBLANGID(GetUserDefaultLangID()))
			{
				default:
				case SUBLANG_PORTUGUESE:
					SMOOTHDefaultLanguage = LNG_PORTUGUESE;
					break;
				case SUBLANG_PORTUGUESE_BRAZILIAN:
					SMOOTHDefaultLanguage = LNG_PORTUGUESE_BRAZIL;
					break;
			}
#endif
			break;
		case LANG_ROMANIAN:
			SMOOTHDefaultLanguage = LNG_ROMANIAN;
			break;
		case LANG_RUSSIAN:
			SMOOTHDefaultLanguage = LNG_RUSSIAN;
			break;
		case LANG_SLOVAK:
			SMOOTHDefaultLanguage = LNG_SLOVAK;
			break;
		case LANG_SLOVENIAN:
			SMOOTHDefaultLanguage = LNG_SLOVENIAN;
			break;
		case LANG_SPANISH:
			SMOOTHDefaultLanguage = LNG_SPANISH;
			break;
		case LANG_SWEDISH:
			SMOOTHDefaultLanguage = LNG_SWEDISH;
			break;
		case LANG_TAMIL:
			SMOOTHDefaultLanguage = LNG_TAMIL;
			break;
		case LANG_THAI:
			SMOOTHDefaultLanguage = LNG_THAI;
			break;
		case LANG_TURKISH:
			SMOOTHDefaultLanguage = LNG_TURKISH;
			break;
		case LANG_UKRAINIAN:
			SMOOTHDefaultLanguage = LNG_UKRAINIAN;
			break;
		case LANG_LATVIAN:
			SMOOTHDefaultLanguage = LNG_LATVIAN;
			break;
		default:
			SMOOTHDefaultLanguage = LNG_ENGLISH;
			break;
	}
#endif

	return SMOOTHDefaultLanguage;
}

int SMOOTHGetSystemLanguage()
{
	int	 systemLanguage = LNG_ENGLISH;

#ifdef __WIN32__
	switch (PRIMARYLANGID(GetSystemDefaultLangID()))
	{
		case LANG_AFRIKAANS:
			systemLanguage = LNG_AFRIKAANS;
			break;
		case LANG_BASQUE:
			systemLanguage = LNG_BASQUE;
			break;
		case LANG_BULGARIAN:
			systemLanguage = LNG_BULGARIAN;
			break;
		case LANG_CATALAN:
			systemLanguage = LNG_CATALAN;
			break;
		case LANG_CHINESE:
#ifdef __WIN32__
			switch (SUBLANGID(GetSystemDefaultLangID()))
			{
				default:
				case SUBLANG_CHINESE_TRADITIONAL:
					systemLanguage = LNG_CHINESE_TAIWAN;
					break;
				case SUBLANG_CHINESE_SIMPLIFIED:
					systemLanguage = LNG_CHINESE;
					break;
			}
#endif
			break;
		case LANG_CROATIAN:
			systemLanguage = LNG_CROATIAN;
			break;
		case LANG_CZECH:
			systemLanguage = LNG_CZECH;
			break;
		case LANG_DANISH:
			systemLanguage = LNG_DANISH;
			break;
		case LANG_DUTCH:
			systemLanguage = LNG_DUTCH;
			break;
		case LANG_ENGLISH:
			systemLanguage = LNG_ENGLISH;
			break;
		case LANG_ESTONIAN:
			systemLanguage = LNG_ESTONIAN;
			break;
		case LANG_FINNISH:
			systemLanguage = LNG_FINNISH;
			break;
		case LANG_FRENCH:
			systemLanguage = LNG_FRENCH;
			break;
		case LANG_GERMAN:
			systemLanguage = LNG_GERMAN;
			break;
		case LANG_GREEK:
			systemLanguage = LNG_GREEK;
			break;
		case LANG_HEBREW:
			systemLanguage = LNG_HEBREW;
			break;
		case LANG_HUNGARIAN:
			systemLanguage = LNG_HUNGARIAN;
			break;
		case LANG_ICELANDIC:
			systemLanguage = LNG_ICELANDIC;
			break;
		case LANG_ITALIAN:
			systemLanguage = LNG_ITALIAN;
			break;
		case LANG_JAPANESE:
			systemLanguage = LNG_JAPANESE;
			break;
		case LANG_KOREAN:
			systemLanguage = LNG_KOREAN;
			break;
		case LANG_LITHUANIAN:
			systemLanguage = LNG_LITHUANIAN;
			break;
		case LANG_MACEDONIAN:
			systemLanguage = LNG_MACEDONIAN;
			break;
		case LANG_NEUTRAL:
			systemLanguage = LNG_ENGLISH;
			break;
		case LANG_NORWEGIAN:
			systemLanguage = LNG_NORWEGIAN;
			break;
		case LANG_POLISH:
			systemLanguage = LNG_POLISH;
			break;
		case LANG_PORTUGUESE:
#ifdef __WIN32__
			switch (SUBLANGID(GetSystemDefaultLangID()))
			{
				default:
				case SUBLANG_PORTUGUESE:
					systemLanguage = LNG_PORTUGUESE;
					break;
				case SUBLANG_PORTUGUESE_BRAZILIAN:
					systemLanguage = LNG_PORTUGUESE_BRAZIL;
					break;
			}
#endif
			break;
		case LANG_ROMANIAN:
			systemLanguage = LNG_ROMANIAN;
			break;
		case LANG_RUSSIAN:
			systemLanguage = LNG_RUSSIAN;
			break;
		case LANG_SLOVAK:
			systemLanguage = LNG_SLOVAK;
			break;
		case LANG_SLOVENIAN:
			systemLanguage = LNG_SLOVENIAN;
			break;
		case LANG_SPANISH:
			systemLanguage = LNG_SPANISH;
			break;
		case LANG_SWEDISH:
			systemLanguage = LNG_SWEDISH;
			break;
		case LANG_TAMIL:
			systemLanguage = LNG_TAMIL;
			break;
		case LANG_THAI:
			systemLanguage = LNG_THAI;
			break;
		case LANG_TURKISH:
			systemLanguage = LNG_TURKISH;
			break;
		case LANG_UKRAINIAN:
			systemLanguage = LNG_UKRAINIAN;
			break;
		case LANG_LATVIAN:
			systemLanguage = LNG_LATVIAN;
			break;
		default:
			systemLanguage = LNG_ENGLISH;
			break;
	}
#endif

	return systemLanguage;
}

void SMOOTHActivateLanguage(int language)
{
	switch (language)
	{
		case LNG_AFRIKAANS:
			ActivateAfrikaansLanguage();
			break;
		case LNG_BASQUE:
			ActivateBasqueLanguage();
			break;
		case LNG_BRETON:
			ActivateBretonLanguage();
			break;
		case LNG_BULGARIAN:
			ActivateBulgarianLanguage();
			break;
		case LNG_CATALAN:
			ActivateCatalanLanguage();
			break;
		case LNG_CHINESE:
			ActivateChineseLanguage();
			break;
		case LNG_CHINESE_TAIWAN:
			ActivateSimplifiedChineseLanguage();
			break;
		case LNG_CROATIAN:
			ActivateCroatianLanguage();
			break;
		case LNG_CZECH:
			ActivateCzechLanguage();
			break;
		case LNG_DANISH:
			ActivateDanishLanguage();
			break;
		case LNG_DUTCH:
			ActivateDutchLanguage();
			break;
		case LNG_ENGLISH:
			ActivateEnglishLanguage();
			break;
		case LNG_ESPERANTO:
			ActivateEsperantoLanguage();
			break;
		case LNG_ESTONIAN:
			ActivateEstonianLanguage();
			break;
		case LNG_FINNISH:
			ActivateFinnishLanguage();
			break;
		case LNG_FRENCH:
			ActivateFrenchLanguage();
			break;
		case LNG_GERMAN:
			ActivateGermanLanguage();
			break;
		case LNG_GREEK:
			ActivateGreekLanguage();
			break;
		case LNG_HEBREW:
			ActivateHebrewLanguage();
			break;
		case LNG_HUNGARIAN:
			ActivateHungarianLanguage();
			break;
		case LNG_ICELANDIC:
			ActivateIcelandicLanguage();
			break;
		case LNG_IRISH:
			ActivateIrishLanguage();
			break;
		case LNG_ITALIAN:
			ActivateItalianLanguage();
			break;
		case LNG_JAPANESE:
			ActivateJapaneseLanguage();
			break;
		case LNG_KOREAN:
			ActivateKoreanLanguage();
			break;
		case LNG_LATIN:
			ActivateLatinLanguage();
			break;
		case LNG_LITHUANIAN:
			ActivateLithuanianLanguage();
			break;
		case LNG_MACEDONIAN:
			ActivateMacedonianLanguage();
			break;
		case LNG_MAORI:
			ActivateMaoriLanguage();
			break;
		case LNG_NORWEGIAN:
			ActivateNorwegianLanguage();
			break;
		case LNG_POLISH:
			ActivatePolishLanguage();
			break;
		case LNG_PORTUGUESE:
			ActivatePortugueseLanguage();
			break;
		case LNG_PORTUGUESE_BRAZIL:
			ActivateBrazilPortugueseLanguage();
			break;
		case LNG_ROMANIAN:
			ActivateRomanianLanguage();
			break;
		case LNG_RUSSIAN:
			ActivateRussianLanguage();
			break;
		case LNG_SERBIAN:
			ActivateSerbianLanguage();
			break;
		case LNG_SLOVAK:
			ActivateSlovakLanguage();
			break;
		case LNG_SLOVENIAN:
			ActivateSlovenianLanguage();
			break;
		case LNG_SPANISH:
			ActivateSpanishLanguage();
			break;
		case LNG_SWEDISH:
			ActivateSwedishLanguage();
			break;
		case LNG_TAMIL:
			ActivateTamilLanguage();
			break;
		case LNG_THAI:
			ActivateThaiLanguage();
			break;
		case LNG_TURKISH:
			ActivateTurkishLanguage();
			break;
		case LNG_UKRAINIAN:
			ActivateUkrainianLanguage();
			break;
		case LNG_WALLOON:
			ActivateWalloonLanguage();
			break;
		case LNG_WELSH:
			ActivateWelshLanguage();
			break;
		case LNG_GALICIAN:
			ActivateGalicianLanguage();
			break;
		case LNG_LATVIAN:
			ActivateLatvianLanguage();
			break;
		case LNG_OCCITAN:
			ActivateOccitanLanguage();
			break;
		default:
			return;
	}
}

SMOOTHVoid SMOOTH::SetLanguage(SMOOTHInt language)
{
	SMOOTHActivateLanguage(LNG_ENGLISH);
	SMOOTHActivateLanguage(SMOOTHDefaultLanguage);
	SMOOTHActivateLanguage(language);
}

#endif
