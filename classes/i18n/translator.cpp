 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/translator.h>
#include <smooth/i18n/language.h>

#include <smooth/files/directory.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <stdlib.h>
#	include <unistd.h>
#endif

S::I18n::Translator	*S::I18n::Translator::defaultTranslator = NIL;

S::I18n::Translator::Translator(const String &iAppPrefix)
{
	appPrefix      = iAppPrefix.ToLower();
	activeLanguage = NIL;

	GetSupportedLanguages();

	ActivateLanguage("internal");
}

S::I18n::Translator::~Translator()
{
	for (int i = 0; i < languages.Length(); i++)
	{
		delete languages.GetNth(i);
	}

	languages.RemoveAll();
}

S::Int S::I18n::Translator::SetInternalLanguageInfo(const String &langName, const String &author, const String &url, Bool rightToLeft)
{
	Language	*iLang = NIL;

	for (Int n = 0; n < languages.Length(); n++)
	{
		if (languages.GetNth(n)->magic == "internal")
		{
			iLang = languages.GetNth(n);

			languages.Remove(languages.GetNthIndex(n));

			break;
		}
	}

	if (iLang != NIL)
	{
		iLang->SetName(langName);

		iLang->author = author;
		iLang->url = url;
		iLang->rightToLeft = rightToLeft;

		Bool	 done = false;

		for (Int i = 0; i <= languages.Length(); i++)
		{
			if (i == languages.Length())
			{
				languages.Add(iLang);

				break;
			}

			Language	*lang = languages.GetNth(i);

			for (Int j = 0; j < Math::Max(iLang->GetName().Length(), lang->GetName().Length()); j++)
			{
				if (iLang->GetName()[j] < lang->GetName()[j])
				{
					languages.InsertAtPos(i, iLang);

					done = True;

					break;
				}
				else if (iLang->GetName()[j] > lang->GetName()[j])
				{
					break;
				}
			}

			if (done) break;
		}
	}

	return Success();
}

S::Int S::I18n::Translator::SelectUserDefaultLanguage()
{
	String	 code;

#if defined __WIN32__

#  ifndef SUBLANG_CROATIAN_CROATIA
#    define SUBLANG_CROATIAN_CROATIA 0x01
#  endif

	switch (PRIMARYLANGID(GetUserDefaultLangID()))
	{
		case LANG_ENGLISH:	code = "en";	break;
		case LANG_ARABIC:	code = "ar";	break;
		case LANG_BASQUE:	code = "eu";	break;
		case LANG_BULGARIAN:	code = "bg";	break;
		case LANG_CATALAN:	code = "ca";	break;
		case LANG_CHINESE:	code = "zh_CN";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_SIMPLIFIED)	code = "zh_CN";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_TRADITIONAL)	code = "zh_TW";
							break;
		case LANG_CZECH:	code = "cs";	break;
		case LANG_DANISH:	code = "da";	break;
		case LANG_DUTCH:	code = "nl";	break;
		case LANG_ESTONIAN:	code = "et";	break;
		case LANG_FARSI:	code = "fa";	break;
		case LANG_FINNISH:	code = "fi";	break;
		case LANG_FRENCH:	code = "fr";	break;
		case LANG_GALICIAN:	code = "gl";	break;
		case LANG_GERMAN:	code = "de";	break;
		case LANG_GREEK:	code = "el";	break;
		case LANG_HEBREW:	code = "he";	break;
		case LANG_HINDI:	code = "hi";	break;
		case LANG_HUNGARIAN:	code = "hu";	break;
		case LANG_INDONESIAN:	code = "id";	break;
		case LANG_ITALIAN:	code = "it";	break;
		case LANG_JAPANESE:	code = "ja";	break;
		case LANG_KOREAN:	code = "ko";	break;
		case LANG_LITHUANIAN:	code = "lt";	break;
		case LANG_LATVIAN:	code = "lv";	break;
		case LANG_MALAY:	code = "ms";	break;
		case LANG_NORWEGIAN:	code = "no";	break;
		case LANG_POLISH:	code = "pl";	break;
		case LANG_PORTUGUESE:	code = "pt";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE)		code = "pt";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE_BRAZILIAN)	code = "pt_BR";
							break;
		case LANG_PUNJABI:	code = "pa";	break;
		case LANG_ROMANIAN:	code = "ro";	break;
		case LANG_RUSSIAN:	code = "ru";	break;
		case LANG_SERBIAN:	code = "sr";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CROATIAN_CROATIA)		code = "hr";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_SERBIAN_LATIN)		code = "sr";
							break;
		case LANG_SLOVAK:	code = "sk";	break;
		case LANG_SLOVENIAN:	code = "sl";	break;
		case LANG_SPANISH:	code = "es";	break;
		case LANG_SWEDISH:	code = "sv";	break;
		case LANG_THAI:		code = "th";	break;
		case LANG_TURKISH:	code = "tr";	break;
		case LANG_UKRAINIAN:	code = "uk";	break;
		case LANG_VIETNAMESE:	code = "vi";	break;
	}
