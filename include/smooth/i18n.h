 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_I18N_
#define _H_OBJSMOOTH_I18N_

#include "string.h"

SMOOTHVAR int SMOOTHDefaultLanguage;

SMOOTHVAR int I18N_DEFAULTFONTSIZE;
SMOOTHVAR int I18N_SMALLFONTSIZE;

#ifdef __SMOOTH_DLL__
int SMOOTHGetDefaultLanguage();

extern SMOOTHString I18N_DEFAULTFONT;

extern SMOOTHString TXT_ERROR;
extern SMOOTHString TXT_ERROR_OBJECTMANAGEREXISTS;
extern SMOOTHString TXT_ERROR_THREADMANAGEREXISTS;
extern SMOOTHString TXT_ERROR_POPUPMANAGEREXISTS;

extern SMOOTHString TXT_OK;
extern SMOOTHString TXT_CANCEL;
extern SMOOTHString TXT_YES;
extern SMOOTHString TXT_NO;
extern SMOOTHString TXT_RETRY;
extern SMOOTHString TXT_ABORT;
extern SMOOTHString TXT_IGNORE;

extern SMOOTHString TXT_SMOOTHAPPLICATION;
extern SMOOTHString TXT_LAYER;

extern SMOOTHString TXT_COLORSELECTION;
extern SMOOTHString TXT_HTMLCODE;
extern SMOOTHString TXT_REDSHORT;
extern SMOOTHString TXT_GREENSHORT;
extern SMOOTHString TXT_BLUESHORT;
extern SMOOTHString TXT_HUESHORT;
extern SMOOTHString TXT_SATURATIONSHORT;
extern SMOOTHString TXT_VALUESHORT;

extern SMOOTHString TXT_OPENFILE;
extern SMOOTHString TXT_SAVEFILE;
extern SMOOTHString TXT_SAVEFILEAS;

extern SMOOTHString TXT_SELECTDIR;
extern SMOOTHString TXT_SELECTFONT;

extern SMOOTHString TXT_SPLASHSCREEN;

extern SMOOTHString TXT_SMOOTHBACKGROUNDAPPLICATION;
extern SMOOTHString TXT_SMOOTHTOOLWINDOW;
#endif

const int LNG_ENGLISH		= 0;
const int LNG_GERMAN		= 1;
const int LNG_FRENCH		= 2;
const int LNG_DUTCH		= 3;
const int LNG_AFRIKAANS		= 4;
const int LNG_ITALIAN		= 5;
const int LNG_SPANISH		= 6;
const int LNG_PORTUGUESE	= 7;
const int LNG_SWEDISH		= 8;
const int LNG_NORWEGIAN		= 9;
const int LNG_DANISH		= 10;
const int LNG_FINNISH		= 11;
const int LNG_RUSSIAN		= 12;
const int LNG_POLISH		= 13;
const int LNG_GREEK		= 14;
const int LNG_BASQUE		= 15;
const int LNG_BRETON		= 16;
const int LNG_TURKISH		= 17;
const int LNG_HUNGARIAN		= 18;
const int LNG_BULGARIAN		= 19;
const int LNG_CZECH		= 20;
const int LNG_HEBREW		= 21;
const int LNG_JAPANESE		= 22;
const int LNG_CHINESE		= 23;
const int LNG_CATALAN		= 24;
const int LNG_CHINESE_TAIWAN	= 25;
const int LNG_CROATIAN		= 26;
const int LNG_ESPERANTO		= 27;
const int LNG_ESTONIAN		= 28;
const int LNG_ICELANDIC		= 29;
const int LNG_IRISH		= 30;
const int LNG_LATIN		= 31;
const int LNG_KOREAN		= 32;
const int LNG_MACEDONIAN	= 33;
const int LNG_PORTUGUESE_BRAZIL	= 34;
const int LNG_ROMANIAN		= 35;
const int LNG_SERBIAN		= 36;
const int LNG_SLOVAK		= 37;
const int LNG_SLOVENIAN		= 38;
const int LNG_THAI		= 39;
const int LNG_WALLOON		= 40;
const int LNG_MAORI		= 41;
const int LNG_LITHUANIAN	= 42;
const int LNG_TAMIL		= 43;
const int LNG_UKRAINIAN		= 44;
const int LNG_WELSH		= 45;
const int LNG_GALICIAN		= 46;
const int LNG_LATVIAN		= 47;
const int LNG_OCCITAN		= 48;

#endif
