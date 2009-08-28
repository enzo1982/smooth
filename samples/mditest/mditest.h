 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_MDITEST
#define H_MDITEST

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class MDITest : public Application
{
	private:
		GUI::Window	*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Statusbar	*mainWnd_statusbar;
		MDI::Client	*mainWnd_client;

		Menubar		*mainWnd_menubar;
		PopupMenu	*menu_file;
	public:
				 MDITest();
				~MDITest();

		Bool		 ExitProc();
		Void		 Close();
		Void		 NewMDI();
};

#endif