#elif defined __HAIKU__
	FILE		*pstdin = popen("locale -l", "r");
	Buffer<char>	 buffer(256);

	buffer.Zero();

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	String	 lang = (char *) buffer;

	if (lang != NIL)
	{
		if (lang.Find("@") >= 0) lang = lang.Head(lang.Find("@"));
		if (lang.Find(".") >= 0) lang = lang.Head(lang.Find("."));

		code = lang;
	}
#else
	String	 lang = getenv("LANG");

	if (lang == NIL) lang = getenv("LC_MESSAGES");

	if (lang != NIL)
	{
		if (lang.Find("@") >= 0) lang = lang.Head(lang.Find("@"));
		if (lang.Find(".") >= 0) lang = lang.Head(lang.Find("."));

		code = lang;
	}
#endif

	/* Try the language code, possibly with appended country code.
	 */
	if (ActivateLanguage(String(appPrefix).Append("_").Append(code).Append(".xml")) == Success()) return Success();

	/* If failed, try the base language code if applicable.
	 */
	if (code.Find("_") >= 0)
	{
		if (ActivateLanguage(String(appPrefix).Append("_").Append(code.Head(code.Find("_"))).Append(".xml")) == Success()) return Success();
	}

	/* Fall back to internal language.
	 */
	ActivateLanguage("internal");

	return Success();
}

S::Int S::I18n::Translator::GetSupportedLanguages()
{
	{
		Language	*language = new Language();

		language->SetName("Default language");

		language->encoding    = "UTF-8";
		language->magic	      = "internal";
		language->author      = "unknown";
		language->url	      = "none";
		language->rightToLeft = False;

		languages.Add(language);
	}

	Directory		 dir(GUI::Application::GetApplicationDirectory().Append("lang").Append(Directory::GetDirectoryDelimiter()));

#ifndef __WIN32__
	if (Directory(GUI::Application::GetApplicationDirectory().Append("..").Append(Directory::GetDirectoryDelimiter()).Append("share").Append(Directory::GetDirectoryDelimiter()).Append(appPrefix).Append(Directory::GetDirectoryDelimiter()).Append("lang")).Exists())
	{
		dir = GUI::Application::GetApplicationDirectory().Append("..").Append(Directory::GetDirectoryDelimiter()).Append("share").Append(Directory::GetDirectoryDelimiter()).Append(appPrefix).Append(Directory::GetDirectoryDelimiter()).Append("lang").Append(Directory::GetDirectoryDelimiter());
	}
#endif

	const Array<File>	&files = dir.GetFilesByPattern(String(appPrefix).Append("_*.xml"));

	for (Int i = 0; i < files.Length(); i++)
	{
		XML::Document	*doc = new XML::Document();

		if (doc->LoadFile(files.GetNth(i)) == Success())
		{
			Language	*language = new Language();

			language->magic = files.GetNth(i).GetFileName();

			if (LoadDescription(doc, language) == Success()) AddLanguage(language);
		}

		delete doc;
	}

	return Success();
}

S::Bool S::I18n::Translator::SetContext(const String &nContext)
{
	return activeLanguage->SetContext(nContext);
}

const S::String &S::I18n::Translator::GetContext() const
{
	return activeLanguage->GetContext();
}

S::Int S::I18n::Translator::GetNOfLanguages() const
{
	return languages.Length();
}

const S::String &S::I18n::Translator::GetNthLanguageName(Int index) const
{
	return languages.GetNth(index)->GetName();
}

const S::String &S::I18n::Translator::GetNthLanguageID(Int index) const
{
	return languages.GetNth(index)->magic;
}

const S::String &S::I18n::Translator::GetNthLanguageAuthor(Int index) const
{
	return languages.GetNth(index)->author;
}

const S::String &S::I18n::Translator::GetNthLanguageEncoding(Int index) const
{
	return languages.GetNth(index)->encoding;
}

const S::String &S::I18n::Translator::GetNthLanguageURL(Int index) const
{
	return languages.GetNth(index)->url;
}

S::Bool S::I18n::Translator::IsNthLanguageRightToLeft(Int index) const
{
	return languages.GetNth(index)->rightToLeft;
}

