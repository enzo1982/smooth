 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DIRDLG_
#define __OBJSMOOTH_DIRDLG_

#include <smooth/dirdlg.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/shlobjmini.h>
#include <smooth/window.h>

SMOOTHDialogDirSelection::SMOOTHDialogDirSelection()
{
	caption = TXT_SELECTDIR;
	directory = NIL;
}

SMOOTHDialogDirSelection::~SMOOTHDialogDirSelection()
{
}

SMOOTHInt SMOOTHDialogDirSelection::ShowDialog()
{
	BROWSEINFOW	 infow;
	BROWSEINFOA	 infoa;
	wchar_t		*bufferw = new wchar_t [32768];
	char		*buffera = new char [32768];

	for (SMOOTHInt i = 0; i < 32768; i++) bufferw[i] = 0;
	for (SMOOTHInt j = 0; j < 32768; j++) buffera[j] = 0;
 
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

	if (SMOOTH::Setup::enableUnicode)	SHGetPathFromIDListW(SHBrowseForFolderW(&infow), bufferw);
	else					SHGetPathFromIDListA(SHBrowseForFolderA(&infoa), buffera);

	directory = NIL;

	if (SMOOTH::Setup::enableUnicode && (bufferw != NIL && bufferw[0] != 0))	directory = bufferw;
	else if (buffera != NIL && buffera[0] != 0)					directory = buffera;

	delete [] bufferw;
	delete [] buffera;

	if (directory != NIL)
	{
		if (directory[directory.Length() - 1] != '\\') directory[directory.Length()] = '\\';
	
		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHString SMOOTHDialogDirSelection::GetDirName()
{
	return directory;
}

#endif
