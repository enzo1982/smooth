 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/i18n/english.h>
#include <smooth/i18n/german.h>
#include <smooth/string.h>
#include <smooth/definitions.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int S::DefaultLanguage = S::LNG_ENGLISH;

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int S::I18N_DEFAULTFONTSIZE;

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int S::I18N_SMALLFONTSIZE;

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::String S::I18N_DEFAULTFONT;

S::String S::TXT_OK;
S::String S::TXT_CANCEL;
S::String S::TXT_YES;
S::String S::TXT_NO;
S::String S::TXT_RETRY;
S::String S::TXT_ABORT;
S::String S::TXT_IGNORE;

S::String S::TXT_SMOOTHAPPLICATION;

S::String S::TXT_COLORSELECTION;
S::String S::TXT_HTMLCODE;
S::String S::TXT_REDSHORT;
S::String S::TXT_GREENSHORT;
S::String S::TXT_BLUESHORT;
S::String S::TXT_HUESHORT;
S::String S::TXT_SATURATIONSHORT;
S::String S::TXT_VALUESHORT;

S::String S::TXT_OPENFILE;
S::String S::TXT_SAVEFILE;
S::String S::TXT_SAVEFILEAS;

S::String S::TXT_SELECTDIR;
S::String S::TXT_SELECTFONT;

S::String S::TXT_SPLASHSCREEN;

S::String S::TXT_SMOOTHTOOLWINDOW;

S::Void ActivateLanguage(S::Int);

S::Int S::GetDefaultLanguage()
{
	switch (PRIMARYLANGID(GetUserDefaultLangID()))
	{
		default:
		case LANG_ENGLISH:
			DefaultLanguage = LNG_ENGLISH;
			break;
		case LANG_GERMAN:
			DefaultLanguage = LNG_GERMAN;
			break;
	}

	return DefaultLanguage;
}

S::Void ActivateLanguage(S::Int language)
{
	switch (language)
	{
		case S::LNG_ENGLISH:
			S::ActivateEnglishLanguage();
			break;
		case S::LNG_GERMAN:
			S::ActivateGermanLanguage();
			break;
		default:
			return;
	}
}

S::Void S::SMOOTH::SetLanguage(Int language)
{
	ActivateLanguage(LNG_ENGLISH);
	ActivateLanguage(DefaultLanguage);
	ActivateLanguage(language);
}
