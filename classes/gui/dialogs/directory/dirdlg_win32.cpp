 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/gui/window/window.h>
#include <smooth/files/directory.h>
#include <smooth/backends/win32/backendwin32.h>

#include <wtypes.h>
#include <shlobj.h>

#define MAX_EXT_PATH 32768

int CALLBACK	 BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	BROWSEINFO	 info		      = { 0 };
	wchar_t		 buffer[MAX_EXT_PATH] = { 0 };

	if (parentWindow != NIL) info.hwndOwner = (HWND) parentWindow->GetSystemWindow();

	info.pszDisplayName = buffer;
	info.lpszTitle	    = String("\n").Append(caption);
	info.ulFlags	    = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	info.lpfn	    = &BrowseCallbackProc;
	info.lParam	    = (LPARAM) this;

	ITEMIDLIST	*idlist = SHBrowseForFolder(&info);

	SHGetPathFromIDList(idlist, buffer);

	CoTaskMemFree(idlist);

	directory = NIL;

	if (buffer[0] == 0)
	{
		error = Error();

		return error;
	}

	directory = Backends::BackendWin32::GetFullPathName(buffer);

	if (!directory.EndsWith(Directory::GetDirectoryDelimiter())) directory.Append(Directory::GetDirectoryDelimiter());

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
