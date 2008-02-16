 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/i18n/translator.h>
#include <smooth/gui/window/window.h>

#ifdef __WIN32__
#	include <wtypes.h>
#	include <shlobj.h>

#	ifndef BIF_USENEWUI
#		define BIF_USENEWUI 0
#	endif

	int CALLBACK	 BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);
#endif

S::GUI::Dialogs::DirSelection::DirSelection()
{
	caption		= I18n::Translator::defaultTranslator->TranslateString("Select directory");
	directory	= NIL;
}

S::GUI::Dialogs::DirSelection::~DirSelection()
{
}

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
#ifdef __WIN32__
	if (Setup::enableUnicode)
	{
		BROWSEINFOW	 infow;
		wchar_t		*bufferw = new wchar_t [32768];

		for (Int i = 0; i < 32768; i++) bufferw[i] = 0;

		if (parentWindow != NIL)	infow.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				infow.hwndOwner = NIL;

		infow.pidlRoot		= NIL;
		infow.pszDisplayName	= bufferw;
		infow.lpszTitle		= caption;
		infow.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		infow.lpfn		= &BrowseCallbackProc;
		infow.lParam		= (LPARAM) this;
		infow.iImage		= 0;

		ITEMIDLIST	*idlist = SHBrowseForFolderW(&infow);

		SHGetPathFromIDListW(idlist, bufferw);

		CoTaskMemFree(idlist);

		directory = NIL;

		if (bufferw != NIL && bufferw[0] != 0) directory = bufferw;

		delete [] bufferw;
	}
	else
	{
		BROWSEINFOA	 infoa;
		char		*buffera = new char [32768];

		for (Int i = 0; i < 32768; i++) buffera[i] = 0;

		if (parentWindow != NIL)	infoa.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				infoa.hwndOwner = NIL;

		infoa.pidlRoot		= NIL;
		infoa.pszDisplayName	= buffera;
		infoa.lpszTitle		= caption;
		infoa.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		infoa.lpfn		= &BrowseCallbackProc;
		infoa.lParam		= (LPARAM) this;
		infoa.iImage		= 0;

		ITEMIDLIST	*idlist = SHBrowseForFolderA(&infoa);

		SHGetPathFromIDListA(idlist, buffera);

		CoTaskMemFree(idlist);

		directory = NIL;

		if (buffera != NIL && buffera[0] != 0) directory = buffera;

		delete [] buffera;
	}
#endif

	if (directory != NIL)
	{
		if (directory[directory.Length() - 1] != '\\') directory[directory.Length()] = '\\';
	}
	else
	{
		error = Error();
	}

	return error;
}

S::Int S::GUI::Dialogs::DirSelection::SetDirName(const String &nDirectory)
{
	directory = nDirectory;

	return Success();
}

const S::String &S::GUI::Dialogs::DirSelection::GetDirName()
{
	return directory;
}

#ifdef __WIN32__
	int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		if (uMsg == BFFM_INITIALIZED && ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName() != NIL)
		{
			if (S::Setup::enableUnicode)	SendMessageW(hwnd, BFFM_SETSELECTION, true, (LPARAM) (wchar_t *) ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName());
			else				SendMessageA(hwnd, BFFM_SETSELECTION, true, (LPARAM) (wchar_t *) ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName());
		}

		return 0;
	}
#endif
