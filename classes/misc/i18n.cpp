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
#include <smooth/i18n/smooth_de.h>
#include <smooth/application.h>
#include <smooth/math.h>
#include <smooth/toolkit.h>
#include <direct.h>
#include <io.h>

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

S::I18n::Language::Language()
{
	rightToLeft = False;

	// get the default font
	{
		HDC		 dc = GetContext(0);
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

		FreeContext(0, dc);

		I18N_DEFAULTFONTSIZE = 8;
		I18N_SMALLFONTSIZE = 8;
	}
}

S::I18n::Language::~Language()
{
	strings.RemoveAll();
}

S::I18n::Translator::Translator(Bool iInternal)
{
	internal = iInternal;

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

S::Int S::I18n::Translator::SetInternalLanguageInfo(String langName, String author, String url, Bool rightToLeft)
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

		for (Int i = 0; i < languages.GetNOfEntries(); i++)
		{
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

			if (i == languages.GetNOfEntries() - 1)
			{
				languages.AddEntry(iLang);

				done = True;
			}

			if (done) break;
		}
	}

	return Success;
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
		String		 dir = Application::GetApplicationDirectory().Append("lang\\");
		_finddata_t	 fileData;
		int		 handle;

		chdir(dir);

		if ((handle = _findfirst("*_*.xml", &fileData)) != -1)
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

		chdir(Application::GetApplicationDirectory());
	}
	else
	{
		for (Int i = 0; i < 1; i++)
		{
			String	 var = NIL;

			if (i == 0) var = smooth_de;

			doc = new XML::Document();
			language = new Language();

			doc->ParseMemory((void *) (char *) var, var.Length());

			if (i == 0) language->magic = "smooth_de";

			LoadDoc(doc, language);

			delete doc;
		}
	}

	return Success;
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

S::String S::I18n::Translator::GetActiveLanguageName()
{
	return activeLanguage->name;
}

S::String S::I18n::Translator::GetActiveLanguageID()
{
	return activeLanguage->magic;
}

S::String S::I18n::Translator::GetActiveLanguageAuthor()
{
	return activeLanguage->author;
}

S::String S::I18n::Translator::GetActiveLanguageEncoding()
{
	return activeLanguage->encoding;
}

S::String S::I18n::Translator::GetActiveLanguageURL()
{
	return activeLanguage->url;
}

S::Int S::I18n::Translator::ActivateLanguage(String magic)
{
	for (int i = 0; i < languages.GetNOfEntries(); i++)
	{
		if (languages.GetNthEntry(i)->magic == magic)
		{
			activeLanguage = languages.GetNthEntry(i);

			Setup::rightToLeft = activeLanguage->rightToLeft;

			return Success;
		}
	}

	return Error;
}

S::String S::I18n::Translator::TranslateString(String string)
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
			if (language->name[k] < lang->name[k])
			{
				languages.InsertEntryAtPos(j, language);

				done = True;

				break;
			}
			else if (language->name[k] > lang->name[k])
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

	XML::Node	*entry = doc->GetRootNode()->GetNodeByName("data")->GetNodeByName("entry");

	while (entry != NIL)
	{
		if (entry->GetName() == "entry")
		{
			language->strings.AddEntry(entry->GetContent(), entry->GetAttributeByName("string")->GetContent().ComputeCRC32());
		}

		entry = entry->GetNextNode();
	}

	return Success;
}
