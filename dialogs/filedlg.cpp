 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define MAKEUNICODESTR(x) L##x

#include <smooth/filedlg.h>
#include <smooth/stk.h>
#include <smooth/graphics/gdi/windowgdi.h>

#include <commdlg.h>

S::DialogFileSelection::DialogFileSelection()
{
	flags = 0;
	mode = 0;
}

S::DialogFileSelection::~DialogFileSelection()
{
}

S::Int S::DialogFileSelection::ShowDialog()
{
	bool	 result;
	Int	 retValue = Success;
	Int	 bpos = 0;

	if (Setup::enableUnicode)
	{
		static OPENFILENAMEW	 ofnw;
		wchar_t			*filterw = new wchar_t [32768];
		wchar_t			*bufferw = new wchar_t [32768];

		for (Int i = 0; i < 32768; i++) bufferw[i] = 0;

		if (parentWindow != NIL)	ofnw.hwndOwner = ((GUI::WindowGDI *) parentWindow)->hwnd;
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

		for (int k = 0; k < filters.GetNOfEntries(); k++)
		{
			for (int l = 0; l < filterNames.GetNthEntry(k).Length(); l++)
			{
				filterw[bpos] = filterNames.GetNthEntry(k)[l];

				bpos++;
			}

			filterw[bpos] = 0;

			bpos++;

			for (int m = 0; m < filters.GetNthEntry(k).Length(); m++)
			{
				filterw[bpos] = filters.GetNthEntry(k)[m];

				bpos++;
			}

			filterw[bpos] = 0;

			bpos++;
		}

		filterw[bpos++] = 0;

		ofnw.lpstrFilter = filterw;

		if (mode == SFM_OPEN)
		{
			ofnw.Flags |= OFN_FILEMUSTEXIST;

			result = GetOpenFileNameW(&ofnw);
		}
		else
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
						file = file.Copy(dir).Append("\\").Append(buffer2w);

						if (file[file.Length() - 1] == '\\') file[file.Length() - 1] = 0;

						files.AddEntry(file);

						pos = 0;

						if (bufferw[n + 1] == 0) break;
					}
				}
			}
			else
			{
				files.AddEntry(dir);
			}

			delete [] buffer2w;
		}
		else
		{
			retValue = Error;
		}

		delete [] bufferw;
		delete [] filterw;
	}
	else
	{
		static OPENFILENAMEA	 ofna;
		char			*filtera = new char [32768];
		char			*buffera = new char [32768];

		for (Int j = 0; j < 32768; j++) buffera[j] = 0;

		if (parentWindow != NIL)	ofna.hwndOwner = ((GUI::WindowGDI *) parentWindow)->hwnd;
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

		for (int k = 0; k < filters.GetNOfEntries(); k++)
		{
			for (int l = 0; l < filterNames.GetNthEntry(k).Length(); l++)
			{
				filtera[bpos] = filterNames.GetNthEntry(k)[l];

				bpos++;
			}

			filtera[bpos] = 0;

			bpos++;

			for (int m = 0; m < filters.GetNthEntry(k).Length(); m++)
			{
				filtera[bpos] = filters.GetNthEntry(k)[m];

				bpos++;
			}

			filtera[bpos] = 0;

			bpos++;
		}

		filtera[bpos++] = 0;

		ofna.lpstrFilter = filtera;

		if (mode == SFM_OPEN)
		{
			ofna.Flags |= OFN_FILEMUSTEXIST;

			result = GetOpenFileNameA(&ofna);
		}
		else
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
						file = file.Copy(dir).Append("\\").Append(buffer2a);

						if (file[file.Length() - 1] == '\\') file[file.Length() - 1] = 0;

						files.AddEntry(file);

						pos = 0;

						if (buffera[n + 1] == 0) break;
					}
				}
			}
			else
			{
				files.AddEntry(dir);
			}

			delete [] buffer2a;
		}
		else
		{
			retValue = Error;
		}

		delete [] buffera;
		delete [] filtera;
	}

	return retValue;
}

S::Int S::DialogFileSelection::AddFilter(String name, String filter)
{
	filterNames.AddEntry(name);
	filters.AddEntry(filter);

	return Success;
}

S::Int S::DialogFileSelection::SetFlags(Int newFlags)
{
	flags |= newFlags;

	return Success;
}

S::Int S::DialogFileSelection::SetMode(Int newMode)
{
	mode = newMode;

	return Success;
}

S::Int S::DialogFileSelection::SetDefaultExtension(String newDefExt)
{
	defExt = newDefExt;

	return Success;
}

S::Int S::DialogFileSelection::GetNumberOfFiles()
{
	return files.GetNOfEntries();
}

S::String S::DialogFileSelection::GetFileName()
{
	if (files.GetNOfEntries() > 0)	return files.GetFirstEntry();
	else				return NIL;
}

S::String S::DialogFileSelection::GetNthFileName(Int n)
{
	if (files.GetNOfEntries() > n)	return files.GetNthEntry(n);
	else				return NIL;
}
