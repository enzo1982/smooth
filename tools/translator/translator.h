 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TRANSLATOR
#define H_OBJSMOOTH_TRANSLATOR

#include <smooth.h>
#include "section.h"

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

class Translator : public Application
{
	private:
		static const Short	 numInfoItems = 7;

		GUI::Window		*wnd;
		Titlebar		*title;
		Menubar			*menubar;
		Statusbar		*statusbar;

		PopupMenu		*menu_file;
		PopupMenu		*menu_entry;

		Text			*text_filter;
		EditBox			*edit_filter;
		Button			*button_clear;

		ListBox			*list_entries;
		ListBox			*list_filtered;

		Button			*button_new;
		Text			*text_id;
		EditBox			*edit_id;
		Text			*text_original;
		MultiEdit		*edit_original;
		Text			*text_translated;
		MultiEdit		*edit_translated;
		Button			*button_save;
		Button			*button_remove;

		String			 fileName;
		String			 templateName;

		Array<ListEntry *>	 entries;
		Array<ListEntry *>	 createdEntries;

		Section			*dataSection;

		Bool			 modified;

		String			 GetShortFileName(const String &);

		Int			 OpenTemplate(const String &);

		Void			 OpenFileName(const String &);
		Void			 SaveFileName(const String &);

		Void			 Close();

		Void			 ReplaceLineEndings(const String &);
		Void			 FormatLines(const String &);

		ListEntry		*GetSelectedEntry(List *);
		List			*GetEntryList(List *, Int);
	slots:
		Void			 NewEntry();
		Void			 SaveData();
		Void			 RemoveEntry();
		Void			 SelectEntry(ListEntry *);
		Void			 FilterEntries();
		Void			 ClearFilter();
		Void			 ClearFilter(List *);

		Void			 NewFile();
		Void			 OpenFile();
		Void			 CloseFile();
		Void			 SaveFile();
		Void			 SaveFileAs();

		Void			 ExportAs();

		Bool			 ExitProc();
		Void			 ResizeProc();
	public:
					 Translator(const String &);
					~Translator();
};

#endif
