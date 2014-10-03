 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/file/filedlg_win32.h>
#include <smooth/gui/window/window.h>
#include <smooth/files/directory.h>

#include <windows.h>
#include <commdlg.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	Bool	 result = False;

	/* Get default filter index.
	 */
	Int	 filterIndex = 1;

	if (defExt != NIL)
	{
		for (Int k = 0; k < filters.Length(); k++)
		{
			if (filters.GetNth(k).ToLower().Contains(String("*.").Append(defExt).ToLower()))
			{
				filterIndex = k + 1;

				break;
			}
		}
	}

	/* Configure and display dialog.
	 */
	Int	 bpos = 0;

	static OPENFILENAME	 ofn;
	wchar_t			*filter = new wchar_t [32768];
	wchar_t			*buffer = new wchar_t [32768];

	for (Int i = 0; i < 32768; i++)		   buffer[i] = 0;
	for (Int n = 0; n < defFile.Length(); n++) buffer[n] = defFile[n];

	if (parentWindow != NIL) ofn.hwndOwner = (HWND) parentWindow->GetSystemWindow();
	else			 ofn.hwndOwner = NIL;

	ofn.lStructSize	    = sizeof(OPENFILENAMEW);
	ofn.nFilterIndex    = filterIndex;
	ofn.lpstrFile	    = buffer;
	ofn.nMaxFile	    = 32768;
	ofn.lpstrFileTitle  = NIL;
	ofn.lpstrInitialDir = NIL;
	ofn.lpstrTitle	    = caption;
	ofn.Flags	    = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
	ofn.lpstrDefExt	    = defExt;

	for (Int k = 0; k < filters.Length(); k++)
	{
		for (Int l = 0; l < filterNames.GetNth(k).Length(); l++) filter[bpos++] = filterNames.GetNth(k)[l];

		filter[bpos++] = 0;

		for (Int m = 0; m < filters.GetNth(k).Length(); m++) filter[bpos++] = filters.GetNth(k)[m];

		filter[bpos++] = 0;
	}

	filter[bpos++] = 0;

	ofn.lpstrFilter = filter;

	if	(mode == SFM_OPEN) { ofn.Flags |= OFN_FILEMUSTEXIST; result = GetOpenFileName(&ofn); }
	else if (mode == SFM_SAVE) {				     result = GetSaveFileName(&ofn); }

	if (result)
	{
		Int	 n;
		Int	 pos = 0;
		String	 dir;
		String	 file;
		wchar_t	*buffer2 = new wchar_t [32768];

		for (n = 0; n < 32768; n++)
		{
			buffer2[pos++] = buffer[n];

			if (buffer[n] == 0)
			{
				dir.Copy(buffer2);

				break;
			}
		}

		if (flags & SFD_ALLOWMULTISELECT)
		{
			n++;
			pos = 0;

			for (; n < 32768; n++)
			{
				buffer2[pos++] = buffer[n];

				if (buffer[n] == 0)
				{
					file = file.Copy(dir).Append(Directory::GetDirectoryDelimiter()).Append(buffer2);

					if (file.EndsWith(Directory::GetDirectoryDelimiter())) file[file.Length() - 1] = 0;

					files.Add(file);

					pos = 0;

					if (buffer[n + 1] == 0) break;
				}
			}
		}
		else
		{
			files.Add(dir);
		}

		delete [] buffer2;
	}
	else
	{
		error = Error();
	}

	delete [] buffer;
	delete [] filter;

	return error;
}
