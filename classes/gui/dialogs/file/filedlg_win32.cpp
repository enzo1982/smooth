 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/filedlg.h>
#include <smooth/gui/window/window.h>
#include <smooth/files/directory.h>
#include <smooth/backends/win32/backendwin32.h>
#include <smooth/foreach.h>

#include <windows.h>
#include <commdlg.h>

#define MAX_EXT_PATH 32768

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	Bool	 result = False;

	/* Get default filter index.
	 */
	Int	 filterIndex = 1;

	if (defExt != NIL)
	{
		foreach (const String &filter, filters)
		{
			if (!filter.ToLower().Contains(String("*.").Append(defExt).ToLower())) continue;

			filterIndex = foreachindex + 1;

			break;
		}
	}

	/* Configure and display dialog.
	 */
	OPENFILENAME	 ofn		      = { 0 };
	wchar_t		 filter[MAX_EXT_PATH] = { 0 };
	wchar_t		 buffer[MAX_EXT_PATH] = { 0 };

	wcsncpy(buffer, defFile, defFile.Length());

	if (parentWindow != NIL) ofn.hwndOwner = (HWND) parentWindow->GetSystemWindow();

	ofn.lStructSize	    = sizeof(OPENFILENAME);
	ofn.nFilterIndex    = filterIndex;
	ofn.lpstrFile	    = buffer;
	ofn.nMaxFile	    = MAX_EXT_PATH;
	ofn.lpstrInitialDir = defPath;
	ofn.lpstrTitle	    = caption;
	ofn.Flags	    = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
	ofn.lpstrDefExt	    = defExt;

	Int	 bpos = 0;

	for (Int i = 0; i < filters.Length(); i++)
	{
		for (Int n = 0; n < filterNames.GetNth(i).Length(); n++) filter[bpos++] = filterNames.GetNth(i)[n];

		filter[bpos++] = 0;

		for (Int n = 0; n < filters.GetNth(i).Length(); n++) filter[bpos++] = filters.GetNth(i)[n];

		filter[bpos++] = 0;
	}

	filter[bpos++] = 0;

	ofn.lpstrFilter = filter;

	if	(mode == SFM_OPEN) { ofn.Flags |= OFN_FILEMUSTEXIST; result = GetOpenFileName(&ofn); }
	else if (mode == SFM_SAVE) {				     result = GetSaveFileName(&ofn); }

	if (!result)
	{
		error = Error();

		return error;
	}

	/* Extract file names from result buffer.
	 */
	Int	 n;
	Int	 pos = 0;
	String	 folder;
	wchar_t	 name[MAX_EXT_PATH];

	for (n = 0; n < MAX_EXT_PATH; n++)
	{
		name[pos++] = buffer[n];

		if (buffer[n] == 0)
		{
			folder = name;

			break;
		}
	}

	if (flags & SFD_ALLOWMULTISELECT)
	{
		n++;
		pos = 0;

		for (; n < MAX_EXT_PATH; n++)
		{
			name[pos++] = buffer[n];

			if (buffer[n] == 0)
			{
				String	 file = String(folder).Append(Directory::GetDirectoryDelimiter()).Append(name);

				if (file.EndsWith(Directory::GetDirectoryDelimiter())) file[file.Length() - 1] = 0;

				files.Add(Backends::BackendWin32::GetFullPathName(file));

				pos = 0;

				if (buffer[n + 1] == 0) break;
			}
		}
	}
	else
	{
		files.Add(Backends::BackendWin32::GetFullPathName(folder));
	}

	return error;
}
