 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
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

#include <smooth/gui/dialogs/directory/dirdlg_haiku.h>

class HaikuDirectoryDialog : public BLooper
{
	private:
		S::String	&directory;
	public:
		HaikuDirectoryDialog(S::String &iDirectory) : directory(iDirectory)
		{
		}

		void MessageReceived(BMessage *message)
		{
			switch (message->what)
			{
				case B_REFS_RECEIVED:
					{
						entry_ref	 dir;

						if (message->FindRef("refs", &dir) == B_OK)
						{
							BPath	 path(&dir);

							directory.ImportFrom("UTF-8", path.Path());
						}
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

class HaikuDirectoryFilter : public BRefFilter
{
	public:
		HaikuDirectoryFilter()
		{
		}

		bool Filter(const entry_ref *ref, BNode *node, stat_beos *st, const char *filetype)
		{
			if (S::String(ref->name).StartsWith(".")) return false;

			if (node->IsDirectory()) return true;

			return false;
		}
};

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	HaikuDirectoryDialog	*dialog = new HaikuDirectoryDialog(directory);
	HaikuDirectoryFilter	 filter;

	BMessenger		 messenger(dialog);
	BFilePanel		 panel(B_OPEN_PANEL, &messenger, NULL,
				       B_DIRECTORY_NODE, False, NULL, &filter, True, True);

	if (directory != NIL) panel.SetPanelDirectory(directory);

	panel.Show();

	thread_id	 thread = dialog->Run();
	status_t	 status = B_OK;

	wait_for_thread(thread, &status);

	if (directory == NIL) error = Error();

	return error;
}
