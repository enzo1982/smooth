 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/directory/dirdlg_win32.h>
#include <smooth/gui/window/window.h>
#include <smooth/files/directory.h>

#include <wtypes.h>
#include <shlobj.h>

#ifndef BIF_USENEWUI
#	define BIF_USENEWUI 0
#endif

int CALLBACK	 BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	BROWSEINFO	 info;
	wchar_t		*buffer = new wchar_t [32768];

	for (Int i = 0; i < 32768; i++) buffer[i] = 0;

	if (parentWindow != NIL) info.hwndOwner = (HWND) parentWindow->GetSystemWindow();
	else			 info.hwndOwner = NIL;

	info.pidlRoot	    = NIL;
	info.pszDisplayName = buffer;
	info.lpszTitle	    = caption;
	info.ulFlags	    = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	info.lpfn	    = &BrowseCallbackProc;
	info.lParam	    = (LPARAM) this;
	info.iImage	    = 0;

	ITEMIDLIST	*idlist = SHBrowseForFolder(&info);

	SHGetPathFromIDList(idlist, buffer);

	CoTaskMemFree(idlist);

	directory = NIL;

	if (buffer[0] != 0) directory = buffer;

	delete [] buffer;

	if (directory != NIL)
	{
		if (!directory.EndsWith(Directory::GetDirectoryDelimiter())) directory.Append(Directory::GetDirectoryDelimiter());
	}
	else
	{
		error = Error();
	}

	return error;
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName() != NIL)
	{
		SendMessage(hwnd, BFFM_SETSELECTION, true, (LPARAM) (wchar_t *) ((S::GUI::Dialogs::DirSelection *) lpData)->GetDirName());
	}

	return 0;
}
