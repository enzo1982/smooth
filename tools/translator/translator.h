 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

typedef struct
{
	Int	 listid;
	Int	 id;
	String	 original;
	String	 translation;
}
listEntry;

class Translator : public Application
{
	private:
		Window		*wnd;
		Titlebar	*title;
		Menubar		*menubar;
		Statusbar	*statusbar;
		Menu		*menu_file;

		Button		*button_new;
		Text		*text_id;
		EditBox		*edit_id;
		Text		*text_original;
		EditBox		*edit_original;
		Text		*text_translated;
		EditBox		*edit_translated;
		Button		*button_save;
		Button		*button_remove;
		ListBox		*list_entries;

		String		 filename;

		Array<listEntry *>	 entries;

		Void		 OpenFile();
		Void		 SaveFile();
		Void		 SaveFileAs();
		Void		 SaveFileWithName(String);
		Void		 NewFile();
		Void		 Close();
		Void		 CloseFile();
		Void		 SaveData();
		Void		 SelectEntry();
		Void		 NewEntry();
		Void		 RemoveEntry();
		Bool		 ExitProc();
	public:
				 Translator();
				~Translator();
};

#endif
