 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/i18n/i18n.h>
#include <smooth/i18n/smooth_de.h>
#include <smooth/i18n/smooth_it.h>
#include <smooth/files/directory.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>

#ifdef __WIN32__
#include <direct.h>
#include <io.h>
#else
#include <glob.h>
#endif

S::I18n::Translator	*S::I18n::Translator::defaultTranslator = NIL;

S::I18n::Language::Language()
{
	rightToLeft = False;
}

S::I18n::Language::~Language()
{
	strings.RemoveAll();
}

S::I18n::Translator::Translator(const String &iAppPrefix, Bool iInternal)
{
	internal = iInternal;
	appPrefix = iAppPrefix;

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

S::Int S::I18n::Translator::GetSupportedLanguages()
{
	XML::Document	*doc = NIL;
	Language	*language = NIL;

	{
		language = new Language();

		language->name = "Default language";
		language->encoding = "UTF-8";
		language->magic = "internal";
		language->author = "unknown";
		language->url = "none";
		language->rightToLeft = False;

		languages.Add(language);
	}

	if (!internal)
	{
		if (Directory::SetActiveDirectory(Directory(GUI::Application::GetApplicationDirectory().Append("lang\\"))) == Error())
		{
			Directory::SetActiveDirectory(Directory(GUI::Application::GetStartupDirectory().Append("lang\\")));
		}

#ifdef __WIN32__
		_finddata_t	 fileData;
		int		 handle;

		if ((handle = _findfirst(String(appPrefix).ToLower().Append("_*.xml"), &fileData)) != -1)
		{
			do
			{
				doc = new XML::Document();
				language = new Language();

				doc->LoadFile(fileData.name);

				language->magic = fileData.name;

				LoadDoc(doc, language);

				delete doc;
			}
			while (_findnext(handle, &fileData) == 0);
		}

		_findclose(handle);
#else
		glob_t	*fileData = new glob_t;

		if (glob(String(appPrefix).ToLower().Append("_*.xml"), GLOB_NOSORT, NIL, fileData) == 0)
		{
			for (Int i = 0; i < fileData->gl_pathc; i++)
			{
				doc = new XML::Document();
				language = new Language();

				doc->LoadFile(fileData->gl_pathv[i]);

				language->magic = fileData->gl_pathv[i];

				LoadDoc(doc, language);

				delete doc;
			}
		}

		globfree(fileData);

		delete fileData;
#endif

		Directory::SetActiveDirectory(Directory(GUI::Application::GetStartupDirectory()));
	}
	else
	{
		for (Int i = 0; i < 2; i++)
		{
			String	 var = NIL;

			if (i == 0) var = smooth_de;
			if (i == 1) var = smooth_it;

			doc = new XML::Document();
			language = new Language();

			doc->ParseMemory((void *) (char *) var, var.Length());

			if (i == 0) language->magic = "smooth_de";
			if (i == 1) language->magic = "smooth_it";

			LoadDoc(doc, language);

			delete doc;
		}
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
	XML::Node	*info = doc->GetRootNode()->GetNodeByName("info");
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

	XML::Node	*data = doc->GetRootNode()->GetNodeByName("data");

	for (Int k = 0; k < data->GetNOfNodes(); k++)
	{
		XML::Node	*entry = data->GetNthNode(k);

		if (entry->GetName() == "entry")
		{
			language->strings.Add(entry->GetContent(), entry->GetAttributeByName("string")->GetContent().ComputeCRC32());
		}
	}

	return Success();
}