const S::String &S::I18n::Translator::GetActiveLanguageName() const
{
	return activeLanguage->GetName();
}

const S::String &S::I18n::Translator::GetActiveLanguageID() const
{
	return activeLanguage->magic;
}

const S::String &S::I18n::Translator::GetActiveLanguageAuthor() const
{
	return activeLanguage->author;
}

const S::String &S::I18n::Translator::GetActiveLanguageEncoding() const
{
	return activeLanguage->encoding;
}

const S::String &S::I18n::Translator::GetActiveLanguageURL() const
{
	return activeLanguage->url;
}

S::Bool S::I18n::Translator::IsActiveLanguageRightToLeft() const
{
	return activeLanguage->rightToLeft;
}

S::Int S::I18n::Translator::ActivateLanguage(const String &magic)
{
	foreach (Language *language, languages)
	{
		if (language->magic == magic)
		{
			activeLanguage = language;

			/* Load actual language data.
			 */
			if (magic != "internal" && activeLanguage->strings.Length() == 0 && activeLanguage->sections.Length() == 0)
			{
				Directory	 dir(GUI::Application::GetApplicationDirectory().Append("lang").Append(Directory::GetDirectoryDelimiter()));

#ifndef __WIN32__
				if (Directory(GUI::Application::GetApplicationDirectory().Append("..").Append(Directory::GetDirectoryDelimiter()).Append("share").Append(Directory::GetDirectoryDelimiter()).Append(appPrefix).Append(Directory::GetDirectoryDelimiter()).Append("lang")).Exists())
				{
					dir = GUI::Application::GetApplicationDirectory().Append("..").Append(Directory::GetDirectoryDelimiter()).Append("share").Append(Directory::GetDirectoryDelimiter()).Append(appPrefix).Append(Directory::GetDirectoryDelimiter()).Append("lang").Append(Directory::GetDirectoryDelimiter());
				}
#endif

				String		 file = String(dir).Append(Directory::GetDirectoryDelimiter()).Append(magic);
				XML::Document	*doc  = new XML::Document();

				if (doc->LoadFile(file) == Success()) LoadData(doc, activeLanguage);

				delete doc;
			}

			return Success();
		}
	}

	return Error();
}

const S::String &S::I18n::Translator::TranslateString(const String &string, const String &context)
{
	String	prevContext = activeLanguage->GetContext();

	if (context != NIL) activeLanguage->SetContext(context);

	const String	&translation = activeLanguage->GetString(string);

	if (context != NIL) activeLanguage->SetContext(prevContext);

	if (translation == NIL)	return string;
	else			return translation;
}

S::Int S::I18n::Translator::AddLanguage(Language *language)
{
	/* Check arguments and return an error if they are not sane.
	 */
	if (language == NIL) return Error();

	Bool	 done = False;

	for (Int j = 0; j < languages.Length(); j++)
	{
		Language	*lang = languages.GetNth(j);

		for (Int k = 0; k < Math::Max(language->GetName().Length(), lang->GetName().Length()); k++)
		{
			if ((language->GetName()[k] < lang->GetName()[k] && !(language->GetName()[k] == '(' && lang->GetName()[k] == '/')) || (language->GetName()[k] == '/' && lang->GetName()[k] == '('))
			{
				languages.InsertAtPos(j, language);

				done = True;

				break;
			}
			else if ((language->GetName()[k] > lang->GetName()[k] && !(language->GetName()[k] == '/' && lang->GetName()[k] == '(')) || (language->GetName()[k] == '(' && lang->GetName()[k] == '/'))
			{
				break;
			}
		}

		if (j == languages.Length() - 1)
		{
			languages.Add(language);

			done = True;
		}

		if (done) break;
	}

	return Success();
}

S::Int S::I18n::Translator::LoadDescription(XML::Document *doc, Language *language)
{
	/* Check arguments and return an error if they are not sane.
	 */
	if (doc == NIL || doc->GetRootNode() == NIL || language == NIL) return Error();

	XML::Node	*root = doc->GetRootNode();
	Error		 result = language->ParseHeader(root);

	if (result != Success()) return Error();

	return Success();
}

S::Int S::I18n::Translator::LoadData(XML::Document *doc, Language *language)
{
	/* Check arguments and return an error if they are not sane.
	 */
	if (doc == NIL || doc->GetRootNode() == NIL || language == NIL) return Error();

	XML::Node	*root = doc->GetRootNode();
	Error		 result = language->Parse(root);

	if (result != Success()) return Error();

	return Success();
}
