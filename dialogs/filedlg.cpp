 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_FILEDLG_
#define __OBJSMOOTH_FILEDLG_

#define MAKEUNICODESTR(x) L##x

#include <smooth/filedlg.h>
#include <smooth/stk.h>
#include <smooth/window.h>

#include <commdlg.h>

SMOOTHDialogFileSelection::SMOOTHDialogFileSelection()
{
	flags = 0;
	mode = 0;
}

SMOOTHDialogFileSelection::~SMOOTHDialogFileSelection()
{
}

SMOOTHInt SMOOTHDialogFileSelection::ShowDialog()
{
	static OPENFILENAMEW	 ofnw;
	static OPENFILENAMEA	 ofna;
	wchar_t			*filterw = new wchar_t [32768];
	char			*filtera = new char [32768];
	wchar_t			*bufferw = new wchar_t [32768];
	char			*buffera = new char [32768];
	bool			 result;
	SMOOTHInt		 retValue = SMOOTH::Success;

	for (SMOOTHInt i = 0; i < 32768; i++) bufferw[i] = 0;
	for (SMOOTHInt j = 0; j < 32768; j++) buffera[j] = 0;

	if (parentWindow != NIL)
	{
		ofnw.hwndOwner = parentWindow->hwnd;
		ofna.hwndOwner = parentWindow->hwnd;
	}
	else
	{
		ofnw.hwndOwner = NIL;
		ofna.hwndOwner = NIL;
	}

	ofnw.lStructSize	= sizeof(OPENFILENAMEW);
	ofnw.nFilterIndex	= 1;
	ofnw.lpstrFile		= bufferw;
	ofnw.nMaxFile		= 32786;
	ofnw.lpstrFileTitle	= NIL;
	ofnw.lpstrInitialDir	= NIL;
	ofnw.lpstrTitle		= caption;
	ofnw.Flags		= OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
	ofnw.lpstrDefExt	= defExt;

	ofna.lStructSize	= sizeof(OPENFILENAMEA);
	ofna.nFilterIndex	= 1;
	ofna.lpstrFile		= buffera;
	ofna.nMaxFile		= 32786;
	ofna.lpstrFileTitle	= NIL;
	ofna.lpstrInitialDir	= NIL;
	ofna.lpstrTitle		= caption;
	ofna.Flags		= OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | flags;
	ofna.lpstrDefExt	= defExt;

	SMOOTHInt	 bpos = 0;

	for (int k = 0; k < filters.GetNOfEntries(); k++)
	{
		for (int l = 0; l < filterNames.GetNthEntry(k).Length(); l++)
		{
			filterw[bpos] = filterNames.GetNthEntry(k)[l];
			filtera[bpos] = filterNames.GetNthEntry(k)[l];

			bpos++;
		}

		filterw[bpos] = 0;
		filtera[bpos] = 0;

		bpos++;

		for (int m = 0; m < filters.GetNthEntry(k).Length(); m++)
		{
			filterw[bpos] = filters.GetNthEntry(k)[m];
			filtera[bpos] = filters.GetNthEntry(k)[m];

			bpos++;
		}

		filterw[bpos] = 0;
		filtera[bpos] = 0;

		bpos++;
	}

	filterw[bpos++] = 0;
	filtera[bpos++] = 0;

	ofnw.lpstrFilter = filterw;
	ofna.lpstrFilter = filtera;

	if (mode == SFM_OPEN)
	{
		ofnw.Flags = ofnw.Flags | OFN_FILEMUSTEXIST;
		ofna.Flags = ofna.Flags | OFN_FILEMUSTEXIST;

		if (SMOOTH::Setup::enableUnicode)	result = GetOpenFileNameW(&ofnw);
		else					result = GetOpenFileNameA(&ofna);
	}
	else
	{
		if (SMOOTH::Setup::enableUnicode)	result = GetSaveFileNameW(&ofnw);
		else					result = GetSaveFileNameA(&ofna);
	}

	if (result)
	{
		SMOOTHInt	 n;
		SMOOTHInt	 pos = 0;
		SMOOTHString	 dir;
		SMOOTHString	 file;
		wchar_t		*buffer2w = new wchar_t [1024];
		char		*buffer2a = new char [1024];

		if (SMOOTH::Setup::enableUnicode)
		{
			for (n = 0; n < 32768; n++)
			{
				buffer2w[pos++] = bufferw[n];

				if (bufferw[n] == 0)
				{
					dir.Copy(buffer2w);

					break;
				}
			}

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
			for (n = 0; n < 32768; n++)
			{
				buffer2a[pos++] = buffera[n];

				if (buffera[n] == 0)
				{
					dir.Copy(buffer2a);

					break;
				}
			}

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

		delete [] buffer2w;
		delete [] buffer2a;
	}
	else
	{
		retValue = SMOOTH::Error;
	}

	delete [] bufferw;
	delete [] buffera;
	delete [] filterw;
	delete [] filtera;

	return retValue;
}

SMOOTHInt SMOOTHDialogFileSelection::AddFilter(SMOOTHString name, SMOOTHString filter)
{
	filterNames.AddEntry(name);
	filters.AddEntry(filter);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHDialogFileSelection::SetFlags(SMOOTHInt newFlags)
{
	flags = flags | newFlags;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHDialogFileSelection::SetMode(SMOOTHInt newMode)
{
	mode = newMode;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHDialogFileSelection::SetDefaultExtension(SMOOTHString newDefExt)
{
	defExt = newDefExt;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHDialogFileSelection::GetNumberOfFiles()
{
	return files.GetNOfEntries();
}

SMOOTHString SMOOTHDialogFileSelection::GetFileName()
{
	if (files.GetNOfEntries() > 0)	return files.GetFirstEntry();
	else				return NIL;
}

SMOOTHString SMOOTHDialogFileSelection::GetNthFileName(SMOOTHInt n)
{
	if (files.GetNOfEntries() > n)	return files.GetNthEntry(n);
	else				return NIL;
}

#endif
