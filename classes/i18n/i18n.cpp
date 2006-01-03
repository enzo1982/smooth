 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int S::I18N_DEFAULTFONTSIZE;

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::String S::I18N_DEFAULTFONT;

#ifdef __WIN32__
int CALLBACK EnumFontProcA(ENUMLOGFONTEXA *lpelfe, NEWTEXTMETRICEXA *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}

int CALLBACK EnumFontProcW(ENUMLOGFONTEXW *lpelfe, NEWTEXTMETRICEXW *lpntme, int fontType, LPARAM lParam)
{
	if (S::String(lpelfe->elfLogFont.lfFaceName) == "Microsoft Sans Serif")	return 0;
	else									return 1;
}

#endif

S::I18n::Language::Language()
{
	rightToLeft = False;

	// get the default font

#ifdef __WIN32__
	HDC		 dc = GetWindowDC(0);
	LOGFONTA	 fontInfoA;
	LOGFONTW	 fontInfoW;

	fontInfoA.lfCharSet = DEFAULT_CHARSET;
	fontInfoA.lfFaceName[0] = 0;
	fontInfoA.lfPitchAndFamily = 0;

	fontInfoW.lfCharSet = DEFAULT_CHARSET;
	fontInfoW.lfFaceName[0] = 0;
	fontInfoW.lfPitchAndFamily = 0;

	int	 result;

	if (Setup::enableUnicode)	result = EnumFontFamiliesExW(dc, &fontInfoW, (FONTENUMPROCW) &EnumFontProcW, 0, 0);
	else				result = EnumFontFamiliesExA(dc, &fontInfoA, (FONTENUMPROCA) &EnumFontProcA, 0, 0);

	if (result == 0)	I18N_DEFAULTFONT = "Microsoft Sans Serif";
	else			I18N_DEFAULTFONT = "MS Sans Serif";

	ReleaseDC(0, dc);
#else
	I18N_DEFAULTFONT = "Helvetica";
#endif

	I18N_DEFAULTFONTSIZE = 8;
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
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		delete languages.GetNthEntry(i);
	}

	languages.RemoveAll();
}

S::Int S::I18n::Translator::SetInternalLanguageInfo(const String &langName, const String &author, const String &url, Bool rightToLeft)
{
	Language	*iLang = NIL;

	for (Int n = 0; n < languages.GetNOfEntries(); n++)
	{
		if (languages.GetNthEntry(n)->magic == "internal")
		{
			iLang = languages.GetNthEntry(n);

			languages.RemoveEntry(languages.GetNthEntryIndex(n));

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

		for (Int i = 0; i <= languages.GetNOfEntries(); i++)
		{
			if (i == languages.GetNOfEntries())
			{
				languages.AddEntry(iLang);

				break;
			}

			Language	*lang = languages.GetNthEntry(i);

			for (Int j = 0; j < (Int) Math::Max(iLang->name.Length(), lang->name.Length()); j++)
			{
				if (iLang->name[j] < lang->name[j])
				{
					languages.InsertEntryAtPos(i, iLang);

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

		languages.AddEntry(language);
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

S::Int S::I18n::Translator::GetNOfLanguages()
{
	return languages.GetNOfEntries();
}

S::String S::I18n::Translator::GetNthLanguageName(Int index)
{
	return languages.GetNthEntry(index)->name;
}

S::String S::I18n::Translator::GetNthLanguageID(Int index)
{
	return languages.GetNthEntry(index)->magic;
}

S::String S::I18n::Translator::GetNthLanguageAuthor(Int index)
{
	return languages.GetNthEntry(index)->author;
}

S::String S::I18n::Translator::GetNthLanguageEncoding(Int index)
{
	return languages.GetNthEntry(index)->encoding;
}

S::String S::I18n::Translator::GetNthLanguageURL(Int index)
{
	return languages.GetNthEntry(index)->url;
}

const S::String &S::I18n::Translator::GetActiveLanguageName()
{
	return activeLanguage->name;
}

const S::String &S::I18n::Translator::GetActiveLanguageID()
{
	return activeLanguage->magic;
}

const S::String &S::I18n::Translator::GetActiveLanguageAuthor()
{
	return activeLanguage->author;
}

const S::String &S::I18n::Translator::GetActiveLanguageEncoding()
{
	return activeLanguage->encoding;
}

const S::String &S::I18n::Translator::GetActiveLanguageURL()
{
	return activeLanguage->url;
}

S::Int S::I18n::Translator::ActivateLanguage(const String &magic)
{
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		if (languages.GetNthEntry(i)->magic == magic)
		{
			activeLanguage = languages.GetNthEntry(i);

			Setup::rightToLeft = activeLanguage->rightToLeft;

			return Success();
		}
	}

	return Error();
}

S::String S::I18n::Translator::TranslateString(const String &string)
{
	String	 translation = activeLanguage->strings.GetEntry(string.ComputeCRC32());

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

	for (Int j = 0; j < languages.GetNOfEntries(); j++)
	{
		Language	*lang = languages.GetNthEntry(j);

		for (Int k = 0; k < max(language->name.Length(), lang->name.Length()); k++)
		{
			if ((language->name[k] < lang->name[k] && !(language->name[k] == '(' && lang->name[k] == '/')) || (language->name[k] == '/' && lang->name[k] == '('))
			{
				languages.InsertEntryAtPos(j, language);

				done = True;

				break;
			}
			else if ((language->name[k] > lang->name[k] && !(language->name[k] == '/' && lang->name[k] == '(')) || (language->name[k] == '(' && lang->name[k] == '/'))
			{
				break;
			}
		}

		if (j == languages.GetNOfEntries() - 1)
		{
			languages.AddEntry(language);

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
			language->strings.AddEntry(entry->GetContent(), entry->GetAttributeByName("string")->GetContent().ComputeCRC32());
		}
	}

	return Success();
}
