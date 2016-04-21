 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/translator_internal.h>
#include <smooth/i18n/language.h>

#include <smooth/foreach.h>

#include <string.h>

#include "locales/smooth_af.h"
#include "locales/smooth_ar.h"
#include "locales/smooth_bg.h"
#include "locales/smooth_ca.h"
#include "locales/smooth_co.h"
#include "locales/smooth_cs.h"
#include "locales/smooth_da.h"
#include "locales/smooth_de.h"
#include "locales/smooth_el.h"
#include "locales/smooth_eo.h"
#include "locales/smooth_es.h"
#include "locales/smooth_et.h"
#include "locales/smooth_eu.h"
#include "locales/smooth_fa.h"
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
#include "locales/smooth_lv.h"
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
#include "locales/smooth_sl.h"
#include "locales/smooth_sr.h"
#include "locales/smooth_sr@Cyrl.h"
#include "locales/smooth_sv.h"
#include "locales/smooth_th.h"
#include "locales/smooth_tr.h"
#include "locales/smooth_uk.h"
#include "locales/smooth_va.h"
#include "locales/smooth_vi.h"
#include "locales/smooth_zh_CN.h"
#include "locales/smooth_zh_HK.h"
#include "locales/smooth_zh_TW.h"

namespace smooth
{
	namespace I18n
	{
		static const char	*languageXMLs[]   = { smooth_af, smooth_ar, smooth_bg, smooth_ca, smooth_co, smooth_cs, smooth_da, smooth_de, smooth_el, smooth_eo, smooth_es, smooth_et, smooth_eu, smooth_fa, smooth_fi, smooth_fr, smooth_gl, smooth_he, smooth_hi, smooth_hr, smooth_hu, smooth_id, smooth_it, smooth_ja, smooth_ko, smooth_lt, smooth_lv, smooth_ms, smooth_nl, smooth_no, smooth_pa, smooth_pl, smooth_pt, smooth_pt_BR, smooth_ro, smooth_ru, smooth_sk, smooth_sl, smooth_sr, smooth_sr_cyrl, smooth_sv, smooth_th, smooth_tr, smooth_uk, smooth_va, smooth_vi, smooth_zh_CN, smooth_zh_HK, smooth_zh_TW, NIL };
		static const char	*languageMagics[] = { "smooth_af.xml", "smooth_ar.xml", "smooth_bg.xml", "smooth_ca.xml", "smooth_co.xml", "smooth_cs.xml", "smooth_da.xml", "smooth_de.xml", "smooth_el.xml", "smooth_eo.xml", "smooth_es.xml", "smooth_et.xml", "smooth_eu.xml", "smooth_fa.xml", "smooth_fi.xml", "smooth_fr.xml", "smooth_gl.xml", "smooth_he.xml", "smooth_hi.xml", "smooth_hr.xml", "smooth_hu.xml", "smooth_id.xml", "smooth_it.xml", "smooth_ja.xml", "smooth_ko.xml", "smooth_lt.xml", "smooth_lv.xml", "smooth_ms.xml", "smooth_nl.xml", "smooth_no.xml", "smooth_pa.xml", "smooth_pl.xml", "smooth_pt.xml", "smooth_pt_BR.xml", "smooth_ro.xml", "smooth_ru.xml", "smooth_sk.xml", "smooth_sl.xml", "smooth_sr.xml", "smooth_sr@Cyrl.xml", "smooth_sv.xml", "smooth_th.xml", "smooth_tr.xml", "smooth_uk.xml", "smooth_va.xml", "smooth_vi.xml", "smooth_zh_CN.xml", "smooth_zh_HK.xml", "smooth_zh_TW.xml", NIL };
	};
};

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

	for (Int i = 0; languageXMLs[i] != NIL; i++)
	{
		const char	*xml = languageXMLs[i];
		XML::Document	*doc = new XML::Document();

		if (doc->ParseMemory((void *) xml, strlen(xml)) == Success())
		{
			Language	*language = new Language();

			language->magic = languageMagics[i];

			if (LoadDescription(doc, language) == Success()) AddLanguage(language);
		}

		delete doc;
	}

	return Success();
}

S::Int S::I18n::TranslatorInternal::ActivateLanguage(const String &magic)
{
	foreach (Language *language, languages)
	{
		if (language->magic != magic) continue;

		activeLanguage = language;

		/* Load actual language data.
		 */
		if (magic != "internal" && activeLanguage->strings.Length() == 0 && activeLanguage->sections.Length() == 0)
		{
			for (Int i = 0; languageMagics[i] != NIL; i++)
			{
				if (magic != languageMagics[i]) continue;

				XML::Document	*doc = new XML::Document();

				if (doc->ParseMemory((void *) languageXMLs[i], strlen(languageXMLs[i])) == Success()) LoadData(doc, activeLanguage);

				delete doc;
			}
		}

		return Success();
	}

	return Error();
}
