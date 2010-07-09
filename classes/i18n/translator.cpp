 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

S::I18n::Translator	*S::I18n::Translator::defaultTranslator = NIL;

S::I18n::Translator::Translator(const String &iAppPrefix)
{
	appPrefix = iAppPrefix.ToLower();

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
		iLang->name = langName;
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

			for (Int j = 0; j < Math::Max(iLang->name.Length(), lang->name.Length()); j++)
			{
				if (iLang->name[j] < lang->name[j])
				{
					languages.InsertAtPos(i, iLang);

					done = True;

					break;
				}
				else if (iLang->name[j] > lang->name[j])
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
		case LANG_CATALAN:	code = "ca";	break;
		case LANG_CHINESE:	code = "zh_CN";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_SIMPLIFIED)	code = "zh_CN";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CHINESE_TRADITIONAL)	code = "zh_TW";
							break;
		case LANG_CZECH:	code = "cs";	break;
		case LANG_DANISH:	code = "da";	break;
		case LANG_DUTCH:	code = "nl";	break;
		case LANG_ESTONIAN:	code = "et";	break;
		case LANG_FINNISH:	code = "fi";	break;
		case LANG_FRENCH:	code = "fr";	break;
		case LANG_GALICIAN:	code = "gl";	break;
		case LANG_GERMAN:	code = "de";	break;
		case LANG_GREEK:	code = "el";	break;
		case LANG_HEBREW:	code = "he";	break;
		case LANG_HUNGARIAN:	code = "hu";	break;
		case LANG_ITALIAN:	code = "it";	break;
		case LANG_JAPANESE:	code = "ja";	break;
		case LANG_KOREAN:	code = "ko";	break;
		case LANG_LITHUANIAN:	code = "lt";	break;
		case LANG_NORWEGIAN:	code = "no";	break;
		case LANG_POLISH:	code = "pl";	break;
		case LANG_PORTUGUESE:	code = "pt";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE)		code = "pt";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_PORTUGUESE_BRAZILIAN)	code = "pt_BR";
							break;
		case LANG_ROMANIAN:	code = "ro";	break;
		case LANG_RUSSIAN:	code = "ru";	break;
		case LANG_SERBIAN:	code = "sr";	if	(SUBLANGID(GetUserDefaultLangID()) == SUBLANG_CROATIAN_CROATIA)		code = "hr";
							else if (SUBLANGID(GetUserDefaultLangID()) == SUBLANG_SERBIAN_LATIN)		code = "sr";
							break;
		case LANG_SLOVAK:	code = "sk";	break;
		case LANG_SPANISH:	code = "es";	break;
		case LANG_SWEDISH:	code = "sv";	break;
		case LANG_TURKISH:	code = "tr";	break;
		case LANG_UKRAINIAN:	code = "uk";	break;
		case LANG_VIETNAMESE:	code = "vi";	break;
	}
#endif

	if (ActivateLanguage(String(appPrefix).Append("_").Append(code).Append(".xml")) == Error()) ActivateLanguage("internal");

	return Success();
}

S::Int S::I18n::Translator::GetSupportedLanguages()
{
	{
		Language	*language = new Language();

		language->name = "Default language";
		language->encoding = "UTF-8";
		language->magic = "internal";
		language->author = "unknown";
		language->url = "none";
		language->rightToLeft = False;

		languages.Add(language);
	}

	Directory		 dir(GUI::Application::GetApplicationDirectory().Append("lang").Append(Directory::GetDirectoryDelimiter()));
	const Array<File>	&files = dir.GetFilesByPattern(String(appPrefix).Append("_*.xml"));

	for (Int i = 0; i < files.Length(); i++)
	{
		XML::Document	*doc = new XML::Document();

		if (doc->LoadFile(files.GetNth(i)) == Success())
		{
			Language	*language = new Language();

			language->magic = files.GetNth(i).GetFileName();

			LoadDoc(doc, language);
		}

		delete doc;
	}

	return Success();
}

S::Int S::I18n::Translator::GetNOfLanguages() const
{
	return languages.Length();
}

const S::String &S::I18n::Translator::GetNthLanguageName(Int index) const
{
	return languages.GetNth(index)->name;
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
	return activeLanguage->name;
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
	for (int i = 0; i < languages.Length(); i++)
	{
		if (languages.GetNth(i)->magic == magic)
		{
			activeLanguage = languages.GetNth(i);

			return Success();
		}
	}

	return Error();
}

const S::String &S::I18n::Translator::TranslateString(const String &string)
{
	const String	&translation = activeLanguage->strings.Get(string.ComputeCRC32());

	if (translation == NIL)	return string;
	else			return translation;
}

S::Int S::I18n::Translator::LoadDoc(XML::Document *doc, Language *language)
{
	/* Return an error if we are passed a null document.
	 */
	if (doc == NIL) return Error();

	XML::Node	*root = doc->GetRootNode();

	/* Return an error if the document is invalid.
	 */
	if (root == NIL) return Error();

	XML::Node	*info = root->GetNodeByName("info");

	/* Return an error if we didn't find an info node.
	 */
	if (info == NIL) return Error();

	String		 program;
	String		 version;

	for (Int i = 0; i < info->GetNOfNodes(); i++)
	{
		String	 property = info->GetNthNode(i)->GetAttributeByName("name")->GetContent();

		if (property == "program")	 program = info->GetNthNode(i)->GetContent();
		if (property == "version")	 version = info->GetNthNode(i)->GetContent();
		if (property == "language")	 language->name = info->GetNthNode(i)->GetContent();
		if (property == "righttoleft")	 language->rightToLeft = (info->GetNthNode(i)->GetContent() == "true" ? True : False);
		if (property == "encoding")	 language->encoding = info->GetNthNode(i)->GetContent();
		if (property == "author")	 language->author = info->GetNthNode(i)->GetContent();
		if (property == "url")		 language->url = info->GetNthNode(i)->GetContent();
	}

	XML::Node	*data = root->GetNodeByName("data");

	/* Return an error if we didn't find a data node.
	 */
	if (data == NIL) return Error();

	for (Int k = 0; k < data->GetNOfNodes(); k++)
	{
		XML::Node	*entry = data->GetNthNode(k);

		if (entry->GetName() == "entry" && entry->GetAttributeByName("string") != NIL)
		{
			language->strings.Add(entry->GetContent(), entry->GetAttributeByName("string")->GetContent().ComputeCRC32());
		}
	}

	Bool		 done = False;

	for (Int j = 0; j < languages.Length(); j++)
	{
		Language	*lang = languages.GetNth(j);

		for (Int k = 0; k < Math::Max(language->name.Length(), lang->name.Length()); k++)
		{
			if ((language->name[k] < lang->name[k] && !(language->name[k] == '(' && lang->name[k] == '/')) || (language->name[k] == '/' && lang->name[k] == '('))
			{
				languages.InsertAtPos(j, language);

				done = True;

				break;
			}
			else if ((language->name[k] > lang->name[k] && !(language->name[k] == '/' && lang->name[k] == '(')) || (language->name[k] == '(' && lang->name[k] == '/'))
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
