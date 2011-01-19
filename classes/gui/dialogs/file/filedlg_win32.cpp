 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
	Int	 bpos = 0;

	if (Setup::enableUnicode)
	{
		static OPENFILENAMEW	 ofnw;
		wchar_t			*filterw = new wchar_t [32768];
		wchar_t			*bufferw = new wchar_t [32768];

		for (Int i = 0; i < 32768; i++)		   bufferw[i] = 0;
		for (Int n = 0; n < defFile.Length(); n++) bufferw[n] = defFile[n];

		if (parentWindow != NIL)	ofnw.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				ofnw.hwndOwner = NIL;

		ofnw.lStructSize	= sizeof(OPENFILENAMEW);
		ofnw.nFilterIndex	= 1;
		ofnw.lpstrFile		= bufferw;
		ofnw.nMaxFile		= 32768;
		ofnw.lpstrFileTitle	= NIL;
		ofnw.lpstrInitialDir	= NIL;
		ofnw.lpstrTitle		= caption;
		ofnw.Flags		= OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
		ofnw.lpstrDefExt	= defExt;

		for (int k = 0; k < filters.Length(); k++)
		{
			for (int l = 0; l < filterNames.GetNth(k).Length(); l++)
			{
				filterw[bpos++] = filterNames.GetNth(k)[l];
			}

			filterw[bpos++] = 0;

			for (int m = 0; m < filters.GetNth(k).Length(); m++)
			{
				filterw[bpos++] = filters.GetNth(k)[m];
			}

			filterw[bpos++] = 0;
		}

		filterw[bpos++] = 0;

		ofnw.lpstrFilter = filterw;

		if (mode == SFM_OPEN)
		{
			ofnw.Flags |= OFN_FILEMUSTEXIST;

			result = GetOpenFileNameW(&ofnw);
		}
		else if (mode == SFM_SAVE)
		{
			result = GetSaveFileNameW(&ofnw);
		}

		if (result)
		{
			Int	 n;
			Int	 pos = 0;
			String	 dir;
			String	 file;
			wchar_t	*buffer2w = new wchar_t [32768];

			for (n = 0; n < 32768; n++)
			{
				buffer2w[pos++] = bufferw[n];

				if (bufferw[n] == 0)
				{
					dir.Copy(buffer2w);

					break;
				}
			}

			if (flags & SFD_ALLOWMULTISELECT)
			{
				n++;
				pos = 0;

				for (; n < 32768; n++)
				{
					buffer2w[pos++] = bufferw[n];

					if (bufferw[n] == 0)
					{
						file = file.Copy(dir).Append(Directory::GetDirectoryDelimiter()).Append(buffer2w);

						if (file.EndsWith(Directory::GetDirectoryDelimiter())) file[file.Length() - 1] = 0;

						files.Add(file);

						pos = 0;

						if (bufferw[n + 1] == 0) break;
					}
				}
			}
			else
			{
				files.Add(dir);
			}

			delete [] buffer2w;
		}
		else
		{
			error = Error();
		}

		delete [] bufferw;
		delete [] filterw;
	}
	else
	{
		static OPENFILENAMEA	 ofna;
		char			*filtera = new char [32768];
		char			*buffera = new char [32768];

		for (Int j = 0; j < 32768; j++)		   buffera[j] = 0;
		for (Int n = 0; n < defFile.Length(); n++) buffera[n] = defFile[n];

		if (parentWindow != NIL)	ofna.hwndOwner = (HWND) parentWindow->GetSystemWindow();
		else				ofna.hwndOwner = NIL;

		ofna.lStructSize	= sizeof(OPENFILENAMEA);
		ofna.nFilterIndex	= 1;
		ofna.lpstrFile		= buffera;
		ofna.nMaxFile		= 32768;
		ofna.lpstrFileTitle	= NIL;
		ofna.lpstrInitialDir	= NIL;
		ofna.lpstrTitle		= caption;
		ofna.Flags		= OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
		ofna.lpstrDefExt	= defExt;

		for (int k = 0; k < filters.Length(); k++)
		{
			char	*filterName = filterNames.GetNth(k);

			for (int l = 0; l < filterNames.GetNth(k).Length(); l++)
			{
				filtera[bpos++] = filterName[l];
			}

			filtera[bpos++] = 0;

			char	*filter = filters.GetNth(k);

			for (int m = 0; m < filters.GetNth(k).Length(); m++)
			{
				filtera[bpos++] = filter[m];
			}

			filtera[bpos++] = 0;
		}

		filtera[bpos++] = 0;

		ofna.lpstrFilter = filtera;

		if (mode == SFM_OPEN)
		{
			ofna.Flags |= OFN_FILEMUSTEXIST;

			result = GetOpenFileNameA(&ofna);
		}
		else if (mode == SFM_SAVE)
		{
			result = GetSaveFileNameA(&ofna);
		}

		if (result)
		{
			Int	 n;
			Int	 pos = 0;
			String	 dir;
			String	 file;
			char	*buffer2a = new char [32768];

			for (n = 0; n < 32768; n++)
			{
				buffer2a[pos++] = buffera[n];

				if (buffera[n] == 0)
				{
					dir.Copy(buffer2a);

					break;
				}
			}

			if (flags & SFD_ALLOWMULTISELECT)
			{
				n++;
				pos = 0;

				for (; n < 32768; n++)
				{
					buffer2a[pos++] = buffera[n];

					if (buffera[n] == 0)
					{
						file = file.Copy(dir).Append(Directory::GetDirectoryDelimiter()).Append(buffer2a);

						if (file.EndsWith(Directory::GetDirectoryDelimiter())) file[file.Length() - 1] = 0;

						files.Add(file);

						pos = 0;

						if (buffera[n + 1] == 0) break;
					}
				}
			}
			else
			{
				files.Add(dir);
			}

			delete [] buffer2a;
		}
		else
		{
			error = Error();
		}

		delete [] buffera;
		delete [] filtera;
	}

	return error;
}
