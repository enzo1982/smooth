 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dirdlg.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/shlobjmini.h>
#include <smooth/window.h>

S::DialogDirSelection::DialogDirSelection()
{
	caption = TXT_SELECTDIR;
	directory = NIL;
}

S::DialogDirSelection::~DialogDirSelection()
{
}

S::Int S::DialogDirSelection::ShowDialog()
{
	BROWSEINFOW	 infow;
	BROWSEINFOA	 infoa;
	wchar_t		*bufferw = new wchar_t [32768];
	char		*buffera = new char [32768];

	for (Int i = 0; i < 32768; i++) bufferw[i] = 0;
	for (Int j = 0; j < 32768; j++) buffera[j] = 0;
 
	if (parentWindow != NIL)
	{
		infow.hwndOwner = parentWindow->hwnd;
		infoa.hwndOwner = parentWindow->hwnd;
	}
	else
	{
		infow.hwndOwner = NIL;
		infoa.hwndOwner = NIL;
	}

	infow.pidlRoot = NIL;
	infow.pszDisplayName = bufferw;
	infow.lpszTitle = caption;
	infow.ulFlags = BIF_RETURNONLYFSDIRS;
	infow.lpfn = NIL;
	infow.lParam = 0;
	infow.iImage = 0;

	infoa.pidlRoot = NIL;
	infoa.pszDisplayName = buffera;
	infoa.lpszTitle = caption;
	infoa.ulFlags = BIF_RETURNONLYFSDIRS;
	infoa.lpfn = NIL;
	infoa.lParam = 0;
	infoa.iImage = 0;

	if (Setup::enableUnicode)	SHGetPathFromIDListW(SHBrowseForFolderW(&infow), bufferw);
	else				SHGetPathFromIDListA(SHBrowseForFolderA(&infoa), buffera);

	directory = NIL;

	if (Setup::enableUnicode && (bufferw != NIL && bufferw[0] != 0))	directory = bufferw;
	else if (buffera != NIL && buffera[0] != 0)				directory = buffera;

	delete [] bufferw;
	delete [] buffera;

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
