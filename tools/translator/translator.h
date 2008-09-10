 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TRANSLATOR_
#define _H_OBJSMOOTH_TRANSLATOR_

class Translator;

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

typedef struct
{
	ListEntry	*entry;
	Int		 id;
	String		 original;
	String		 translation;
}
listEntry;

class Translator : public Application
{
	private:
		GUI::Window		*wnd;
		Titlebar		*title;
		Menubar			*menubar;
		Statusbar		*statusbar;
		PopupMenu		*menu_file;

		Button			*button_new;
		Text			*text_id;
		EditBox			*edit_id;
		Text			*text_original;
		MultiEdit		*edit_original;
		Text			*text_translated;
		MultiEdit		*edit_translated;
		Button			*button_save;
		Button			*button_remove;
		ListBox			*list_entries;

		String			 filename;

		Array<listEntry *>	 entries;

		Bool			 modified;

		Void			 OpenFileName(const String &);
		Void			 SaveFileName(const String &);

		Void			 Close();
	slots:
		Void			 NewEntry();
		Void			 SaveData();
		Void			 RemoveEntry();
		Void			 SelectEntry();

		Void			 NewFile();
		Void			 OpenFile();
		Void			 CloseFile();
		Void			 SaveFile();
		Void			 SaveFileAs();

		Bool			 ExitProc();
		Void			 ResizeProc();
	public:
					 Translator(const String &);
					~Translator();
};

#endif
