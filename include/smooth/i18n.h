 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
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

namespace smooth
{
	SMOOTHVAR Int DefaultLanguage;

	SMOOTHVAR Int I18N_DEFAULTFONTSIZE;
	SMOOTHVAR Int I18N_SMALLFONTSIZE;
	SMOOTHVAR String I18N_DEFAULTFONT;

#ifdef __SMOOTH_DLL__
	Int GetDefaultLanguage();

	extern String TXT_OK;
	extern String TXT_CANCEL;
	extern String TXT_YES;
	extern String TXT_NO;
	extern String TXT_RETRY;
	extern String TXT_ABORT;
	extern String TXT_IGNORE;

	extern String TXT_SMOOTHAPPLICATION;

	extern String TXT_COLORSELECTION;
	extern String TXT_HTMLCODE;
	extern String TXT_REDSHORT;
	extern String TXT_GREENSHORT;
	extern String TXT_BLUESHORT;
	extern String TXT_HUESHORT;
	extern String TXT_SATURATIONSHORT;
	extern String TXT_VALUESHORT;

	extern String TXT_OPENFILE;
	extern String TXT_SAVEFILE;
	extern String TXT_SAVEFILEAS;

	extern String TXT_SELECTDIR;
	extern String TXT_SELECTFONT;

	extern String TXT_SPLASHSCREEN;

	extern String TXT_SMOOTHTOOLWINDOW;
#endif

	const Int LNG_ENGLISH	= 0;
	const Int LNG_GERMAN	= 1;
};

#endif
