 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dialogs/dirdlg.h>
#include <smooth/i18n.h>
#include <smooth/shlobjmini.h>
#include <smooth/graphics/window.h>

S::DialogDirSelection::DialogDirSelection()
{
	caption = I18n::Translator::defaultTranslator->TranslateString("Select directory");
	directory = NIL;
}

S::DialogDirSelection::~DialogDirSelection()
{
}

S::Int S::DialogDirSelection::ShowDialog()
{
	SHLObjMini_Init();

	if (Setup::enableUnicode && ex_SHGetPathFromIDListW != NIL && ex_SHBrowseForFolderW != NIL)
	{
		BROWSEINFOW	 infow;
		wchar_t		*bufferw = new wchar_t [32768];

		for (Int i = 0; i < 32768; i++) bufferw[i] = 0;

		if (parentWindow != NIL)	infow.hwndOwner = parentWindow->hwnd;
		else				infow.hwndOwner = NIL;

		infow.pidlRoot = NIL;
		infow.pszDisplayName = bufferw;
		infow.lpszTitle = caption;
		infow.ulFlags = BIF_RETURNONLYFSDIRS;
		infow.lpfn = NIL;
		infow.lParam = 0;
		infow.iImage = 0;

		ex_SHGetPathFromIDListW(ex_SHBrowseForFolderW(&infow), bufferw);

		directory = NIL;

		if (bufferw != NIL && bufferw[0] != 0) directory = bufferw;

		delete [] bufferw;
	}
	else
	{
		BROWSEINFOA	 infoa;
		char		*buffera = new char [32768];

		for (Int i = 0; i < 32768; i++) buffera[i] = 0;

		if (parentWindow != NIL)	infoa.hwndOwner = parentWindow->hwnd;
		else				infoa.hwndOwner = NIL;

		infoa.pidlRoot = NIL;
		infoa.pszDisplayName = buffera;
		infoa.lpszTitle = caption;
		infoa.ulFlags = BIF_RETURNONLYFSDIRS;
		infoa.lpfn = NIL;
		infoa.lParam = 0;
		infoa.iImage = 0;

		ex_SHGetPathFromIDListA(ex_SHBrowseForFolderA(&infoa), buffera);

		directory = NIL;

		if (buffera != NIL && buffera[0] != 0) directory = buffera;

		delete [] buffera;
	}

	SHLObjMini_Deinit();

	if (directory != NIL)
	{
		if (directory[directory.Length() - 1] != '\\') directory[directory.Length()] = '\\';
	
		return Success;
	}
	else
	{
		return Error;
	}
}

S::String S::DialogDirSelection::GetDirName()
{
	return directory;
}
