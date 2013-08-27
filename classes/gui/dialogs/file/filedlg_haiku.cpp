 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Looper.h>
#include <Messenger.h>
#include <FilePanel.h>
#include <Path.h>
#include <OS.h>

#include <smooth/gui/dialogs/file/filedlg_haiku.h>
#include <smooth/files/directory.h>

#include <fnmatch.h>

class HaikuFileDialog : public BLooper
{
	private:
		S::Array<S::String>	&files;
	public:
		HaikuFileDialog(S::Array<S::String> &iFiles) : files(iFiles)
		{
		}

		void MessageReceived(BMessage *message)
		{
			switch (message->what)
			{
				case B_REFS_RECEIVED:
					for (int i = 0; i >= 0; i++)
					{
						entry_ref	 ref;

						if (message->FindRef("refs", i, &ref) != B_OK) break;

						BEntry		 entry(&ref, true);
						BPath		 path;

						entry.GetPath(&path);

						S::String	 file;

						file.ImportFrom("UTF-8", path.Path());
						files.Add(file);
					}

					PostMessage(B_QUIT_REQUESTED);

					break;
				case B_SAVE_REQUESTED:
					{
						entry_ref	 dir;
						const char	*name = NULL;

						if (message->FindRef("directory", &dir) != B_OK) break;
						if (message->FindString("name", &name) != B_OK) break;

						BPath		 path(&dir);
						S::String	 file(S::String(path.Path()).Append(S::Directory::GetDirectoryDelimiter()).Append(name));

						file.ImportFrom("UTF-8", file.ConvertTo("ISO-8859-1"));
						files.Add(file);
					}

					PostMessage(B_QUIT_REQUESTED);

					break;
				case B_CANCEL:
					PostMessage(B_QUIT_REQUESTED);

					break;
				default:
					BLooper::MessageReceived(message);

					break;
			};
		}
};

class HaikuFileFilter : public BRefFilter
{
	private:
		const S::Array<S::String>	&filterNames;
		const S::Array<S::String>	&filters;
	public:
		HaikuFileFilter(const S::Array<S::String> &iFilterNames, const S::Array<S::String> &iFilters) : filterNames(iFilterNames), filters(iFilters)
		{
		}

		bool Filter(const entry_ref *ref, BNode *node, stat_beos *st, const char *filetype)
		{
			if (S::String(ref->name).StartsWith(".")) return false;

			if (node->IsDirectory()) return true;

			for (int i = 0; i < filters.Length(); i++)
			{
				bool				 admit	  = false;
				const S::Array<S::String>	&patterns = filters.GetNth(i).Explode(";");

				for (int j = 0; j < patterns.Length(); j++)
				{
					S::String	 pattern = patterns.GetNth(j);

					if (fnmatch(pattern, ref->name, 0) == 0)
					{
						admit = true;

						break;
					}
				}

				S::String::ExplodeFinish();

				if (admit) return true;
			}

			return false;
		}
};

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	HaikuFileDialog	*dialog = new HaikuFileDialog(files);
	HaikuFileFilter	 filter(filterNames, filters);

	BMessenger	 messenger(dialog);
	BFilePanel	 panel(mode == SFM_OPEN ? B_OPEN_PANEL : B_SAVE_PANEL, &messenger, NULL,
			       B_FILE_NODE, flags & SFD_ALLOWMULTISELECT, NULL, &filter, True, True);

	if (mode == SFM_SAVE) panel.SetPanelDirectory(File(defFile).GetFilePath());

	panel.Show();

	thread_id	 thread = dialog->Run();
	status_t	 status = B_OK;

	wait_for_thread(thread, &status);

	if (files.Length() == 0) error = Error();

	return error;
}
