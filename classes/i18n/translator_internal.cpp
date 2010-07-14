 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/translator_internal.h>
#include <smooth/i18n/language.h>

#include "locales/smooth_ar.h"
#include "locales/smooth_bg.h"
#include "locales/smooth_ca.h"
#include "locales/smooth_cs.h"
#include "locales/smooth_da.h"
#include "locales/smooth_de.h"
#include "locales/smooth_el.h"
#include "locales/smooth_es.h"
#include "locales/smooth_et.h"
#include "locales/smooth_fi.h"
#include "locales/smooth_fr.h"
#include "locales/smooth_gl.h"
#include "locales/smooth_he.h"
#include "locales/smooth_hi.h"
#include "locales/smooth_hr.h"
#include "locales/smooth_hu.h"
#include "locales/smooth_id.h"
#include "locales/smooth_it.h"
#include "locales/smooth_ja.h"
#include "locales/smooth_ko.h"
#include "locales/smooth_lt.h"
#include "locales/smooth_ms.h"
#include "locales/smooth_nl.h"
#include "locales/smooth_no.h"
#include "locales/smooth_pa.h"
#include "locales/smooth_pl.h"
#include "locales/smooth_pt.h"
#include "locales/smooth_pt_BR.h"
#include "locales/smooth_ro.h"
#include "locales/smooth_ru.h"
#include "locales/smooth_sk.h"
#include "locales/smooth_sr.h"
#include "locales/smooth_sv.h"
#include "locales/smooth_th.h"
#include "locales/smooth_tr.h"
#include "locales/smooth_uk.h"
#include "locales/smooth_vi.h"
#include "locales/smooth_zh_CN.h"
#include "locales/smooth_zh_TW.h"

S::I18n::TranslatorInternal::TranslatorInternal() : Translator("smooth")
{
	GetSupportedLanguages();
}

S::I18n::TranslatorInternal::~TranslatorInternal()
{
}


S::Int S::I18n::TranslatorInternal::GetSupportedLanguages()
{
	SetInternalLanguageInfo("English", "Robert Kausch", "http://www.smooth-project.org/", False);

	for (Int i = 0; i < 39; i++)
	{
		const char	*xml = NIL;

		if	(i ==  0) xml = smooth_ar;
		else if	(i ==  1) xml = smooth_bg;
		else if	(i ==  2) xml = smooth_ca;
		else if	(i ==  3) xml = smooth_cs;
		else if	(i ==  4) xml = smooth_da;
		else if	(i ==  5) xml = smooth_de;
		else if	(i ==  6) xml = smooth_el;
		else if	(i ==  7) xml = smooth_es;
		else if	(i ==  8) xml = smooth_et;
		else if	(i ==  9) xml = smooth_fi;
		else if	(i == 10) xml = smooth_fr;
		else if	(i == 11) xml = smooth_gl;
		else if	(i == 12) xml = smooth_he;
		else if	(i == 13) xml = smooth_hi;
		else if	(i == 14) xml = smooth_hr;
		else if	(i == 15) xml = smooth_hu;
		else if	(i == 16) xml = smooth_id;
		else if (i == 17) xml = smooth_it;
		else if (i == 18) xml = smooth_ja;
		else if	(i == 19) xml = smooth_ko;
		else if	(i == 20) xml = smooth_lt;
		else if	(i == 21) xml = smooth_ms;
		else if	(i == 22) xml = smooth_nl;
		else if	(i == 23) xml = smooth_no;
		else if	(i == 24) xml = smooth_pa;
		else if	(i == 25) xml = smooth_pl;
		else if	(i == 26) xml = smooth_pt;
		else if	(i == 27) xml = smooth_pt_BR;
		else if	(i == 28) xml = smooth_ro;
		else if (i == 29) xml = smooth_ru;
		else if	(i == 30) xml = smooth_sk;
		else if (i == 31) xml = smooth_sr;
		else if	(i == 32) xml = smooth_sv;
		else if	(i == 33) xml = smooth_th;
		else if	(i == 34) xml = smooth_tr;
		else if	(i == 35) xml = smooth_uk;
		else if (i == 36) xml = smooth_vi;
		else if (i == 37) xml = smooth_zh_CN;
		else if (i == 38) xml = smooth_zh_TW;

		XML::Document	*doc = new XML::Document();
		Language	*language = new Language();

		doc->ParseMemory((void *) xml, strlen(xml));

		if	(i ==  0) language->magic = "smooth_ar.xml";
		else if (i ==  1) language->magic = "smooth_bg.xml";
		else if (i ==  2) language->magic = "smooth_ca.xml";
		else if (i ==  3) language->magic = "smooth_cs.xml";
		else if (i ==  4) language->magic = "smooth_da.xml";
		else if (i ==  5) language->magic = "smooth_de.xml";
		else if (i ==  6) language->magic = "smooth_el.xml";
		else if (i ==  7) language->magic = "smooth_es.xml";
		else if (i ==  8) language->magic = "smooth_et.xml";
		else if (i ==  9) language->magic = "smooth_fi.xml";
		else if (i == 10) language->magic = "smooth_fr.xml";
		else if (i == 11) language->magic = "smooth_gl.xml";
		else if (i == 12) language->magic = "smooth_he.xml";
		else if (i == 13) language->magic = "smooth_hi.xml";
		else if (i == 14) language->magic = "smooth_hr.xml";
		else if (i == 15) language->magic = "smooth_hu.xml";
		else if (i == 16) language->magic = "smooth_id.xml";
		else if (i == 17) language->magic = "smooth_it.xml";
		else if (i == 18) language->magic = "smooth_ja.xml";
		else if (i == 19) language->magic = "smooth_ko.xml";
		else if (i == 20) language->magic = "smooth_lt.xml";
		else if (i == 21) language->magic = "smooth_ms.xml";
		else if (i == 22) language->magic = "smooth_nl.xml";
		else if (i == 23) language->magic = "smooth_no.xml";
		else if (i == 24) language->magic = "smooth_pa.xml";
		else if (i == 25) language->magic = "smooth_pl.xml";
		else if (i == 26) language->magic = "smooth_pt.xml";
		else if (i == 27) language->magic = "smooth_pt_BR.xml";
		else if (i == 28) language->magic = "smooth_ro.xml";
		else if (i == 29) language->magic = "smooth_ru.xml";
		else if (i == 30) language->magic = "smooth_sk.xml";
		else if (i == 31) language->magic = "smooth_sr.xml";
		else if (i == 32) language->magic = "smooth_sv.xml";
		else if (i == 33) language->magic = "smooth_th.xml";
		else if (i == 34) language->magic = "smooth_tr.xml";
		else if (i == 35) language->magic = "smooth_uk.xml";
		else if (i == 36) language->magic = "smooth_vi.xml";
		else if (i == 37) language->magic = "smooth_zh_CN.xml";
		else if (i == 38) language->magic = "smooth_zh_TW.xml";

		LoadDoc(doc, language);

		delete doc;
	}

	return Success();
}
